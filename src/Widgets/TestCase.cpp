/*
 * Copyright (C) 2019-2020 Ashar Khan <ashar786khan@gmail.com>
 *
 * This file is part of CP Editor.
 *
 * CP Editor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * I will not be responsible if CP Editor behaves in unexpected way and
 * causes your ratings to go down and or lose any important contest.
 *
 * Believe Software is "Software" and it isn't immune to bugs.
 *
 */

#include "Widgets/TestCase.hpp"
#include "Core/EventLogger.hpp"
#include "Core/MessageLogger.hpp"
#include "Widgets/DiffViewer.hpp"
#include "Widgets/TestCaseEdit.hpp"
#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <Util.hpp>
#include <generated/SettingsHelper.hpp>

namespace Widgets
{
TestCase::TestCase(int index, MessageLogger *logger, QWidget *parent, const QString &in, const QString &exp)
    : QWidget(parent), log(logger)
{
    LOG_INFO("Testcase " << index << " is being created");
    mainLayout = new QHBoxLayout(this);
    inputUpLayout = new QHBoxLayout();
    outputUpLayout = new QHBoxLayout();
    expectedUpLayout = new QHBoxLayout();
    inputLayout = new QVBoxLayout();
    outputLayout = new QVBoxLayout();
    expectedLayout = new QVBoxLayout();
    showCheckBox = new QCheckBox();
    inputLabel = new QLabel("Input");
    outputLabel = new QLabel("Output");
    expectedLabel = new QLabel("Expected");
    runButton = new QPushButton("Run");
    diffButton = new QPushButton("**");
    delButton = new QPushButton("Del");
    inputEdit = new TestCaseEdit(true, log, in);
    outputEdit = new TestCaseEdit(false, log);
    expectedEdit = new TestCaseEdit(true, log, exp);
    diffViewer = new DiffViewer(this);

    setID(index);
    outputEdit->setReadOnly(true);
    inputEdit->setWordWrapMode(QTextOption::NoWrap);
    outputEdit->setWordWrapMode(QTextOption::NoWrap);
    expectedEdit->setWordWrapMode(QTextOption::NoWrap);

    showCheckBox->setMinimumWidth(20);
    showCheckBox->setChecked(true);
    showCheckBox->setSizePolicy({QSizePolicy::Fixed, QSizePolicy::Fixed});

    inputUpLayout->addWidget(showCheckBox);
    inputUpLayout->addWidget(inputLabel);
    inputUpLayout->addWidget(runButton);
    outputUpLayout->addWidget(outputLabel);
    outputUpLayout->addWidget(diffButton);
    expectedUpLayout->addWidget(expectedLabel);
    expectedUpLayout->addWidget(delButton);
    inputLayout->addLayout(inputUpLayout);
    inputLayout->addWidget(inputEdit);
    outputLayout->addLayout(outputUpLayout);
    outputLayout->addWidget(outputEdit);

    expectedLayout->addLayout(expectedUpLayout);
    expectedLayout->addWidget(expectedEdit);
    mainLayout->addLayout(inputLayout);
    mainLayout->addLayout(outputLayout);
    mainLayout->addLayout(expectedLayout);

    runButton->setToolTip("Test on a single testcase");
    diffButton->setToolTip("Open the Diff Viewer");

    connect(showCheckBox, SIGNAL(toggled(bool)), this, SLOT(onShowCheckBoxToggled(bool)));
    connect(runButton, SIGNAL(clicked()), this, SLOT(onRunButtonClicked()));
    connect(diffButton, SIGNAL(clicked()), SLOT(onDiffButtonClicked()));
    connect(delButton, SIGNAL(clicked()), this, SLOT(onDelButtonClicked()));
    connect(diffViewer, SIGNAL(toLongForHtml()), this, SLOT(onToLongForHtml()));
}

void TestCase::setInput(const QString &text)
{
    LOG_INFO("Input value updated to\n" << text);
    inputEdit->modifyText(text);
}

void TestCase::setOutput(const QString &text)
{
    auto newOutput = text;

    if (text.length() > SettingsHelper::getOutputLengthLimit())
    {
        newOutput = "Output Length Limit Exceeded";
        log->error("Testcases",
                   QString("The output #%1 contains more than %2 characters, so it's not displayed. You can set the "
                           "output length limit in Preferences->Advanced->Limits->Output Length Limit")
                       .arg(id + 1)
                       .arg(SettingsHelper::getOutputLengthLimit()));
    }

    LOG_INFO("Output value updated to\n" << text);
    outputEdit->modifyText(newOutput);
    outputEdit->startAnimation();

    if (!diffViewer->isHidden())
        diffViewer->setText(newOutput, expected());
}

void TestCase::setExpected(const QString &text)
{
    LOG_INFO("Expected value updated to\n" << text);
    expectedEdit->modifyText(text);
}

void TestCase::clearOutput()
{
    outputEdit->modifyText(QString());
    currentVerdict = Core::Checker::UNKNOWN;
    diffButton->setStyleSheet("");
    diffButton->setText("**");
}

QString TestCase::input() const
{
    return inputEdit->toPlainText();
}

QString TestCase::output() const
{
    return outputEdit->toPlainText();
}

QString TestCase::expected() const
{
    return expectedEdit->toPlainText();
}

void TestCase::setID(int index)
{
    LOG_INFO("Changed testcase ID to " << index);
    id = index;
    inputLabel->setText("Input #" + QString::number(id + 1));
    outputLabel->setText("Output #" + QString::number(id + 1));
    expectedLabel->setText("Expected #" + QString::number(id + 1));
}

void TestCase::setVerdict(Core::Checker::Verdict verdict)
{
    currentVerdict = verdict;

    LOG_INFO("Changed verdict to " << INFO_OF(verdict));

    switch (currentVerdict)
    {
    case Core::Checker::UNKNOWN:
        diffButton->setStyleSheet("");
        diffButton->setText("**");
        break;
    case Core::Checker::AC:
        diffButton->setStyleSheet("background: #0b0");
        diffButton->setText("AC");
        break;
    case Core::Checker::WA:
        diffButton->setStyleSheet("background: #d00");
        diffButton->setText("WA");
        break;
    }
}

Core::Checker::Verdict TestCase::verdict() const
{
    return currentVerdict;
}

void TestCase::setShow(bool show)
{
    showCheckBox->setChecked(show);
}

bool TestCase::isShow() const
{
    return showCheckBox->isChecked();
}

void TestCase::setTestCaseEditFont(const QFont &font)
{
    inputEdit->setFont(font);
    outputEdit->setFont(font);
    expectedEdit->setFont(font);
}

void TestCase::onShowCheckBoxToggled(bool checked)
{
    if (checked)
    {
        inputEdit->show();
        outputEdit->show();
        expectedEdit->show();
    }
    else
    {
        inputEdit->hide();
        outputEdit->hide();
        expectedEdit->hide();
    }
}

void TestCase::onRunButtonClicked()
{
    LOG_INFO("Run button clicked for " << INFO_OF(id));
    showCheckBox->setChecked(true);
    emit requestRun(id);
}

void TestCase::onDiffButtonClicked()
{
    LOG_INFO("Diff button clicked for " << INFO_OF(id));
    diffViewer->setText(output(), expected());
    diffViewer->show();
    diffViewer->raise();
}

void TestCase::onDelButtonClicked()
{
    LOG_INFO("Del button clicked for " << INFO_OF(id));
    if (input().isEmpty() && expected().isEmpty())
    {
        emit deleted(this);
    }
    else
    {
        auto res = QMessageBox::question(this, "Delete Testcase",
                                         "Do you want to delete test case #" + QString::number(id + 1));
        if (res == QMessageBox::Yes)
            emit deleted(this);
    }
}

void TestCase::onToLongForHtml()
{
    log->warn(
        "Diff Viewer[" + QString::number(id + 1) + "]",
        QString("The output/expected contains more than %1 characters, HTML diff viewer is disabled. You can change "
                "the length limit in Preferences->Advanced->Limits->HTML Diff Viewer Length Limit")
            .arg(SettingsHelper::getHTMLDiffViewerLengthLimit()));
}

} // namespace Widgets
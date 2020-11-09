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
#include "Util/Util.hpp"
#include "Widgets/DiffViewer.hpp"
#include "Widgets/TestCaseEdit.hpp"
#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QSplitter>
#include <QVBoxLayout>
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
    splitter = new QSplitter(this);
    inputWidget = new QWidget(this);
    outputWidget = new QWidget(this);
    expectedWidget = new QWidget(this);
    inputLayout = new QVBoxLayout(inputWidget);
    outputLayout = new QVBoxLayout(outputWidget);
    expectedLayout = new QVBoxLayout(expectedWidget);
    showCheckBox = new QCheckBox(this);
    inputLabel = new QLabel(tr("Input"), this);
    outputLabel = new QLabel(tr("Output"), this);
    expectedLabel = new QLabel(tr("Expected"), this);
    runButton = new QPushButton(tr("Run"), this);
    diffButton = new QPushButton("**", this);
    delButton = new QPushButton(tr("Del"), this);
    inputEdit = new TestCaseEdit(TestCaseEdit::Input, index, log, in, this);
    outputEdit = new TestCaseEdit(TestCaseEdit::Output, index, log, QString(), this);
    expectedEdit = new TestCaseEdit(TestCaseEdit::Expected, index, log, exp, this);
    diffViewer = new DiffViewer(this);

    setID(index);

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
    splitter->addWidget(inputWidget);
    splitter->addWidget(outputWidget);
    splitter->addWidget(expectedWidget);
    mainLayout->addWidget(splitter);

    inputLayout->setContentsMargins(3, 0, 3, 0);
    outputLayout->setContentsMargins(3, 0, 3, 0);
    expectedLayout->setContentsMargins(3, 0, 3, 0);

    splitter->setChildrenCollapsible(false);

    runButton->setToolTip(tr("Test on a single testcase"));
    diffButton->setToolTip(tr("Open the Diff Viewer"));

    connect(showCheckBox, SIGNAL(toggled(bool)), this, SLOT(onShowCheckBoxToggled(bool)));
    connect(runButton, SIGNAL(clicked()), this, SLOT(onRunButtonClicked()));
    connect(diffButton, SIGNAL(clicked()), SLOT(onDiffButtonClicked()));
    connect(delButton, SIGNAL(clicked()), this, SLOT(onDelButtonClicked()));
    connect(diffViewer, SIGNAL(toLongForHtml()), this, SLOT(onToLongForHtml()));
}

void TestCase::setInput(const QString &text)
{
    inputEdit->modifyText(text);
}

void TestCase::setOutput(const QString &text)
{
    outputEdit->modifyText(text);
    outputEdit->startAnimation();

    if (!diffViewer->isHidden())
        diffViewer->setText(text, expected());
}

void TestCase::setExpected(const QString &text)
{
    expectedEdit->modifyText(text);
}

void TestCase::clearOutput()
{
    outputEdit->modifyText(QString());
    currentVerdict = UNKNOWN;
    diffButton->setStyleSheet("");
    diffButton->setText("**");
}

QString TestCase::input() const
{
    return inputEdit->getText();
}

QString TestCase::output() const
{
    return outputEdit->getText();
}

QString TestCase::expected() const
{
    return expectedEdit->getText();
}

void TestCase::setID(int index)
{
    LOG_INFO("Changed testcase ID to " << index);
    id = index;
    inputLabel->setText(tr("Input #%1").arg(id + 1));
    outputLabel->setText(tr("Output #%1").arg(id + 1));
    expectedLabel->setText(tr("Expected #%1").arg(id + 1));
}

void TestCase::setVerdict(Verdict verdict)
{
    currentVerdict = verdict;

    LOG_INFO("Changed verdict to " << INFO_OF(verdict));

    switch (currentVerdict)
    {
    case UNKNOWN:
        diffButton->setStyleSheet("");
        diffButton->setText("**");
        break;
    case AC:
        diffButton->setStyleSheet("background: #0b0");
        diffButton->setText("AC");
        break;
    case WA:
        diffButton->setStyleSheet("background: #d00");
        diffButton->setText("WA");
        break;
    case TLE:
        diffButton->setStyleSheet("background: #990");
        diffButton->setText("TLE");
        break;
    case RE:
        diffButton->setStyleSheet("background: #b0b");
        diffButton->setText("RE");
        break;
    default:
        Q_UNREACHABLE();
        break;
    }
}

TestCase::Verdict TestCase::verdict() const
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

void TestCase::updateHeight()
{
    inputEdit->startAnimation();
    outputEdit->startAnimation();
    expectedEdit->startAnimation();
}

QList<int> TestCase::splitterSizes() const
{
    return splitter->sizes();
}

void TestCase::restoreSplitterSizes(const QList<int> &sizes)
{
    splitter->setSizes(sizes);
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
    Util::showWidgetOnTop(diffViewer);
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
        auto res = QMessageBox::question(this, tr("Delete Testcase"),
                                         tr("Are you sure you want to delete test case #%1?").arg(id + 1));
        if (res == QMessageBox::Yes)
            emit deleted(this);
    }
}

void TestCase::onToLongForHtml()
{
    log->warn(tr("Diff Viewer[%1]").arg(id + 1),
              tr("The output/expected contains more than %1 characters, HTML diff viewer is disabled. You can change "
                 "the length limit at %2.")
                  .arg(SettingsHelper::getHTMLDiffViewerLengthLimit())
                  .arg(SettingsHelper::pathOfHTMLDiffViewerLengthLimit()),
              false);
}

} // namespace Widgets
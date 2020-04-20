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
#include <QMessageBox>
#include <Util.hpp>
#include <generated/SettingsHelper.hpp>

namespace Widgets
{
TestCase::TestCase(int index, MessageLogger *logger, QWidget *parent, const QString &in, const QString &exp)
    : QWidget(parent), log(logger)
{
    Core::Log::i("testcase/constructed") << "index " << index << " input " << in << " expected " << exp << endl;
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

    Core::Log::i("testcase/constructed", "UI has been built");

    connect(showCheckBox, SIGNAL(toggled(bool)), this, SLOT(onShowCheckBoxToggled(bool)));
    connect(runButton, SIGNAL(clicked()), this, SLOT(onRunButtonClicked()));
    connect(diffButton, SIGNAL(clicked()), SLOT(onDiffButtonClicked()));
    connect(delButton, SIGNAL(clicked()), this, SLOT(onDelButtonClicked()));
    connect(diffViewer, SIGNAL(toLongForHtml()), this, SLOT(onToLongForHtml()));

    Core::Log::i("testcase/constructed", "signals have been attached");
}

void TestCase::setInput(const QString &text)
{
    Core::Log::i("testcase/setInput") << "text \n" << text << endl;
    inputEdit->modifyText(text);
}

void TestCase::setOutput(const QString &text)
{
    Core::Log::i("testcase/setOutput") << "text \n" << text << endl;

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

    outputEdit->modifyText(newOutput);
    outputEdit->startAnimation();

    if (!diffViewer->isHidden())
        diffViewer->setText(newOutput, expected());
}

void TestCase::setExpected(const QString &text)
{
    Core::Log::i("testcase/setExpected") << "text \n" << text << endl;
    expectedEdit->modifyText(text);
}

void TestCase::clearOutput()
{
    Core::Log::i("testcase/clearOutput", "Cleared output");
    outputEdit->modifyText(QString());
    currentVerdict = Core::Checker::UNKNOWN;
    diffButton->setStyleSheet("");
    diffButton->setText("**");
}

QString TestCase::input() const
{
    Core::Log::i("testcase/input", "Invoked");
    return inputEdit->toPlainText();
}

QString TestCase::output() const
{
    Core::Log::i("testcase/output", "Invoked");
    return outputEdit->toPlainText();
}

QString TestCase::expected() const
{
    Core::Log::i("testcase/expected", "Invoked");
    return expectedEdit->toPlainText();
}

void TestCase::loadFromFile(const QString &pathPrefix)
{
    Core::Log::i("testcase/loadFromFile") << "pathPrefix " << pathPrefix << endl;
    auto content = Util::readFile(pathPrefix + ".in", "Read Input #" + QString::number(id + 1), log);
    if (!content.isNull())
    {
        if (content.length() > SettingsHelper::getLoadTestCaseFileLengthLimit())
        {
            log->error(
                "Testcases",
                QString("The testcase file [%1] contains more than %2 characters, so it's not loaded. You can change "
                        "the length limit in Preferences->Advanced->Limits->Load Test Case File Length Limit")
                    .arg(pathPrefix + ".in")
                    .arg(SettingsHelper::getLoadTestCaseFileLengthLimit()));
        }
        else
            setInput(content);
    }
    content = Util::readFile(pathPrefix + ".ans", "Read Expected #" + QString::number(id + 1), log);
    if (!content.isNull())
    {
        if (content.length() > SettingsHelper::getLoadTestCaseFileLengthLimit())
        {
            log->error(
                "Testcases",
                QString("The testcase file [%1] contains more than %2 characters, so it's not loaded. You can change "
                        "the length limit in Preferences->Advanced->Limits->Load Test Case File Length Limit")
                    .arg(pathPrefix + ".ans")
                    .arg(SettingsHelper::getLoadTestCaseFileLengthLimit()));
        }
        else
            setExpected(content);
    }
}

void TestCase::save(const QString &pathPrefix, bool safe)
{
    Core::Log::i("testcase/save") << "pathPrefix " << pathPrefix << endl;

    if (!input().isEmpty() || QFile::exists(pathPrefix + ".in"))
    {
        Core::Log::i("testcase/save", "Okay, Input file exists and should be saved");
        Util::saveFile(pathPrefix + ".in", input(), "Testcase Input #" + QString::number(id + 1), safe, log);
    }
    if (!expected().isEmpty() || QFile::exists(pathPrefix + ".ans"))
    {
        Core::Log::i("testcase/save", "Okay, Expected file exists and should be saved");
        Util::saveFile(pathPrefix + ".ans", expected(), "Testcase Expected #" + QString::number(id + 1), safe, log);
    }
}

void TestCase::setID(int index)
{
    Core::Log::i("testcase/setID") << "index " << index << endl;
    id = index;
    inputLabel->setText("Input #" + QString::number(id + 1));
    outputLabel->setText("Output #" + QString::number(id + 1));
    expectedLabel->setText("Expected #" + QString::number(id + 1));
}

void TestCase::setVerdict(Core::Checker::Verdict verdict)
{
    currentVerdict = verdict;

    Core::Log::i("testcase/setVerdict") << INFO_OF(verdict) << endl;

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
    Core::Log::i("testcase/verdict", "Invoked");
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
    Core::Log::i("TestCase/onRunButtonClicked", "Invoked");
    showCheckBox->setChecked(true);
    emit requestRun(id);
}

void TestCase::onDiffButtonClicked()
{
    Core::Log::i("testcase/on_diffButton_clicked", "invoked");
    diffViewer->setText(output(), expected());
    diffViewer->show();
    diffViewer->raise();
}

void TestCase::onDelButtonClicked()
{
    Core::Log::i("TestCase/onDelButtonClicked", "Invoked");

    if (input().isEmpty() && expected().isEmpty())
    {
        Core::Log::i("TestCase/onDelButtonClicked", "Deleted Directly because it's empty");
        emit deleted(this);
    }
    else
    {
        Core::Log::i("TestCase/onDelButtonClicked", "Asking confirmation for delete");
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
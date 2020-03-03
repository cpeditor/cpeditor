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
#include <QFileDialog>
#include <QMessageBox>
#include <Util.hpp>

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
    moreButton = new QPushButton("More");
    loadInputButton = new QPushButton("Load");
    diffButton = new QPushButton("**");
    loadExpectedButton = new QPushButton("Load");
    inputEdit = new TestCaseEdit(true, in);
    outputEdit = new TestCaseEdit(false);
    expectedEdit = new TestCaseEdit(true, exp);
    moreMenu = new QMenu();
    diffViewer = new DiffViewer(this);

    setID(index);
    outputEdit->setReadOnly(true);
    inputEdit->setWordWrapMode(QTextOption::NoWrap);
    outputEdit->setWordWrapMode(QTextOption::NoWrap);
    expectedEdit->setWordWrapMode(QTextOption::NoWrap);

    showCheckBox->setMinimumWidth(20);
    showCheckBox->setChecked(true);
    showCheckBox->setSizePolicy({QSizePolicy::Fixed, QSizePolicy::Fixed});

    loadExpectedButton->setMinimumWidth(
        loadExpectedButton->fontMetrics().boundingRect(loadExpectedButton->text()).width() + 10);

    moreButton->setMinimumWidth(moreButton->fontMetrics().boundingRect(moreButton->text()).width() + 30);

    moreMenu->addAction("Delete", [this] {
        Core::Log::i("TestCases/moreMenu/Delete", "Invoked");

        if (input().isEmpty() && expected().isEmpty())
        {
            Core::Log::i("TestCases/moreMenu/Delete", "Deleted Directly because it's empty");
            emit deleted(this);
        }
        else
        {
            Core::Log::i("TestCases/moreMenu/Delete", "Asking confirmation for delete");
            auto res = QMessageBox::question(this, "Delete Testcase",
                                             "Do you want to delete test case #" + QString::number(id + 1));
            if (res == QMessageBox::Yes)
                emit deleted(this);
        }
    });

    moreMenu->addAction("Run", [this] {
        showCheckBox->setChecked(true);
        emit requestRun(id);
    });

    moreButton->setMenu(moreMenu);

    inputUpLayout->addWidget(showCheckBox);
    inputUpLayout->addWidget(inputLabel);
    inputUpLayout->addWidget(loadInputButton);
    outputUpLayout->addWidget(outputLabel);
    outputUpLayout->addWidget(diffButton);
    expectedUpLayout->addWidget(expectedLabel);
    expectedUpLayout->addWidget(loadExpectedButton);
    expectedUpLayout->addWidget(moreButton);
    inputLayout->addLayout(inputUpLayout);
    inputLayout->addWidget(inputEdit);
    outputLayout->addLayout(outputUpLayout);
    outputLayout->addWidget(outputEdit);

    expectedLayout->addLayout(expectedUpLayout);
    expectedLayout->addWidget(expectedEdit);
    mainLayout->addLayout(inputLayout);
    mainLayout->addLayout(outputLayout);
    mainLayout->addLayout(expectedLayout);

    Core::Log::i("testcase/constructed", "UI has been built");

    connect(showCheckBox, SIGNAL(toggled(bool)), this, SLOT(on_showCheckBox_toggled(bool)));
    connect(loadInputButton, SIGNAL(clicked()), this, SLOT(on_loadInputButton_clicked()));
    connect(diffButton, SIGNAL(clicked()), SLOT(on_diffButton_clicked()));
    connect(loadExpectedButton, SIGNAL(clicked()), this, SLOT(on_loadExpectedButton_clicked()));
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

    outputEdit->modifyText(text);
    outputEdit->startAnimation();

    if (!diffViewer->isHidden())
        diffViewer->setText(output(), expected());
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
    QFile inputFile(pathPrefix + ".in");
    if (inputFile.exists())
    {
        Core::Log::i("testcase/loadFromFile", "Okay, Input file exists");
        if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
            setInput(inputFile.readAll());
        else
            log->warn("Tests", "Failed to load Input #" + QString::number(id + 1) + ". Do I have read permission?");
    }
    QFile expectedFile(pathPrefix + ".ans");
    if (expectedFile.exists())
    {
        Core::Log::i("testcase/loadFromFile", "Okay, Expected file exists");
        if (expectedFile.open(QIODevice::ReadOnly | QIODevice::Text))
            setExpected(expectedFile.readAll());
        else
            log->warn("Tests", "Failed to load Expected #" + QString::number(id + 1) + ". Do I have read permission?");
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

void TestCase::on_showCheckBox_toggled(bool checked)
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

void TestCase::on_loadInputButton_clicked()
{
    Core::Log::i("testcase/on_loadInputButton_clicked", "invoked");
    auto res = QFileDialog::getOpenFileName(this, "Load Input");
    QFile file(res);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        setInput(file.readAll());
    else
        log->warn("Tests", "Failed to load input file " + res);
}

void TestCase::on_diffButton_clicked()
{
    Core::Log::i("testcase/on_diffButton_clicked", "invoked");
    diffViewer->setText(output(), expected());
    diffViewer->show();
    diffViewer->raise();
}

void TestCase::on_loadExpectedButton_clicked()
{
    Core::Log::i("settingmanager/on_loadExpectedButton_clicked", "invoked");
    auto res = QFileDialog::getOpenFileName(this, "Load Expected");
    QFile file(res);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        setExpected(file.readAll());
    else
        log->warn("Tests", "Failed to load expected file " + res);
}

void TestCase::onToLongForHtml()
{
    log->warn("Diff Viewer[" + QString::number(id + 1) + "]", "The output/expected is longer than " +
                                                                  QString::number(DiffViewer::MAX_CHARACTERS_FOR_HTML) +
                                                                  " characters, use plain text diff");
}

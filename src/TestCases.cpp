/*
 * Copyright (C) 2019-2020 Ashar Khan <ashar786khan@gmail.com>
 *
 * This file is part of CPEditor.
 *
 * CPEditor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * I will not be responsible if CPEditor behaves in unexpected way and
 * causes your ratings to go down and or loose any important contest.
 *
 * Believe Software is "Software" and it isn't immune to bugs.
 *
 */

#include "TestCases.hpp"
#include "diff_match_patch.h"
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QMimeData>
#include <QSaveFile>
#include <QScrollBar>

TestCaseEdit::TestCaseEdit(bool autoAnimation, const QString &text, QWidget *parent) : QPlainTextEdit(text, parent)
{
    animation = new QPropertyAnimation(this, "minimumHeight", this);
    if (autoAnimation)
        connect(this, SIGNAL(textChanged()), this, SLOT(startAnimation()));
    startAnimation();
}

TestCaseEdit::TestCaseEdit(bool autoAnimation, QWidget *parent) : QPlainTextEdit(parent)
{
    animation = new QPropertyAnimation(this, "minimumHeight", this);
    if (autoAnimation)
        connect(this, SIGNAL(textChanged()), this, SLOT(startAnimation()));
    startAnimation();
}

void TestCaseEdit::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
    {
        event->accept();
        event->acceptProposedAction();
    }
}

void TestCaseEdit::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasUrls())
    {
        event->accept();
        event->acceptProposedAction();
    }
}

void TestCaseEdit::dropEvent(QDropEvent *event)
{
    auto urls = event->mimeData()->urls();
    if (!isReadOnly() && !urls.isEmpty())
    {
        QFile file(urls[0].toLocalFile());
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
            modifyText(file.readAll());
        event->acceptProposedAction();
    }
}

void TestCaseEdit::modifyText(const QString &text)
{
    auto cursor = textCursor();
    cursor.select(QTextCursor::Document);
    cursor.insertText(text);
}

void TestCaseEdit::startAnimation()
{
    int newHeight = qMin(fontMetrics().lineSpacing() * (document()->lineCount() + 2) + 5, 300);
    if (newHeight != minimumHeight())
    {
        animation->stop();
        animation->setStartValue(minimumHeight());
        animation->setEndValue(newHeight);
        animation->start();
    }
}

TestCase::TestCase(int index, MessageLogger *logger, QWidget *parent, const QString &input, const QString &expected)
    : QWidget(parent), log(logger)
{
    mainLayout = new QHBoxLayout(this);
    inputUpLayout = new QHBoxLayout();
    outputUpLayout = new QHBoxLayout();
    expectedUpLayout = new QHBoxLayout();
    inputLayout = new QVBoxLayout();
    outputLayout = new QVBoxLayout();
    expectedLayout = new QVBoxLayout();
    inputLabel = new QLabel("Input");
    outputLabel = new QLabel("Output");
    expectedLabel = new QLabel("Expected");
    deleteButton = new QPushButton("Delete");
    loadInputButton = new QPushButton("Load");
    diffButton = new QPushButton("**");
    loadExpectedButton = new QPushButton("Load");
    inputEdit = new TestCaseEdit(true, input);
    outputEdit = new TestCaseEdit(false);
    expectedEdit = new TestCaseEdit(true, expected);

    setID(index);
    outputEdit->setReadOnly(true);
    inputEdit->setWordWrapMode(QTextOption::NoWrap);
    outputEdit->setWordWrapMode(QTextOption::NoWrap);
    expectedEdit->setWordWrapMode(QTextOption::NoWrap);

    inputUpLayout->addWidget(inputLabel);
    inputUpLayout->addWidget(loadInputButton);
    outputUpLayout->addWidget(outputLabel);
    outputUpLayout->addWidget(diffButton);
    expectedUpLayout->addWidget(expectedLabel);
    expectedUpLayout->addWidget(loadExpectedButton);
    expectedUpLayout->addWidget(deleteButton);
    inputLayout->addLayout(inputUpLayout);
    inputLayout->addWidget(inputEdit);
    outputLayout->addLayout(outputUpLayout);
    outputLayout->addWidget(outputEdit);

    expectedLayout->addLayout(expectedUpLayout);
    expectedLayout->addWidget(expectedEdit);
    mainLayout->addLayout(inputLayout);
    mainLayout->addLayout(outputLayout);
    mainLayout->addLayout(expectedLayout);

    connect(deleteButton, SIGNAL(clicked()), this, SLOT(on_deleteButton_clicked()));
    connect(loadInputButton, SIGNAL(clicked()), this, SLOT(on_loadInputButton_clicked()));
    connect(diffButton, SIGNAL(clicked()), SLOT(on_diffButton_clicked()));
    connect(loadExpectedButton, SIGNAL(clicked()), this, SLOT(on_loadExpectedButton_clicked()));
}

void TestCase::setInput(const QString &text)
{
    inputEdit->modifyText(text);
}

void TestCase::setOutput(const QString &text)
{
    outputEdit->modifyText(text);
    outputEdit->startAnimation();
    if (input().trimmed().isEmpty() || expected().trimmed().isEmpty() || text.trimmed().isEmpty())
    {
        diffButton->setStyleSheet("");
        diffButton->setText("**");
    }
    else if (isPass())
    {
        diffButton->setStyleSheet("background: #0b0");
        diffButton->setText("AC");
    }
    else
    {
        diffButton->setStyleSheet("background: #d00");
        diffButton->setText("WA");
    }
}

void TestCase::setExpected(const QString &text)
{
    expectedEdit->modifyText(text);
}

void TestCase::clearOutput()
{
    outputEdit->modifyText(QString());
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

void TestCase::loadFromFile(const QString &pathPrefix)
{
    QFile inputFile(pathPrefix + ".in");
    if (inputFile.exists())
    {
        if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
            setInput(inputFile.readAll());
        else
            log->warn("Tests", "Failed to load Input #" + QString::number(id + 1) + ". Do I have read permission?");
    }
    QFile expectedFile(pathPrefix + ".ans");
    if (expectedFile.exists())
    {
        if (expectedFile.open(QIODevice::ReadOnly | QIODevice::Text))
            setExpected(expectedFile.readAll());
        else
            log->warn("Tests", "Failed to load Expected #" + QString::number(id + 1) + ". Do I have read permission?");
    }
}

void TestCase::save(const QString &pathPrefix)
{
    if (!input().isEmpty() || QFile::exists(pathPrefix + ".in"))
    {
        QSaveFile inputFile(pathPrefix + ".in");
        inputFile.open(QIODevice::WriteOnly | QIODevice::Text);
        inputFile.write(input().toUtf8());
        if (!inputFile.commit())
            log->warn("Tests", "Failed to save Input #" + QString::number(id + 1) + ". Do I have write permission?");
    }
    if (!expected().isEmpty() || QFile::exists(pathPrefix + ".ans"))
    {
        QSaveFile expectedFile(pathPrefix + ".ans");
        expectedFile.open(QIODevice::WriteOnly | QIODevice::Text);
        expectedFile.write(expected().toUtf8());
        if (!expectedFile.commit())
            log->warn("Tests", "Failed to save Expected #" + QString::number(id + 1) + ". Do I have write permission?");
    }
}

void TestCase::setID(int index)
{
    id = index;
    inputLabel->setText("Input #" + QString::number(id + 1));
    outputLabel->setText("Output #" + QString::number(id + 1));
    expectedLabel->setText("Expected #" + QString::number(id + 1));
}

void TestCase::on_deleteButton_clicked()
{
    if (input().isEmpty() && expected().isEmpty())
    {
        emit deleted(this);
    }
    else
    {
        auto res = QMessageBox::question(this, "Delete Testcase", "Do you want to delete this test case?");
        if (res == QMessageBox::Yes)
            emit deleted(this);
    }
}

void TestCase::on_loadInputButton_clicked()
{
    auto res = QFileDialog::getOpenFileName(this, "Load Input");
    QFile file(res);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        setInput(file.readAll());
    else
        log->warn("Tests", "Failed to load input file " + res);
}

void TestCase::on_diffButton_clicked()
{
    auto window = new QMainWindow(this);
    auto widget = new QWidget(window);
    auto layout = new QHBoxLayout();
    widget->setLayout(layout);
    window->setCentralWidget(widget);
    window->setWindowTitle("Diff Viewer");
    window->resize(800, 600);
    window->setAttribute(Qt::WA_DeleteOnClose);

    auto leftLayout = new QVBoxLayout();
    auto expectedLabel = new QLabel("Expected", widget);
    leftLayout->addWidget(expectedLabel);
    auto expectedEdit = new QTextEdit(widget);
    expectedEdit->setReadOnly(true);
    expectedEdit->setWordWrapMode(QTextOption::NoWrap);
    leftLayout->addWidget(expectedEdit);
    layout->addLayout(leftLayout);

    auto rightLayout = new QVBoxLayout();
    auto outputLabel = new QLabel("Output", widget);
    rightLayout->addWidget(outputLabel);
    auto outputEdit = new QTextEdit(widget);
    outputEdit->setReadOnly(true);
    outputEdit->setWordWrapMode(QTextOption::NoWrap);
    rightLayout->addWidget(outputEdit);
    layout->addLayout(rightLayout);

    if (output().length() <= 100000 && expected().length() <= 100000)
    {
        diff_match_patch differ;
        differ.Diff_EditCost = 10;
        auto diffs = differ.diff_main(expected(), output());
        differ.diff_cleanupEfficiency(diffs);

        QString expectedHTML, outputHTML;
        for (auto diff : diffs)
        {
            QString text = diff.text.toHtmlEscaped().replace("\n", "&para;<br>").replace(" ", "&nbsp;");
            switch (diff.operation)
            {
            case INSERT:
                outputHTML += QString("<ins style=\"background:#8f8;\">") + text + QString("</ins>");
                break;
            case DELETE:
                expectedHTML += "<s style=\"background:#f88;\">" + text + "</s>";
                break;
            case EQUAL:
                expectedHTML += "<span>" + text + "</span>";
                outputHTML += "<span>" + text + "</span>";
                break;
            }
        }
        QPalette p = expectedEdit->palette();
        p.setColor(QPalette::Base, Qt::white); // for system dark theme
        p.setColor(QPalette::Text, Qt::black);
        expectedEdit->setPalette(p);
        expectedEdit->setHtml(expectedHTML);
        outputEdit->setPalette(p);
        outputEdit->setHtml(outputHTML);
    }
    else
    {
        QMessageBox::warning(this, "Diff Viewer", "The output/expected is too large, use plain diff.");
        expectedEdit->setPlainText(expected());
        outputEdit->setPlainText(output());
        rightLayout->addWidget(outputEdit);
        layout->addLayout(rightLayout);
    }

    connect(expectedEdit->horizontalScrollBar(), SIGNAL(valueChanged(int)), outputEdit->horizontalScrollBar(),
            SLOT(setValue(int)));
    connect(outputEdit->horizontalScrollBar(), SIGNAL(valueChanged(int)), expectedEdit->horizontalScrollBar(),
            SLOT(setValue(int)));
    connect(expectedEdit->verticalScrollBar(), SIGNAL(valueChanged(int)), outputEdit->verticalScrollBar(),
            SLOT(setValue(int)));
    connect(outputEdit->verticalScrollBar(), SIGNAL(valueChanged(int)), expectedEdit->verticalScrollBar(),
            SLOT(setValue(int)));

    window->show();
}

void TestCase::on_loadExpectedButton_clicked()
{
    auto res = QFileDialog::getOpenFileName(this, "Load Expected");
    QFile file(res);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        setExpected(file.readAll());
    else
        log->warn("Tests", "Failed to load expected file " + res);
}

bool TestCase::isPass()
{
    auto out = output().remove('\r');
    auto ans = expected().remove('\r');
    auto a_lines = out.split('\n');
    auto b_lines = ans.split('\n');
    for (int i = 0; i < a_lines.size() || i < b_lines.size(); ++i)
    {
        if (i >= a_lines.size())
        {
            if (b_lines[i].trimmed().isEmpty())
                continue;
            else
                return false;
        }
        if (i >= b_lines.size())
        {
            if (a_lines[i].trimmed().isEmpty())
                continue;
            else
                return false;
        }
        auto a_words = a_lines[i].split(' ');
        auto b_words = b_lines[i].split(' ');
        for (int j = 0; j < a_words.size() || j < b_words.size(); ++j)
        {
            if (j >= a_words.size())
            {
                if (b_words[j].trimmed().isEmpty())
                    continue;
                else
                    return false;
            }
            if (j >= b_words.size())
            {
                if (a_words[j].trimmed().isEmpty())
                    continue;
                else
                    return false;
            }
            if (a_words[j] != b_words[j])
                return false;
        }
    }
    return true;
}

TestCases::TestCases(MessageLogger *logger, QWidget *parent) : QWidget(parent), log(logger)
{
    mainLayout = new QVBoxLayout(this);
    titleLayout = new QHBoxLayout();
    label = new QLabel("Test Cases");
    addButton = new QPushButton("Add Test Case");
    scrollArea = new QScrollArea();
    scrollAreaWidget = new QWidget();
    scrollAreaLayout = new QVBoxLayout(scrollAreaWidget);

    titleLayout->addWidget(label);
    titleLayout->addWidget(addButton);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(scrollAreaWidget);
    mainLayout->addLayout(titleLayout);
    mainLayout->addWidget(scrollArea);

    connect(addButton, SIGNAL(clicked()), this, SLOT(on_addButton_clicked()));
}

void TestCases::setInput(int index, const QString &input)
{
    testcases[index]->setInput(input);
}

void TestCases::setOutput(int index, const QString &output)
{
    testcases[index]->setOutput(output);
}

void TestCases::setExpected(int index, const QString &expected)
{
    testcases[index]->setExpected(expected);
}

void TestCases::addTestCase(const QString &input, const QString &expected)
{
    if (count() >= MAX_NUMBER_OF_TESTCASES)
    {
        QMessageBox::warning(this, "Add Test Case",
                             "There are already " + QString::number(count()) + " test cases, you can't add more.");
    }
    else
    {
        auto testcase = new TestCase(count(), log, this, input, expected);
        connect(testcase, SIGNAL(deleted(TestCase *)), this, SLOT(onChildDeleted(TestCase *)));
        testcases.push_back(testcase);
        scrollAreaLayout->addWidget(testcase);
    }
}

void TestCases::clearOutput()
{
    for (int i = 0; i < count(); ++i)
        testcases[i]->clearOutput();
}

void TestCases::clear()
{
    while (count() > 0)
        onChildDeleted(testcases.front());
}

QString TestCases::input(int index) const
{
    return testcases[index]->input();
}

QString TestCases::output(int index) const
{
    return testcases[index]->output();
}

QString TestCases::expected(int index) const
{
    return testcases[index]->expected();
}

void TestCases::loadStatus(const QStringList &inputList, const QStringList &expectedList)
{
    clear();
    for (int i = 0; i < inputList.length(); ++i)
        addTestCase(inputList[i], expectedList[i]);
}

QStringList TestCases::inputs() const
{
    QStringList res;
    for (int i = 0; i < count(); ++i)
        res.append(testcases[i]->input());
    return res;
}

QStringList TestCases::expecteds() const
{
    QStringList res;
    for (int i = 0; i < count(); ++i)
        res.append(testcases[i]->expected());
    return res;
}

void TestCases::loadFromFile(const QString &filePath)
{
    QFileInfo fileInfo(filePath);
    auto dir = fileInfo.dir();
    auto name = fileInfo.completeBaseName();
    int maxIndex = 0;
    auto entries = dir.entryInfoList({name + "*.in", name + "*.ans"}, QDir::Files);
    for (auto entry : entries)
        maxIndex = qMax(maxIndex, numberOfTestFile(name, entry));
    maxIndex = qMin(maxIndex, MAX_NUMBER_OF_TESTCASES);
    clear();
    for (int i = 0; i < maxIndex; ++i)
    {
        addTestCase();
        testcases[i]->loadFromFile(testFilePathPrefix(fileInfo, i));
    }
}

void TestCases::save(const QString &filePath)
{
    QFileInfo fileInfo(filePath);
    auto dir = fileInfo.dir();
    auto name = fileInfo.completeBaseName();
    for (int i = 0; i < count(); ++i)
        testcases[i]->save(testFilePathPrefix(fileInfo, i));
    auto entries = dir.entryInfoList({name + "*.in", name + "*.ans"}, QDir::Files);
    for (auto entry : entries)
    {
        int number = numberOfTestFile(name, entry);
        if (number > count() && number <= MAX_NUMBER_OF_TESTCASES)
        {
            auto res = QMessageBox::question(this, "Save Tests",
                                             entry.fileName() + " is deleted in the editor, delete it on the disk?");
            if (res == QMessageBox::Yes)
                QFile(entry.filePath()).remove();
        }
    }
}

int TestCases::id(TestCase *testcase) const
{
    return testcases.indexOf(testcase);
}

int TestCases::count() const
{
    return testcases.count();
}

void TestCases::on_addButton_clicked()
{
    addTestCase();
}

void TestCases::onChildDeleted(TestCase *widget)
{
    testcases.removeOne(widget);
    widget->hide();
    scrollAreaLayout->removeWidget(widget);
    delete widget;
    for (int i = 0; i < count(); ++i)
        testcases[i]->setID(i);
}

QString TestCases::testFilePathPrefix(const QFileInfo &fileInfo, int index)
{
    return fileInfo.dir().filePath(fileInfo.completeBaseName() + "_" + QString::number(index + 1));
}

int TestCases::numberOfTestFile(const QString &sourceName, const QFileInfo &fileName)
{
    auto baseName = fileName.completeBaseName();
    return baseName.mid(baseName.indexOf(sourceName) + sourceName.length() + 1).toInt();
}

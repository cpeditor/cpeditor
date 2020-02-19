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

#include "Widgets/TestCases.hpp"
#include "Core/EventLogger.hpp"
#include "diff_match_patch.h"
#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QMimeData>
#include <QSaveFile>
#include <QScrollBar>
#include <Util.hpp>

TestCaseEdit::TestCaseEdit(bool autoAnimation, const QString &text, QWidget *parent) : QPlainTextEdit(text, parent)
{
    Core::Log::i("testcaseEdit/constructed") << "autoAnimate " << autoAnimation << " text " << text << endl;
    animation = new QPropertyAnimation(this, "minimumHeight", this);
    if (autoAnimation)
        connect(this, SIGNAL(textChanged()), this, SLOT(startAnimation()));
    startAnimation();
}

TestCaseEdit::TestCaseEdit(bool autoAnimation, QWidget *parent) : QPlainTextEdit(parent)
{
    Core::Log::i("testcaseEdit/constructed") << "autoAnimate " << autoAnimation << endl;
    animation = new QPropertyAnimation(this, "minimumHeight", this);
    if (autoAnimation)
        connect(this, SIGNAL(textChanged()), this, SLOT(startAnimation()));
    startAnimation();
}

void TestCaseEdit::dragEnterEvent(QDragEnterEvent *event)
{
    Core::Log::i("testCaseEdit/dragEnterEvent", "Something is being dropped into testcaseEdit");
    if (event->mimeData()->hasUrls())
    {
        Core::Log::i("testCaseEdit/dragEnterEvent", "Accepting dropped object");
        event->accept();
        event->acceptProposedAction();
    }
}

void TestCaseEdit::dragMoveEvent(QDragMoveEvent *event)
{
    Core::Log::i("testCaseEdit/dragMoveEvent", "Something is being Moved into testcaseEdit");

    if (event->mimeData()->hasUrls())
    {
        Core::Log::i("testCaseEdit/dragMoveEvent", "Accepting moved object");
        event->accept();
        event->acceptProposedAction();
    }
}

void TestCaseEdit::dropEvent(QDropEvent *event)
{
    Core::Log::i("testcaseEdit/dropEvent", "Something has been dropped");
    auto urls = event->mimeData()->urls();
    if (!isReadOnly() && !urls.isEmpty())
    {
        Core::Log::i("testcaseEdit/dropEvent", "Reading the dropped file to testcase");
        QFile file(urls[0].toLocalFile());
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
            modifyText(file.readAll());
        event->acceptProposedAction();
        Core::Log::i("testcaseEdit/dropEvent") << "dropped file was " << file.fileName() << endl;
    }
}

void TestCaseEdit::modifyText(const QString &text)
{
    Core::Log::i("testcasesEdit/modifyText") << "text " << text << endl;
    auto cursor = textCursor();
    cursor.select(QTextCursor::Document);
    cursor.insertText(text);
}

void TestCaseEdit::startAnimation()
{
    Core::Log::i("testcaseEdit/startAnimation", "started Animation");
    int newHeight = qMin(fontMetrics().lineSpacing() * (document()->lineCount() + 2) + 5, 300);
    if (newHeight != minimumHeight())
    {
        animation->stop();
        animation->setStartValue(minimumHeight());
        animation->setEndValue(newHeight);
        animation->start();
        Core::Log::i("testcaseEdit/startAnimation", "re-started Animation");
    }
}

TestCase::TestCase(int index, MessageLogger *logger, QWidget *parent, const QString &input, const QString &expected)
    : QWidget(parent), log(logger)
{
    Core::Log::i("testcase/constructed") << "index " << index << " input " << input << " expected " << expected << endl;
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
    deleteButton = new QPushButton("Del");
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

    auto setWidth = [](QPushButton *btn) {
        btn->setMinimumWidth(btn->fontMetrics().boundingRect(btn->text()).width() + 10);
    };
    setWidth(loadExpectedButton);
    setWidth(deleteButton);

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

    Core::Log::i("testcase/constructed", "UI has been built");

    connect(deleteButton, SIGNAL(clicked()), this, SLOT(on_deleteButton_clicked()));
    connect(loadInputButton, SIGNAL(clicked()), this, SLOT(on_loadInputButton_clicked()));
    connect(diffButton, SIGNAL(clicked()), SLOT(on_diffButton_clicked()));
    connect(loadExpectedButton, SIGNAL(clicked()), this, SLOT(on_loadExpectedButton_clicked()));

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

    currentVerdict = output().isEmpty() || expected().isEmpty() ? UNKNOWN : (isPass() ? AC : WA);

    Core::Log::i("testcase/setOutput") << "currentVerdict " << currentVerdict << endl;

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
    }
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
    currentVerdict = UNKNOWN;
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

TestCase::Verdict TestCase::verdict() const
{
    Core::Log::i("testcase/verdict", "Invoked");
    return currentVerdict;
}

void TestCase::on_deleteButton_clicked()
{
    Core::Log::i("testcase/on_deleteButton_clicked", "Invoked");

    if (input().isEmpty() && expected().isEmpty())
    {
        Core::Log::i("testcase/on_deleteButton_clicked", "Deleted Directly because empty");
        emit deleted(this);
    }
    else
    {
        Core::Log::i("testcase/on_deleteButton_clicked", "Asking confirmation for delete");
        auto res = QMessageBox::question(this, "Delete Testcase", "Do you want to delete this test case?");
        if (res == QMessageBox::Yes)
            emit deleted(this);
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
    auto window = new QMainWindow(this);
    auto widget = new QWidget(window);
    auto layout = new QHBoxLayout();
    widget->setLayout(layout);
    window->setCentralWidget(widget);
    window->setWindowTitle("Diff Viewer");
    window->resize(800, 600);
    window->setAttribute(Qt::WA_DeleteOnClose);

    auto leftLayout = new QVBoxLayout();
    auto outputLabel = new QLabel("Output", widget);
    leftLayout->addWidget(outputLabel);
    auto outputEdit = new QTextEdit(widget);
    outputEdit->setReadOnly(true);
    outputEdit->setWordWrapMode(QTextOption::NoWrap);
    leftLayout->addWidget(outputEdit);
    layout->addLayout(leftLayout);

    auto rightLayout = new QVBoxLayout();
    auto expectedLabel = new QLabel("Expected", widget);
    rightLayout->addWidget(expectedLabel);
    auto expectedEdit = new QTextEdit(widget);
    expectedEdit->setReadOnly(true);
    expectedEdit->setWordWrapMode(QTextOption::NoWrap);
    rightLayout->addWidget(expectedEdit);
    layout->addLayout(rightLayout);

    if (output().length() <= 100000 && expected().length() <= 100000)
    {
        Core::Log::i("testcase/on_diffButton_clicked", "less than 10^5 size of output and expected. Highlighting now");
        diff_match_patch differ;
        differ.Diff_EditCost = 10;
        auto diffs = differ.diff_main(output(), expected());
        differ.diff_cleanupEfficiency(diffs);

        QString expectedHTML, outputHTML;
        for (auto diff : diffs)
        {
            QString text = diff.text.toHtmlEscaped().replace("\n", "&para;<br>").replace(" ", "&nbsp;");
            switch (diff.operation)
            {
            case INSERT:
                expectedHTML += QString("<ins style=\"background:#8f8;\">") + text + QString("</ins>");
                break;
            case DELETE:
                outputHTML += "<s style=\"background:#f88;\">" + text + "</s>";
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
        QMessageBox::warning(this, "Diff Viewer", "The output/expected is too large, using plain diff.");
        expectedEdit->setPlainText(expected());
        outputEdit->setPlainText(output());
    }

    connect(expectedEdit->horizontalScrollBar(), SIGNAL(valueChanged(int)), outputEdit->horizontalScrollBar(),
            SLOT(setValue(int)));
    connect(outputEdit->horizontalScrollBar(), SIGNAL(valueChanged(int)), expectedEdit->horizontalScrollBar(),
            SLOT(setValue(int)));
    connect(expectedEdit->verticalScrollBar(), SIGNAL(valueChanged(int)), outputEdit->verticalScrollBar(),
            SLOT(setValue(int)));
    connect(outputEdit->verticalScrollBar(), SIGNAL(valueChanged(int)), expectedEdit->verticalScrollBar(),
            SLOT(setValue(int)));

    Core::Log::i("testcase/on_diffButton_clicked", "connnections established, showing diff window");

    window->show();
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

bool TestCase::isPass() const
{
    Core::Log::i("settingmanager/isPass", "invoked");
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

const int TestCases::MAX_NUMBER_OF_TESTCASES;

TestCases::TestCases(MessageLogger *logger, QWidget *parent) : QWidget(parent), log(logger)
{
    Core::Log::i("testcases/constructed", "invoked");
    mainLayout = new QVBoxLayout(this);
    titleLayout = new QHBoxLayout();
    label = new QLabel("Test Cases");
    verdicts = new QLabel();
    hideACButton = new QPushButton("Hide AC");
    addButton = new QPushButton("Add New");
    clearButton = new QPushButton("Clear");
    scrollArea = new QScrollArea();
    scrollAreaWidget = new QWidget();
    scrollAreaLayout = new QVBoxLayout(scrollAreaWidget);

    Core::Log::i("testcases/constructed", "widgets created");

    titleLayout->addWidget(label);
    titleLayout->addWidget(verdicts);
    titleLayout->addWidget(hideACButton);
    titleLayout->addWidget(addButton);
    titleLayout->addWidget(clearButton);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(scrollAreaWidget);
    mainLayout->addLayout(titleLayout);
    mainLayout->addWidget(scrollArea);

    Core::Log::i("testcases/constructed", "widgets attached");

    updateVerdicts();

    connect(hideACButton, SIGNAL(clicked()), this, SLOT(on_hideACButton_clicked()));
    connect(addButton, SIGNAL(clicked()), this, SLOT(on_addButton_clicked()));
    connect(clearButton, SIGNAL(clicked()), this, SLOT(on_clearButton_clicked()));

    Core::Log::i("testcases/constructed", "connection established");
}

void TestCases::setInput(int index, const QString &input)
{
    Core::Log::i("testcases/setInput") << "index : " << input << "\n" << input << endl;
    testcases[index]->setInput(input);
}

void TestCases::setOutput(int index, const QString &output)
{
    Core::Log::i("testcases/setoutput") << "index : " << index << " output \n" << output << endl;
    testcases[index]->setOutput(output);
    updateVerdicts();
}

void TestCases::setExpected(int index, const QString &expected)
{
    Core::Log::i("testcases/setExpected") << "index : " << index << " expected \n" << expected << endl;
    testcases[index]->setExpected(expected);
}

void TestCases::addTestCase(const QString &input, const QString &expected)
{
    if (count() >= MAX_NUMBER_OF_TESTCASES)
    {
        Core::Log::w("testcases/addTestcase", "Max testcase limit reached");
        QMessageBox::warning(this, "Add Test Case",
                             "There are already " + QString::number(count()) + " test cases, you can't add more.");
    }
    else
    {
        Core::Log::w("testcases/addTestcase", "New testcase added");
        auto testcase = new TestCase(count(), log, this, input, expected);
        connect(testcase, SIGNAL(deleted(TestCase *)), this, SLOT(onChildDeleted(TestCase *)));
        testcases.push_back(testcase);
        scrollAreaLayout->addWidget(testcase);
        updateVerdicts();
    }
}

void TestCases::clearOutput()
{
    Core::Log::w("testcases/clearOutput", "invoked");
    for (int i = 0; i < count(); ++i)
        testcases[i]->clearOutput();
    updateVerdicts();
}

void TestCases::clear()
{
    Core::Log::w("testcases/clear", "invoked");
    while (count() > 0)
        onChildDeleted(testcases.front());
}

QString TestCases::input(int index) const
{
    Core::Log::w("testcases/input") << " index " << index << endl;
    return testcases[index]->input();
}

QString TestCases::output(int index) const
{
    Core::Log::w("testcases/output") << " index " << index << endl;
    return testcases[index]->output();
}

QString TestCases::expected(int index) const
{
    Core::Log::w("testcases/expected") << " index " << index << endl;
    return testcases[index]->expected();
}

void TestCases::loadStatus(const QStringList &inputList, const QStringList &expectedList)
{
    Core::Log::w("testcases/loadStatus", "invoked");
    clear();
    for (int i = 0; i < inputList.length(); ++i)
        addTestCase(inputList[i], expectedList[i]);
}

QStringList TestCases::inputs() const
{
    Core::Log::w("testcases/inputs", "invoked");
    QStringList res;
    for (int i = 0; i < count(); ++i)
        res.append(testcases[i]->input());
    return res;
}

QStringList TestCases::expecteds() const
{
    Core::Log::w("testcases/expecteds", "invoked");
    QStringList res;
    for (int i = 0; i < count(); ++i)
        res.append(testcases[i]->expected());
    return res;
}

void TestCases::loadFromFile(const QString &filePath)
{
    Core::Log::i("testcases/loadFromFile") << "filepath " << filePath << endl;
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
    if (maxIndex == 0)
        addTestCase();
}

void TestCases::save(const QString &filePath, bool safe)
{
    Core::Log::i("testcases/save") << "filepath " << filePath << endl;
    QFileInfo fileInfo(filePath);
    auto dir = fileInfo.dir();
    auto name = fileInfo.completeBaseName();
    for (int i = 0; i < count(); ++i)
        testcases[i]->save(testFilePathPrefix(fileInfo, i), safe);
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
    Core::Log::i("testcases/id", "invoked");
    return testcases.indexOf(testcase);
}

int TestCases::count() const
{
    Core::Log::i("testcases/count", "invoked");
    return testcases.count();
}

void TestCases::on_hideACButton_clicked()
{
    Core::Log::i("testcases/on_hideACButton_clicked") << "Invoked, " << INFO_OF(isHideAC) << endl;
    isHideAC ^= 1;
    if (isHideAC)
        hideACButton->setText("Show AC");
    else
        hideACButton->setText("Hide AC");
    updateVerdicts();
}

void TestCases::on_addButton_clicked()
{
    Core::Log::i("testcases/on_addButton_clicked", "invoked");
    addTestCase();
}

void TestCases::on_clearButton_clicked()
{
    Core::Log::i("testcases/on_clearButton_clicked", "invoked");
    for (int i = 0; i < count(); ++i)
    {
        if (input(i).isEmpty() && output(i).isEmpty() && expected(i).isEmpty())
        {
            onChildDeleted(testcases[i]);
            --i;
        }
    }
    if (count() > 0)
    {
        auto res = QMessageBox::question(this, "Clear Test Cases", "Do you want to delete all test cases?");
        if (res == QMessageBox::Yes)
            clear();
    }
}

void TestCases::onChildDeleted(TestCase *widget)
{
    Core::Log::i("testcases/onChildDeleted", "invoked");
    testcases.removeOne(widget);
    widget->hide();
    scrollAreaLayout->removeWidget(widget);
    delete widget;
    for (int i = 0; i < count(); ++i)
        testcases[i]->setID(i);
    updateVerdicts();
}

void TestCases::updateVerdicts()
{
    Core::Log::i("testcases/updateVerdicts", "invoked");
    int ac = 0, wa = 0;
    for (auto t : testcases)
    {
        switch (t->verdict())
        {
        case TestCase::AC:
            ++ac;
            if (isHideAC)
                t->hide();
            else
                t->show();
            break;
        case TestCase::WA:
            ++wa;
            t->show();
            break;
        case TestCase::UNKNOWN:
            t->show();
            break;
        }
    }
    verdicts->setText("<span style=\"color:red\">" + QString::number(wa) + "</span> / <span style=\"color:green\">" +
                      QString::number(ac) + "</span> / " + QString::number(count()));
}

QString TestCases::testFilePathPrefix(const QFileInfo &fileInfo, int index)
{
    Core::Log::i("testcases/testFilePathPrefix") << "index " << index << endl;
    return fileInfo.dir().filePath(fileInfo.completeBaseName() + "_" + QString::number(index + 1));
}

int TestCases::numberOfTestFile(const QString &sourceName, const QFileInfo &fileName)
{
    Core::Log::i("testcases/numberofTestFile") << "sourceName " << sourceName << endl;
    auto baseName = fileName.completeBaseName();
    return baseName.mid(baseName.indexOf(sourceName) + sourceName.length() + 1).toInt();
}

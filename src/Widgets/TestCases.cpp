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
 * causes your ratings to go down and or lose any important contest.
 *
 * Believe Software is "Software" and it isn't immune to bugs.
 *
 */

#include "Widgets/TestCases.hpp"
#include "Core/EventLogger.hpp"
#include <QFileDialog>
#include <QMessageBox>

const int TestCases::MAX_NUMBER_OF_TESTCASES;

TestCases::TestCases(MessageLogger *logger, QWidget *parent) : QWidget(parent), log(logger)
{
    Core::Log::i("testcases/constructed", "invoked");
    mainLayout = new QVBoxLayout(this);
    titleLayout = new QHBoxLayout();
    checkerLayout = new QHBoxLayout();
    label = new QLabel("Test Cases");
    verdicts = new QLabel();
    checkerLabel = new QLabel("Checker:");
    addButton = new QPushButton("Add Test");
    moreButton = new QPushButton("More");
    addCheckerButton = new QPushButton("Add Checker");
    checkerComboBox = new QComboBox();
    scrollArea = new QScrollArea();
    scrollAreaWidget = new QWidget();
    scrollAreaLayout = new QVBoxLayout(scrollAreaWidget);

    Core::Log::i("testcases/constructed", "widgets created");

    titleLayout->addWidget(label);
    titleLayout->addWidget(verdicts);
    titleLayout->addWidget(addButton);
    titleLayout->addWidget(moreButton);
    checkerLayout->addWidget(checkerLabel);
    checkerLayout->addWidget(checkerComboBox);
    checkerLayout->addWidget(addCheckerButton);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(scrollAreaWidget);
    mainLayout->addLayout(titleLayout);
    mainLayout->addLayout(checkerLayout);
    mainLayout->addWidget(scrollArea);

    Core::Log::i("testcases/constructed", "widgets attached");

    updateVerdicts();

    moreMenu = new QMenu();

    moreMenu->addAction("Remove Empty", [this] {
        Core::Log::i("TestCases/More/Remove Empty", "Invoked");
        for (int i = 0; i < count(); ++i)
        {
            if (input(i).isEmpty() && output(i).isEmpty() && expected(i).isEmpty())
            {
                onChildDeleted(testcases[i]);
                --i;
            }
        }
    });

    moreMenu->addAction("Remove All", [this] {
        Core::Log::i("testcases/More/Remove All", "invoked");
        auto res = QMessageBox::question(this, "Clear Testcases", "Do you want to delete all test cases?");
        if (res == QMessageBox::Yes)
        {
            for (int i = 0; i < count(); ++i)
            {
                onChildDeleted(testcases[i]);
                --i;
            }
        }
    });

    moreMenu->addAction("Hide AC", [this] {
        Core::Log::i("testcases/More/Hide AC", "Invoked");
        for (auto t : testcases)
            if (t->verdict() == Core::Checker::AC)
                t->setShow(false);
    });

    moreMenu->addAction("Show All", [this] {
        Core::Log::i("TestCases/More/Show All", "Invoked");
        for (auto t : testcases)
            t->setShow(true);
    });

    moreMenu->addAction("Hide All", [this] {
        Core::Log::i("TestCases/More/Hide All", "Invoked");
        for (auto t : testcases)
            t->setShow(false);
    });

    moreMenu->addAction("Inverse", [this] {
        Core::Log::i("TestCases/More/Inverse", "Invoked");
        for (auto t : testcases)
            t->setShow(t->isShow() ^ 1);
    });

    moreButton->setMenu(moreMenu);

    checkerLabel->setSizePolicy({QSizePolicy::Maximum, QSizePolicy::Fixed});
    checkerComboBox->setSizePolicy({QSizePolicy::Expanding, QSizePolicy::Fixed});
    addCheckerButton->setSizePolicy({QSizePolicy::Maximum, QSizePolicy::Fixed});
    checkerComboBox->setMinimumWidth(100);

    checkerComboBox->addItems({"Ignore trailing spaces", "Strictly the same", "ncmp - Compare int64s",
                               "rcmp4 - Compare doubles, max error 1e-4", "rcmp6 - Compare doubles, max error 1e-6",
                               "rcmp9 - Compare doubles, max error 1e-9", "wcmp - Compare tokens",
                               "nyesno - Compare YES/NOs, case insensitive"});
    checkerComboBox->setCurrentIndex(0);

    connect(checkerComboBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(checkerChanged()));
    connect(addButton, SIGNAL(clicked()), this, SLOT(on_addButton_clicked()));
    connect(addCheckerButton, SIGNAL(clicked()), this, SLOT(on_addCheckerButton_clicked()));

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
        connect(testcase, SIGNAL(requestRun(int)), this, SIGNAL(requestRun(int)));
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
            QFile(entry.filePath()).remove();
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

void TestCases::setCheckerIndex(int index)
{
    if (index >= 0 && index < checkerComboBox->count())
        checkerComboBox->setCurrentIndex(index);
    else
        Core::Log::e("TestCases/setCheckerIndex") << "Invalid checker index " << index << endl;
}

int TestCases::checkerIndex() const
{
    return checkerComboBox->currentIndex();
}

void TestCases::addCustomCheckers(const QStringList &list)
{
    checkerComboBox->addItems(list);
}

QStringList TestCases::customCheckers() const
{
    QStringList res;
    for (int i = 8; i < checkerComboBox->count(); ++i)
        res.push_back(checkerComboBox->itemText(i));
    return res;
}

QString TestCases::checkerText() const
{
    return checkerComboBox->currentText();
}

Core::Checker::CheckerType TestCases::checkerType() const
{
    Core::Log::i("TestCases/checkerType") << INFO_OF(checkerIndex()) << endl;
    switch (checkerIndex())
    {
    case 0:
        return Core::Checker::IgnoreTrailingSpaces;
    case 1:
        return Core::Checker::Strict;
    case 2:
        return Core::Checker::Ncmp;
    case 3:
        return Core::Checker::Rcmp4;
    case 4:
        return Core::Checker::Rcmp6;
    case 5:
        return Core::Checker::Rcmp9;
    case 6:
        return Core::Checker::Wcmp;
    case 7:
        return Core::Checker::Nyesno;
    default:
        return Core::Checker::Custom;
    }
}

void TestCases::setShow(int index, bool show)
{
    testcases[index]->setShow(show);
}

bool TestCases::isShow(int index) const
{
    return testcases[index]->isShow();
}

void TestCases::setVerdict(int index, Core::Checker::Verdict verdict)
{
    Core::Log::i("TestCases/setVerdict") << INFO_OF(index) << ", " << INFO_OF(verdict) << endl;
    testcases[index]->setVerdict(verdict);
    updateVerdicts();
}

void TestCases::on_addButton_clicked()
{
    Core::Log::i("testcases/on_addButton_clicked", "invoked");
    addTestCase();
}

void TestCases::on_addCheckerButton_clicked()
{
    Core::Log::i("TestCases/on_addCheckerButton_clicked", "Invoked");
    auto path = QFileInfo(QFileDialog::getOpenFileName(this, "Add Checker")).canonicalFilePath();
    if (!path.isEmpty())
    {
        checkerComboBox->addItem(path);
        checkerComboBox->setCurrentIndex(checkerComboBox->count() - 1);
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
        case Core::Checker::AC:
            ++ac;
            break;
        case Core::Checker::WA:
            ++wa;
            break;
        case Core::Checker::UNKNOWN:
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

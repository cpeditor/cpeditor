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

#include "Widgets/TestCases.hpp"
#include "Core/EventLogger.hpp"
#include "generated/SettingsHelper.hpp"
#include <QFileDialog>
#include <QMessageBox>

namespace Widgets
{
const int TestCases::MAX_NUMBER_OF_TESTCASES;

TestCases::TestCases(MessageLogger *logger, QWidget *parent) : QWidget(parent), log(logger)
{
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

    verdicts->setToolTip("Wrong Answer / Accepted / Total");
    addCheckerButton->setToolTip("Add a custom testlib checker");

    updateVerdicts();

    moreMenu = new QMenu();

    moreMenu->addAction("Add Testcases", [this] {
        QStringList paths = QFileDialog::getOpenFileNames(this, "Choose testcases", ""); // set to the src dir
        LOG_INFO(paths.join(", "));
        if (paths.size())
        {
            QStringList rules = SettingsHelper::getAddTestcasesFromFilesRule();
            for (auto input : paths)
            {
                auto it = rules.begin();
                bool finded = false;
                QString answer;
                while (!finded && it != rules.end())
                {
                    QRegularExpression re(*it++);
                    auto match = re.match(input);
                    if (match.hasMatch())
                    {
                        answer = *it++;
                        auto caps = match.capturedTexts();
                        for (int i = 1; i < caps.size(); ++i)
                            answer = answer.arg(caps[i]);
                        finded = true;
                    }
                }
                if (finded)
                {
                    QFile infile(input), ansfile(answer);
                    QString inbuffer, ansbuffer;
                    infile.open(QIODevice::ReadOnly);
                    inbuffer = QString::fromUtf8(infile.readAll());
                    if (ansfile.exists())
                    {
                        ansfile.open(QIODevice::ReadOnly);
                        ansbuffer = QString::fromUtf8(ansfile.readAll());
                    }
                    addTestCase(inbuffer, ansbuffer);
                }
            }
        }
    });

    moreMenu->addAction("Remove Empty", [this] {
        LOG_INFO("Testcases Removing empty");
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
        LOG_INFO("Testcases removing all testcases");
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
        LOG_INFO("Testcases hiding all Accepted");
        for (auto t : testcases)
            if (t->verdict() == Core::Checker::AC)
                t->setShow(false);
    });

    moreMenu->addAction("Show All", [this] {
        LOG_INFO("Testcases making all cases visible");
        for (auto t : testcases)
            t->setShow(true);
    });

    moreMenu->addAction("Hide All", [this] {
        LOG_INFO("Testcases Hiding all cases");
        for (auto t : testcases)
            t->setShow(false);
    });

    moreMenu->addAction("Invert", [this] {
        LOG_INFO("Testcases Inverting all cases");
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
        LOG_WARN("Max testcase limit reached");
        QMessageBox::warning(this, "Add Test Case",
                             "There are already " + QString::number(count()) + " test cases, you can't add more.");
    }
    else
    {
        LOG_INFO("New testcase added");
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
    for (int i = 0; i < count(); ++i)
        testcases[i]->clearOutput();
    updateVerdicts();
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
    if (maxIndex == 0)
        addTestCase();
}

void TestCases::save(const QString &filePath, bool safe)
{
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
    return testcases.indexOf(testcase);
}

int TestCases::count() const
{
    return testcases.count();
}

void TestCases::setCheckerIndex(int index)
{
    if (index >= 0 && index < checkerComboBox->count())
        checkerComboBox->setCurrentIndex(index);
    else
        LOG_ERR("Invalid checker index " << index);
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
    testcases[index]->setVerdict(verdict);
    updateVerdicts();
}

void TestCases::on_addButton_clicked()
{
    addTestCase();
}

void TestCases::on_addCheckerButton_clicked()
{
    LOG_INFO("Add checker button clicked");
    auto path = QFileInfo(QFileDialog::getOpenFileName(this, "Add Checker")).canonicalFilePath();
    if (!path.isEmpty())
    {
        checkerComboBox->addItem(path);
        checkerComboBox->setCurrentIndex(checkerComboBox->count() - 1);
    }
}

void TestCases::onChildDeleted(TestCase *widget)
{
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
    return fileInfo.dir().filePath(fileInfo.completeBaseName() + "_" + QString::number(index + 1));
}

int TestCases::numberOfTestFile(const QString &sourceName, const QFileInfo &fileName)
{
    LOG_INFO(INFO_OF(sourceName) << INFO_OF(fileName.absolutePath()));
    auto baseName = fileName.completeBaseName();
    return baseName.mid(baseName.indexOf(sourceName) + sourceName.length() + 1).toInt();
}
} // namespace Widgets

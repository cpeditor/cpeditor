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

#ifndef TESTCASES_HPP
#define TESTCASES_HPP

#include "Core/Checker.hpp"
#include "Core/MessageLogger.hpp"
#include "Widgets/TestCase.hpp"
#include <QComboBox>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

class TestCases : public QWidget
{
    Q_OBJECT

  public:
    explicit TestCases(MessageLogger *logger, QWidget *parent = nullptr);
    void setInput(int index, const QString &input);
    void setOutput(int index, const QString &output);
    void setExpected(int index, const QString &expected);
    void addTestCase(const QString &input = QString(), const QString &expected = QString());
    void clearOutput();
    void clear();
    QString input(int index) const;
    QString output(int index) const;
    QString expected(int index) const;
    void loadStatus(const QStringList &inputList, const QStringList &expectedList);
    QStringList inputs() const;
    QStringList expecteds() const;
    void loadFromFile(const QString &filePath);
    void save(const QString &filePath, bool safe);
    int id(TestCase *testcase) const;
    int count() const;
    void setCheckerIndex(int index);
    int checkerIndex() const;
    void addCustomCheckers(const QStringList &list);
    QStringList customCheckers() const;
    QString checkerText() const;
    Core::Checker::CheckerType checkerType() const;
    void setShow(int index, bool show);
    bool isShow(int index) const;

  public slots:
    void setVerdict(int index, Core::Checker::Verdict verdict);

  signals:
    void checkerChanged();
    void requestRun(int index);

  private slots:
    void on_addButton_clicked();
    void on_addCheckerButton_clicked();
    void onChildDeleted(TestCase *widget);

  private:
    static const int MAX_NUMBER_OF_TESTCASES = 100;
    QVBoxLayout *mainLayout = nullptr, *scrollAreaLayout = nullptr;
    QHBoxLayout *titleLayout = nullptr, *checkerLayout = nullptr;
    QPushButton *addButton = nullptr, *moreButton = nullptr, *addCheckerButton = nullptr;
    QMenu *moreMenu = nullptr;
    QComboBox *checkerComboBox = nullptr;
    QScrollArea *scrollArea = nullptr;
    QWidget *scrollAreaWidget = nullptr;
    QLabel *label = nullptr, *verdicts = nullptr, *checkerLabel = nullptr;
    QList<TestCase *> testcases;
    MessageLogger *log;
    bool choosingChecker = false;

    void updateVerdicts();
    QString testFilePathPrefix(const QFileInfo &fileInfo, int index);
    int numberOfTestFile(const QString &sourceName, const QFileInfo &fileName);
};

#endif // TESTCASES_HPP

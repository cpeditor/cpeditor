/*
 * Copyright (C) 2019-2021 Ashar Khan <ashar786khan@gmail.com>
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

#ifndef TESTCASES_HPP
#define TESTCASES_HPP

#include "Core/Checker.hpp"
#include <QWidget>

class MessageLogger;
class QComboBox;
class QHBoxLayout;
class QLabel;
class QMenu;
class QPushButton;
class QScrollArea;
class QVBoxLayout;

namespace Widgets
{
class TestCase;

class TestCases : public QWidget
{
    Q_OBJECT

  public:
    explicit TestCases(MessageLogger *logger, QWidget *parent = nullptr);

    QString input(int index) const;
    QString output(int index) const;
    QString expected(int index) const;

    void setInput(int index, const QString &input);
    void setOutput(int index, const QString &output);
    void setExpected(int index, const QString &expected);

    void loadStatus(const QStringList &inputList, const QStringList &expectedList);

    QStringList inputs() const;
    QStringList expecteds() const;

    void addTestCase(const QString &input = QString(), const QString &expected = QString());

    void clearOutput();
    void clear();

    int id(TestCase *testcase) const;
    int count() const;

    void setCheckerIndex(int index);
    int checkerIndex() const;
    void addCustomCheckers(const QStringList &list);
    QStringList customCheckers() const;
    QString checkerText() const;
    Core::Checker::CheckerType checkerType() const;

    void setChecked(int index, bool checked);
    bool isChecked(int index) const;

    void loadFromSavedFiles(const QString &filePath);
    void saveToFiles(const QString &filePath, bool safe);

    QString loadTestCaseFromFile(const QString &path, const QString &head);

    void setTestCaseEditFont(const QFont &font);

    void updateHeights();

    QVariantList splitterStates() const;
    void restoreSplitterStates(const QVariantList &states);

  public slots:
    void setVerdict(int index, TestCase::Verdict verdict);

  signals:
    void checkerChanged();
    void requestRun(int index);

  private slots:
    void on_addButton_clicked();
    void on_addCheckerButton_clicked();
    void onChildDeleted(TestCase *widget);

  private:
    bool validateIndex(int index, const QString &funcName) const;
    void updateVerdicts();
    static QString inputFilePath(const QString &filePath, int index);
    static QString answerFilePath(const QString &filePath, int index);
    static QString testCaseFilePath(QString rule, const QString &filePath, int index);

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
};
} // namespace Widgets
#endif // TESTCASES_HPP

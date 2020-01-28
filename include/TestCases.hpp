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

#ifndef TESTCASES_HPP
#define TESTCASES_HPP

#include "MessageLogger.hpp"
#include <QFileInfo>
#include <QHBoxLayout>
#include <QLabel>
#include <QPlainTextEdit>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

class TestCaseEdit : public QPlainTextEdit
{
    Q_OBJECT

  public:
    explicit TestCaseEdit(bool autoAnimation, const QString &text, QWidget *parent = nullptr);
    explicit TestCaseEdit(bool autoAnimation, QWidget *parent = nullptr);
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void modifyText(const QString &text);

  public slots:
    void startAnimation();

  private:
    QPropertyAnimation *animation;
};

class TestCase : public QWidget
{
    Q_OBJECT

  public:
    explicit TestCase(MessageLogger *logger, QWidget *parent = nullptr, const QString &input = QString(),
                      const QString &expected = QString());
    void setInput(const QString &text);
    void setOutput(const QString &text);
    void setExpected(const QString &text);
    void clearOutput();
    QString input() const;
    QString output() const;
    QString expected() const;
    void loadFromFile(const QString &pathPrefix);
    void save(const QString &pathPrefix);

  signals:
    void deleted(TestCase *widget);

  private slots:
    void on_deleteButton_clicked();
    void on_loadInputButton_clicked();
    void on_diffButton_clicked();
    void on_loadExpectedButton_clicked();

  private:
    QHBoxLayout *mainLayout = nullptr, *inputUpLayout = nullptr, *outputUpLayout = nullptr, *expectedUpLayout = nullptr;
    QVBoxLayout *inputLayout = nullptr, *outputLayout = nullptr, *expectedLayout = nullptr;
    QLabel *inputLabel = nullptr, *outputLabel = nullptr, *expectedLabel = nullptr;
    QPushButton *deleteButton = nullptr, *loadInputButton = nullptr, *diffButton = nullptr,
                *loadExpectedButton = nullptr;
    TestCaseEdit *inputEdit = nullptr, *outputEdit = nullptr, *expectedEdit = nullptr;
    MessageLogger *log;

    bool isPass();
    int id();
};

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
    void save(const QString &filePath);
    int id(TestCase *testcase) const;
    int count() const;

  private slots:
    void on_addButton_clicked();
    void onChildDeleted(TestCase *widget);

  private:
    static const int MAX_NUMBER_OF_TESTCASES = 100;
    QVBoxLayout *mainLayout = nullptr, *scrollAreaLayout = nullptr;
    QHBoxLayout *titleLayout = nullptr;
    QPushButton *addButton = nullptr;
    QScrollArea *scrollArea = nullptr;
    QWidget *scrollAreaWidget = nullptr;
    QLabel *label = nullptr;
    QList<TestCase *> testcases;
    MessageLogger *log;

    QString testFilePathPrefix(const QFileInfo &fileInfo, int index);
    int numberOfTestFile(const QString &sourceName, const QFileInfo &fileName);
};

#endif // TESTCASES_HPP

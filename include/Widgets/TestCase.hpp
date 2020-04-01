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

#ifndef TESTCASE_HPP
#define TESTCASE_HPP

#include "Core/Checker.hpp"
#include "Core/MessageLogger.hpp"
#include "Widgets/DiffViewer.hpp"
#include "Widgets/TestCaseEdit.hpp"
#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QPushButton>
#include <QVBoxLayout>

class TestCase : public QWidget
{
    Q_OBJECT

  public:
    explicit TestCase(int index, MessageLogger *logger, QWidget *parent = nullptr, const QString &input = QString(),
                      const QString &expected = QString());
    void setInput(const QString &text);
    void setOutput(const QString &text);
    void setExpected(const QString &text);
    void clearOutput();
    QString input() const;
    QString output() const;
    QString expected() const;
    void loadFromFile(const QString &pathPrefix);
    void save(const QString &pathPrefix, bool safe);
    void setID(int index);
    void setVerdict(Core::Checker::Verdict verdict);
    Core::Checker::Verdict verdict() const;
    void setShow(bool show);
    bool isShow() const;

  signals:
    void deleted(TestCase *widget);
    void requestRun(int index);
    void requestDebug(int index);

  private slots:
    void onShowCheckBoxToggled(bool checked);
    void onRunButtonClicked();
    void onDebugButtonClicked();
    void onDiffButtonClicked();
    void onDelButtonClicked();
    void onToLongForHtml();

  private:
    QHBoxLayout *mainLayout = nullptr, *inputUpLayout = nullptr, *outputUpLayout = nullptr, *expectedUpLayout = nullptr;
    QVBoxLayout *inputLayout = nullptr, *outputLayout = nullptr, *expectedLayout = nullptr;
    QCheckBox *showCheckBox = nullptr;
    QLabel *inputLabel = nullptr, *outputLabel = nullptr, *expectedLabel = nullptr;
    QPushButton *runButton = nullptr, *debugButton = nullptr, *diffButton = nullptr, *delButton = nullptr;
    TestCaseEdit *inputEdit = nullptr, *outputEdit = nullptr, *expectedEdit = nullptr;
    DiffViewer *diffViewer = nullptr;
    MessageLogger *log;
    Core::Checker::Verdict currentVerdict = Core::Checker::UNKNOWN;
    int id;
};

#endif // TESTCASE_HPP

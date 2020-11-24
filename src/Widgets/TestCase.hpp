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

#include <QWidget>

class MessageLogger;
class QCheckBox;
class QHBoxLayout;
class QLabel;
class QMenu;
class QPushButton;
class QSplitter;
class QVBoxLayout;

namespace Widgets
{
class DiffViewer;
class TestCaseEdit;

class TestCase : public QWidget
{
    Q_OBJECT

  public:
    enum Verdict
    {
        AC,  // Accepted
        WA,  // Wrong answer
        TLE, // Time Limit Exceeded
        RE,  // Runtime Error
        UNKNOWN
    };

    explicit TestCase(int index, MessageLogger *logger, QWidget *parent = nullptr, const QString &input = QString(),
                      const QString &expected = QString());
    void setInput(const QString &text);
    void setOutput(const QString &text);
    void setExpected(const QString &text);
    void clearOutput();
    QString input() const;
    QString output() const;
    QString expected() const;
    bool isEmpty() const;
    void setID(int index);
    void setVerdict(Verdict verdict);
    Verdict verdict() const;
    void setChecked(bool checked);
    bool isChecked() const;
    void setTestCaseEditFont(const QFont &font);
    void updateHeight();
    QList<int> splitterSizes() const;
    void restoreSplitterSizes(const QList<int> &sizes);

  signals:
    void deleted(TestCase *widget);
    void requestRun(int index);

  private slots:
    void onCheckBoxToggled(bool checked);
    void onRunButtonClicked();
    void onDiffButtonClicked();
    void onDelButtonClicked();
    void onToLongForHtml();

  private:
    QHBoxLayout *mainLayout = nullptr, *inputUpLayout = nullptr, *outputUpLayout = nullptr, *expectedUpLayout = nullptr;
    QSplitter *splitter = nullptr;
    QWidget *inputWidget = nullptr, *outputWidget = nullptr, *expectedWidget = nullptr;
    QVBoxLayout *inputLayout = nullptr, *outputLayout = nullptr, *expectedLayout = nullptr;
    QCheckBox *checkBox = nullptr;
    QLabel *inputLabel = nullptr, *outputLabel = nullptr, *expectedLabel = nullptr;
    QPushButton *runButton = nullptr, *diffButton = nullptr, *delButton = nullptr;
    TestCaseEdit *inputEdit = nullptr, *outputEdit = nullptr, *expectedEdit = nullptr;
    DiffViewer *diffViewer = nullptr;
    MessageLogger *log;
    Verdict currentVerdict = UNKNOWN;
    int id;
};
} // namespace Widgets
#endif // TESTCASE_HPP

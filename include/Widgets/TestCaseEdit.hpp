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

#ifndef TESTCASEEDIT_HPP
#define TESTCASEEDIT_HPP

#include <QPlainTextEdit>
#include <QPropertyAnimation>

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

#endif // TESTCASEEDIT_HPP

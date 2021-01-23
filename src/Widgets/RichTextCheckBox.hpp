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

// https://www.qtcentre.org/threads/60455-SOLVED-Issues-with-displaying-rich-text-for-a-QCheckBox?p=268211#post268211

#ifndef RICHTEXTCHECKBOX_HPP
#define RICHTEXTCHECKBOX_HPP

#include <QCheckBox>
#include <QLabel>

class ClickableLabel : public QLabel
{
    Q_OBJECT

  public:
    explicit ClickableLabel(QWidget *parent = nullptr);
    void mouseReleaseEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

  signals:
    void released();
    void pressed();
    void left();

  private:
    std::atomic_bool linkClicked;
};

class HoverCheckBox : public QCheckBox
{
    Q_OBJECT

  public:
    explicit HoverCheckBox(QWidget *parent = nullptr);

  private:
    virtual void paintEvent(QPaintEvent *);
    bool isHover;
    bool isPressed;

  public slots:
    void setHover();
    void setPressed();
    void clearPressed();
    void clearStates();
};

class RichTextCheckBox : public QWidget
{
    Q_OBJECT

  public:
    explicit RichTextCheckBox(QString text, QWidget *parent = nullptr);
    QCheckBox *getCheckBox();
    QString text() const;
    QString plainText() const;
    Qt::CheckState checkState() const;
    bool isChecked() const;
    void clearStates();

  private:
    HoverCheckBox *checkBox;
    ClickableLabel *label;
    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);

  signals:
    void toggled(bool);
    void clicked();

  public slots:
    void setChecked(bool);
    void setCheckState(Qt::CheckState state);
};

#endif // RICHTEXTCHECKBOX_HPP

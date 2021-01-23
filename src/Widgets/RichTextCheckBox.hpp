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
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

  signals:
    void released();
    void pressed();
    void left();

  private:
    std::atomic_bool linkClicked{false};
};

class HoverCheckBox : public QCheckBox
{
    Q_OBJECT

  public:
    explicit HoverCheckBox(QWidget *parent = nullptr);

  public slots:
    void setHover();
    void setPressed();
    void clearPressed();
    void clearStates();

  private:
    void paintEvent(QPaintEvent * /*unused*/) override;

    std::atomic_bool isHover{false};
    std::atomic_bool isPressed{false};
};

class RichTextCheckBox : public QWidget
{
    Q_OBJECT

  public:
    explicit RichTextCheckBox(const QString &text, QWidget *parent = nullptr);
    QCheckBox *getCheckBox();
    QString text() const;
    QString plainText() const;
    Qt::CheckState checkState() const;
    bool isChecked() const;
    void clearStates();

  signals:
    void toggled(bool);
    void clicked();

  public slots:
    void setChecked(bool);
    void setCheckState(Qt::CheckState state);

  private:
    void enterEvent(QEvent * /*unused*/) override;
    void leaveEvent(QEvent * /*unused*/) override;

    HoverCheckBox *checkBox = nullptr;
    ClickableLabel *label = nullptr;
};

#endif // RICHTEXTCHECKBOX_HPP

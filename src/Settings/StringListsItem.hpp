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

#ifndef STRINGLISTSITEM_HPP
#define STRINGLISTSITEM_HPP

#include <QWidget>

class QHBoxLayout;
class QPushButton;
class QTableWidget;
class QVBoxLayout;

class StringListsItem : public QWidget
{
    Q_OBJECT

  public:
    explicit StringListsItem(const QVariantList &cols, QWidget *parent = nullptr);

    void setStringLists(const QVariantList &val);
    QVariantList getStringLists();

  signals:
    void valueChanged();

  private:
    QHBoxLayout *layout = nullptr;
    QVBoxLayout *buttonLayout = nullptr;
    QTableWidget *table = nullptr;
    QPushButton *add = nullptr;
    QPushButton *del = nullptr;
    QPushButton *moveUp = nullptr;
    QPushButton *moveDown = nullptr;
};

#endif // STRINGLISTSITEM_HPP

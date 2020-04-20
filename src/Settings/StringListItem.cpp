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

#include "Settings/StringListItem.hpp"
#include <QDebug>
#include <QHeaderView>

StringListItem::StringListItem(int column, QWidget *parent) : QWidget(parent)
{
    layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    table = new QTableWidget(this);
    table->setColumnCount(column);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::DoubleClicked);
    table->horizontalHeader()->setVisible(false);
    connect(table, &QTableWidget::cellChanged, this, &StringListItem::stringsChanged);
    layout->addWidget(table);

    buttonLayout = new QVBoxLayout(this);

    add = new QToolButton(this);
    add->setText("＋");
    connect(add, &QToolButton::clicked, [&]() {
        int cols = table->columnCount();
        int rows = table->rowCount();
        auto items = table->selectedItems();
        int insRow = rows;
        if (items.size())
            insRow = items.front()->row();
        table->insertRow(insRow);
        for (int i = 0; i < cols; ++i)
            table->setItem(insRow, i, new QTableWidgetItem(""));
    });
    buttonLayout->addWidget(add);

    del = new QToolButton(this);
    del->setText("－");
    connect(del, &QToolButton::clicked, [&]() {
        auto items = table->selectedItems();
        if (items.size())
            table->removeRow(items.front()->row());
    });
    buttonLayout->addWidget(del);

    moveUp = new QToolButton(this);
    moveUp->setText("∧");
    connect(moveUp, &QToolButton::clicked, [&]() {
        int cols = table->columnCount();
        auto items = table->selectedItems();
        if (items.size())
        {
            table->setItemSelected(items.front(), false);
            int row = items.front()->row();
            if (row > 0)
                for (int i = 0; i < cols; ++i)
                {
                    auto temp = table->takeItem(row - 1, i);
                    table->setItem(row - 1, i, table->takeItem(row, i));
                    table->setItem(row, i, temp);
                }
            table->setItemSelected(items.front(), true);
        }
    });
    buttonLayout->addWidget(moveUp);

    moveDown = new QToolButton(this);
    moveDown->setText("∨");
    connect(moveDown, &QToolButton::clicked, [&]() {
        int cols = table->columnCount();
        int rows = table->rowCount();
        auto items = table->selectedItems();
        if (items.size())
        {
            table->setItemSelected(items.front(), false);
            int row = items.front()->row();
            if (row + 1 < rows)
                for (int i = 0; i < cols; ++i)
                {
                    auto temp = table->takeItem(row + 1, i);
                    table->setItem(row + 1, i, table->takeItem(row, i));
                    table->setItem(row, i, temp);
                }
            table->setItemSelected(items.front(), true);
        }
    });
    buttonLayout->addWidget(moveDown);

    layout->addLayout(buttonLayout);
}

void StringListItem::setStrings(const QStringList &sl)
{
    int cols = table->columnCount();
    int rows = sl.size() / cols;
    auto it = sl.begin();
    table->clear();
    table->setRowCount(rows);
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            table->setItem(i, j, new QTableWidgetItem(*it++));
}

QStringList StringListItem::getStrings()
{
    QStringList data;
    int cols = table->columnCount();
    int rows = table->rowCount();
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            if (table->item(i, j))
                data.push_back(table->item(i, j)->text());
    return data;
}
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

#include "Settings/StringListsItem.hpp"
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QVariant>

StringListsItem::StringListsItem(const QVariantList &cols, QWidget *parent) : QWidget(parent)
{
    layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    table = new QTableWidget(this);
    table->setMinimumWidth(480);
    table->setColumnCount(cols.count());
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked |
                           QAbstractItemView::AnyKeyPressed);
    for (int i = 0; i < cols.count(); ++i)
    {
        auto *item = new QTableWidgetItem(cols[i].toStringList().front());
        item->setToolTip(cols[i].toStringList().back());
        table->setHorizontalHeaderItem(i, item);
    }
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(table, &QTableWidget::cellChanged, this, &StringListsItem::valueChanged);
    layout->addWidget(table);

    buttonLayout = new QVBoxLayout();

    add = new QPushButton(this);
    add->setText(tr("Add"));
    add->setShortcut({"Ctrl+N"});
    add->setToolTip(tr("Insert a row (Ctrl+N)"));
    connect(add, &QPushButton::clicked, [&]() {
        int cols = table->columnCount();
        int rows = table->rowCount();
        auto items = table->selectedItems();
        int insRow = rows;
        if (!items.empty())
            insRow = items.front()->row() + 1;
        table->insertRow(insRow);
        for (int i = 0; i < cols; ++i)
            table->setItem(insRow, i, new QTableWidgetItem(""));
    });
    buttonLayout->addWidget(add);

    del = new QPushButton(this);
    del->setText(tr("Remove"));
    del->setShortcut({"Ctrl+W"});
    del->setToolTip(tr("Delete the current row (Ctrl+W)"));
    connect(del, &QPushButton::clicked, [&]() {
        auto items = table->selectedItems();
        if (!items.empty())
        {
            auto res =
                QMessageBox::question(this, tr("Remove Item"), tr("Do you really want to delete the current row?"));
            if (res == QMessageBox::Yes)
            {
                table->removeRow(items.front()->row());
                emit valueChanged();
            }
        }
    });
    buttonLayout->addWidget(del);

    moveUp = new QPushButton(this);
    moveUp->setText(tr("Move Up"));
    moveUp->setShortcut({"Ctrl+Shift+Up"});
    moveUp->setToolTip(tr("Move the current row up (Ctrl+Shift+Up)"));
    connect(moveUp, &QPushButton::clicked, [&]() {
        int cols = table->columnCount();
        auto items = table->selectedItems();
        if (!items.empty())
        {
            items.front()->setSelected(false);
            int row = items.front()->row();
            if (row > 0)
                for (int i = 0; i < cols; ++i)
                {
                    auto *temp = table->takeItem(row - 1, i);
                    table->setItem(row - 1, i, table->takeItem(row, i));
                    table->setItem(row, i, temp);
                }
            items.front()->setSelected(true);
        }
    });
    buttonLayout->addWidget(moveUp);

    moveDown = new QPushButton(this);
    moveDown->setText(tr("Move Down"));
    moveDown->setShortcut({"Ctrl+Shift+Down"});
    moveDown->setToolTip(tr("Move the current row down (Ctrl+Shift+Down)"));
    connect(moveDown, &QPushButton::clicked, [&]() {
        int cols = table->columnCount();
        int rows = table->rowCount();
        auto items = table->selectedItems();
        if (!items.empty())
        {
            items.front()->setSelected(false);
            int row = items.front()->row();
            if (row + 1 < rows)
                for (int i = 0; i < cols; ++i)
                {
                    auto *temp = table->takeItem(row + 1, i);
                    table->setItem(row + 1, i, table->takeItem(row, i));
                    table->setItem(row, i, temp);
                }
            items.front()->setSelected(true);
        }
    });
    buttonLayout->addWidget(moveDown);

    layout->addLayout(buttonLayout);
}

void StringListsItem::setStringLists(const QVariantList &val)
{
    int cols = table->columnCount();
    int rows = val.size();
    table->setRowCount(rows);
    for (int i = 0; i < rows; ++i)
    {
        auto list = val[i].toStringList();
        for (int j = 0; j < cols && j < list.length(); ++j)
            table->setItem(i, j, new QTableWidgetItem(list[j]));
    }
}

QVariantList StringListsItem::getStringLists()
{
    QVariantList data;
    int cols = table->columnCount();
    int rows = table->rowCount();
    for (int i = 0; i < rows; ++i)
    {
        QStringList list;
        for (int j = 0; j < cols; ++j)
        {
            if (table->item(i, j))
                list.push_back(table->item(i, j)->text());
            else
                list.push_back(QString());
        }
        data.push_back(list);
    }
    return data;
}

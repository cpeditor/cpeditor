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

#include "Preferences/Language/CodeTemplateItem.hpp"
#include <QApplication>
#include <QFileDialog>
#include <QStyle>

CodeTemplateItem::CodeTemplateItem(const QString &pathFilter, QWidget *parent) : QWidget(parent), filter(pathFilter)
{
    layout = new QHBoxLayout(this);

    lineEdit = new QLineEdit();
    lineEdit->setMinimumWidth(400);
    layout->addWidget(lineEdit);

    toolButton = new QToolButton();
    toolButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogOpenButton));
    connect(toolButton, SIGNAL(clicked()), this, SLOT(onButtonClicked()));
    layout->addWidget(toolButton);
}

QLineEdit *CodeTemplateItem::getLineEdit()
{
    return lineEdit;
}

void CodeTemplateItem::onButtonClicked()
{
    auto path = QFileDialog::getOpenFileName(this, "Choose Code Template", lineEdit->text(), filter);
    if (!path.isEmpty())
    {
        lineEdit->selectAll();
        lineEdit->insert(path);
    }
}

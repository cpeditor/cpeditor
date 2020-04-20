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

#include "Settings/PathItem.hpp"
#include "Util.hpp"
#include <QApplication>
#include <QFileDialog>
#include <QStyle>

static QString filters[] = {"Excutable (*" EXE_SUFFIX ")", "C++ Sources (*.cpp *.hpp *.h *.cc *.cxx *.c)",
                            "Java Sources (*.java)", "Python Sources (*.py *.py3)"};

static QString titles[] = {
    "Choose Excutable",
    "Choose C++ Sources",
    "Choose Java Sources",
    "Choose Python Sources",
};

PathItem::PathItem(const QString &pathFilter, const QString &dialogTitle, QWidget *parent)
    : QWidget(parent), filter(pathFilter), title(dialogTitle)
{
    layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    lineEdit = new QLineEdit();
    lineEdit->setMinimumWidth(400);
    layout->addWidget(lineEdit);

    toolButton = new QToolButton();
    toolButton->setToolTip("Choose a file");
    toolButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogOpenButton));
    connect(toolButton, SIGNAL(clicked()), this, SLOT(onButtonClicked()));
    layout->addWidget(toolButton);
}

PathItem::PathItem(int filterId, int titleId, QWidget *parent) : PathItem(filters[filterId], titles[titleId], parent)
{
}

QLineEdit *PathItem::getLineEdit()
{
    return lineEdit;
}

void PathItem::onButtonClicked()
{
    auto path = QFileDialog::getOpenFileName(this, title, lineEdit->text(), filter);
    if (!path.isEmpty())
        lineEdit->setText(path);
}

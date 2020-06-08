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
#include "Util/FileUtil.hpp"
#include <QApplication>
#include <QCoreApplication>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QStyle>
#include <QToolButton>

static const QString titles[] = {
    "Choose Excutable",
    "Choose C++ Sources",
    "Choose Java Sources",
    "Choose Python Sources",
};

inline QString getFilters(int index)
{
    return QStringList{QCoreApplication::translate("Settings::PathItem", "Excutable") + " (*" + Util::exeSuffix + ")",
                       QCoreApplication::translate("Settings::PathItem", "C++ Sources") +
                           " (*.cpp *.hpp *.h *.cc *.cxx *.c)",
                       QCoreApplication::translate("Settings::PathItem", "Java Sources") + " (*.java)",
                       QCoreApplication::translate("Settings::PathItem", "Python Sources") + " (*.py *.py3)"}[index];
}

inline QString getTitles(int index)
{
    return QStringList{
        QCoreApplication::translate("Settings::PathItem", "Choose Excutable"),
        QCoreApplication::translate("Settings::PathItem", "Choose C++ Sources"),
        QCoreApplication::translate("Settings::PathItem", "Choose Java Sources"),
        QCoreApplication::translate("Settings::PathItem", "Choose Python Sources"),
    }[index];
}

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

PathItem::PathItem(int filterId, int titleId, QWidget *parent)
    : PathItem(getFilters(filterId), getTitles(titleId), parent)
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

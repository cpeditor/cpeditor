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

#include "Settings/PathItem.hpp"
#include "Core/EventLogger.hpp"
#include "Util/FileUtil.hpp"
#include <QApplication>
#include <QCoreApplication>
#include <QFileDialog>
#include <QFocusEvent>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QStyle>
#include <QToolButton>

PathItem::PathItem(Type type, QWidget *parent) : QWidget(parent), fileType(type)
{
    layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    lineEdit = new QLineEdit();
    lineEdit->setMinimumWidth(400);
    connect(lineEdit, &QLineEdit::textChanged, this, &PathItem::pathChanged);
    layout->addWidget(lineEdit);

    toolButton = new QToolButton();
    toolButton->setToolTip(tr("Choose a file"));
    toolButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogOpenButton));
    connect(toolButton, &QToolButton::clicked, this, &PathItem::onButtonClicked);
    layout->addWidget(toolButton);
}

QLineEdit *PathItem::getLineEdit()
{
    return lineEdit;
}

void PathItem::onButtonClicked()
{
    auto path = QFileDialog::getOpenFileName(this, title(), lineEdit->text(), filter());
    if (!path.isEmpty())
        lineEdit->setText(path);
    emit pathChanged();
}

QString PathItem::filter() const
{
    switch (fileType)
    {
    case AnyFile:
        return QString();
    case CppSource:
    case JavaSource:
    case PythonSource:
        return Util::fileNameFilter(fileType == CppSource, fileType == JavaSource, fileType == PythonSource);
    case Executable:
        return tr("Excutable Files") + " (*" + Util::exeSuffix + ")";
    default:
        Q_UNREACHABLE();
    }
}

QString PathItem::title() const
{
    switch (fileType)
    {
    case AnyFile:
        return tr("Choose a file");
    case CppSource:
        return tr("Choose a %1 source file").arg("C++");
    case JavaSource:
        return tr("Choose a %1 source file").arg("Java");
    case PythonSource:
        return tr("Choose a %1 source file").arg("Python");
    case Executable:
        return tr("Choose an excutable file");
    default:
        Q_UNREACHABLE();
    }
}

void PathItem::focusInEvent(QFocusEvent *event)
{
    lineEdit->setFocus(event->reason());
}

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

#include "Settings/ShortcutItem.hpp"
#include <QApplication>
#include <QStyle>

ShortcutItem::ShortcutItem(QWidget *parent) : QWidget(parent)
{
    layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    edit = new QKeySequenceEdit(this);
    connect(edit, SIGNAL(keySequenceChanged(const QKeySequence &)), this, SIGNAL(shortcutChanged()));
    layout->addWidget(edit);

    clear = new QToolButton(this);
    clear->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogResetButton));
    connect(clear, &QToolButton::clicked, edit, &QKeySequenceEdit::clear);
    layout->addWidget(clear);
}

void ShortcutItem::setShortcut(const QString &s)
{
    edit->setKeySequence(QKeySequence::fromString(s));
}

QString ShortcutItem::getShortcut()
{
    return edit->keySequence().toString();
}
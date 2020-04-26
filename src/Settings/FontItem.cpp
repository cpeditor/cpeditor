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

#include "Settings/FontItem.hpp"
#include <QFontDialog>

FontItem::FontItem(QWidget *parent) : QPushButton(parent)
{
    Q_ASSERT(parent != nullptr);
    connect(this, SIGNAL(clicked(bool)), this, SLOT(onButtonClicked()));
}

void FontItem::setFont(const QString &desc)
{
    QFont newFont;
    if (newFont.fromString(desc))
    {
        setFont(newFont);
    }
}

void FontItem::setFont(QFont newFont)
{
    if (newFont != font)
    {
        font = newFont;
        setText(QString("%1 %2").arg(font.family()).arg(font.pointSize()));
        emit fontChanged(font);
    }
}

QFont FontItem::getFont()
{
    return font;
}

void FontItem::onButtonClicked()
{
    bool ok;
    QFont newFont = QFontDialog::getFont(&ok, font, this, QString(), QFontDialog::MonospacedFonts);
    if (ok)
        setFont(newFont);
    parentWidget()->raise();
}

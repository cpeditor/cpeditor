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
#include "generated/SettingsHelper.hpp"
#include <QFontDialog>
#include <QVariant>

FontItem::FontItem(QWidget *parent, const QVariant &param) : QPushButton(parent)
{
    Q_ASSERT(parent != nullptr);
    connect(this, &FontItem::clicked, this, &FontItem::onButtonClicked);
}

void FontItem::setFont(QFont const &newFont)
{
    if (newFont != font)
    {
        font = newFont;
        emit fontChanged(font);
    }

    // out of the if statement, because it's useful if this function is called for the first time and the newFont is the
    // default font
    setText(QString("%1 %2").arg(font.family()).arg(font.pointSize()));
}

QFont FontItem::getFont()
{
    return font;
}

void FontItem::onButtonClicked()
{
    bool ok = false;
    QFont newFont = QFontDialog::getFont(&ok, font, this, QString(),
                                         SettingsHelper::isShowOnlyMonospacedFont() ? QFontDialog::MonospacedFonts
                                                                                    : QFontDialog::FontDialogOption());
    if (ok)
        setFont(newFont);
    parentWidget()->raise();
}

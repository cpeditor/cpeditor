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

#include "Util/Util.hpp"
#include <QTextCodec>
#include <QWidget>

namespace Util
{

void showWidgetOnTop(QWidget *widget)
{
    widget->show();
    widget->setWindowState((widget->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    widget->activateWindow();
    widget->raise();
}

QString guessCodec(const QByteArray &data, QTextCodec *&codec)
{
    QList<QByteArray> scanList{"UTF-8", "GB2312", "KOI8-RU"};
    for (const QByteArray &name : scanList)
    {
        QTextCodec *c = QTextCodec::codecForName(name);
        QTextCodec::ConverterState state;
        QString res = c->toUnicode(data.data(), data.size(), &state);
        if (!state.invalidChars)
        {
            codec = c;
            return res;
        }
    }
    codec = QTextCodec::codecForName("UTF-8");
    return codec->toUnicode(data);
}

} // namespace Util

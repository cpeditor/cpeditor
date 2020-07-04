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
#include "Core/EventLogger.hpp"
#include <QTextCodec>
#include <QWidget>

namespace Util
{

static QMap<QString, QByteArray> codecs = {{"zh", "GB18030"}, {"ru", "KOI8-RU"}};

void showWidgetOnTop(QWidget *widget)
{
    widget->show();
    widget->setWindowState((widget->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    widget->activateWindow();
    widget->raise();
}

QString guessCodec(const QByteArray &data, QTextCodec *&codec)
{
    QString name = QLocale::system().name();
    if (name.indexOf("_") != -1)
        name = name.left(name.indexOf("_"));
    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");
    QTextCodec *sysc = nullptr;
    if (codecs.contains(name))
    {
        sysc = QTextCodec::codecForName(codecs[name]);
    }
    if (sysc)
    {
        QTextCodec::ConverterState state1, state2;
        QString sys = sysc->toUnicode(data.data(), data.size(), &state1);
        QString utf = utf8->toUnicode(data.data(), data.size(), &state2);
        if (state2.invalidChars <= state1.invalidChars)
        {
            codec = utf8;
            return utf;
        }
        else
        {
            codec = sysc;
            return sys;
        }
    }
    else
    {
        codec = utf8;
        return QString::fromUtf8(data);
    }
}

} // namespace Util

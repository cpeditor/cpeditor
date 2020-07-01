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
#include <unicode/ucsdet.h>
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
    UErrorCode status;
    UCharsetDetector *detector = ucsdet_open(&status);
    ucsdet_setText(detector, data.data(), data.size(), &status);
    int count;
    const UCharsetMatch **matchs = ucsdet_detectAll(detector, &count, &status);
    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");
    QTextCodec *guess = utf8;
    QByteArray name;
    LOG_INFO("Find " << count << " matches");
    if (count > 0)
    {
        int cfd = ucsdet_getConfidence(matchs[0], &status);
        LOG_INFO("Highest confidence is " << cfd);
        if (cfd == 10) // means the text don't match any. treat them as utf-8
        {
            name = "UTF-8";
            LOG_WARN("Fallback to UTF-8");
        }
        else
        {
            name = ucsdet_getName(matchs[0], &status);
            LOG_INFO("Pick " << name);
        }
        guess = QTextCodec::codecForName(name.toUpper());
        if (!guess)
        {
            LOG_WARN("Cannot find codec for " << name << ", Use UTF-8 instead.");
            guess = utf8;
        }
    }
    ucsdet_close(detector);
    codec = guess;
    return codec->toUnicode(data);
}

} // namespace Util

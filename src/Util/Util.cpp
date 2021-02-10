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

#include "Util/Util.hpp"
#include "Core/EventLogger.hpp"
#include "Core/Translator.hpp"
#include "generated/version.hpp"
#include <QRegularExpression>
#include <QUrl>
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

QString sanitizeAnchorName(const QString &str)
{
    return str.trimmed()
        .remove(QRegularExpression(R"([!"\#$%&'()*+,\-./:;<=>?@\[\\\]^_‘{|}~]|，|（|）|。)"))
        .toLower()
        .replace(' ', '-');
}

QString websiteLink(const QString &path)
{
    return QUrl(QString("https://cpeditor.org/%1/%2/%3").arg(MINOR_VERSION).arg(Core::Translator::langCode()).arg(path))
        .url(QUrl::NormalizePathSegments);
}


bool parseCfUrl(const QString &url, QString &contestId, QString &problemCode)
{
    LOG_INFO(INFO_OF(url));
    auto match =
        QRegularExpression(".*://codeforces.com/(?:gym|contest)/([1-9][0-9]*)/problem/(0|[A-Z][1-9]?)").match(url);
    if (match.hasMatch())
    {
        contestId = match.captured(1);
        problemCode = match.captured(2);
        return true;
    }
    match = QRegularExpression(".*://codeforces.com/problemset/problem/([1-9][0-9]*)/([A-Z][1-9]?)").match(url);
    if (match.hasMatch())
    {
        contestId = match.captured(1);
        problemCode = match.captured(2);
        return true;
    }
    return false;
}

} // namespace Util

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

#include "Core/MessageLogger.hpp"
#include "Core/EventLogger.hpp"
#include "Settings/PreferencesWindow.hpp"
#include "generated/SettingsHelper.hpp"
#include <QDateTime>

MessageLogger::MessageLogger(PreferencesWindow *preferences, QWidget *parent)
    : QTextBrowser(parent), preferencesWindow(preferences)
{
    connect(this, &MessageLogger::anchorClicked, this, &MessageLogger::onAnchorClicked);
    setOpenExternalLinks(true);
}

void MessageLogger::message(const QString &head, const QString &body, const QString &color, bool htmlEscaped)
{
    LOG_WARN_IF(body.contains("<a href") && htmlEscaped,
                "The message contains \"<a href\", but htmlEscaped is enabled.");

    QString newHead;
    QString newBody;
    if (htmlEscaped)
    {
        // replace spaces by "&nbsp;" to avoid multiple spaces becoming one, important for compilation errors
        newHead = head.toHtmlEscaped().replace(" ", "&nbsp;");
        newBody = body.toHtmlEscaped().replace(" ", "&nbsp;");
    }
    else
    {
        newHead = head;
        newBody = body;
    }

    // don't display too long messages, otherwise the application may stuck
    if (newBody.length() > SettingsHelper::getMessageLengthLimit())
        newBody = newBody.left(SettingsHelper::getMessageLengthLimit()) + tr("\n... The message is too long");

    // get the HTML of the message
    // use monospace for the message body, it's important for compilation errors
    // "monospace" might not work on Windows, but "Consolas,Courier,monospace" works
    QString res = QString("<b>[%1] [%2] </b><span style=\"").arg(QTime::currentTime().toString(), newHead);
    if (!color.isEmpty())
        res += "color:" + color;
    res += "\">[";
    if (newBody.contains('\n'))
        res += "<br>" + newBody.replace("\n", "<br>");
    else
        res += newBody;
    res += "]</span>";

    append(res);
}

void MessageLogger::info(const QString &head, const QString &body, bool htmlEscaped)
{
    LOG_INFO(INFO_OF(head) << INFO_OF(body));
    message(head, body, "", htmlEscaped);
}

void MessageLogger::warn(const QString &head, const QString &body, bool htmlEscaped)
{
    LOG_INFO(INFO_OF(head) << INFO_OF(body));
    message(head, body, "green", htmlEscaped);
}

void MessageLogger::error(const QString &head, const QString &body, bool htmlEscaped)
{
    LOG_INFO(INFO_OF(head) << INFO_OF(body));
    message(head, body, "red", htmlEscaped);
}

void MessageLogger::onAnchorClicked(const QUrl &link)
{
    auto url = link.toString();
    LOG_INFO(INFO_OF(url));
    if (url.startsWith("#Preferences/"))
        preferencesWindow->open(url.mid(13));
}

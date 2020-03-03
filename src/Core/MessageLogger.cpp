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

#include "Core/MessageLogger.hpp"
#include "Core/EventLogger.hpp"
#include <QDateTime>

void MessageLogger::setContainer(QTextBrowser *value)
{
    MessageLogger::box = value;
    Core::Log::i("messagelogger/setContainer", "Container set and updated to open links");
    box->setOpenExternalLinks(true);
}

void MessageLogger::message(const QString &head, const QString &body, const QString &color)
{
    auto newHead = head.toHtmlEscaped().replace(" ", "&nbsp;");
    auto newBody = body.toHtmlEscaped().replace(" ", "&nbsp;");
    if (newBody.length() > 10000)
        newBody = newBody.left(10000) + "\n... The message is too long";
    QString res = "<b>[" + QTime::currentTime().toString() + "] [" + newHead +
                  "] </b><span style=\"font-family:Consolas,Courier,monospace;";
    if (!color.isEmpty())
        res += "color:" + color;
    res += "\">[";
    if (newBody.contains('\n'))
        res += "<br>" + newBody.replace("\n", "<br>");
    else
        res += newBody;
    res += "]</font>";
    box->append(res);
}

void MessageLogger::info(const QString &head, const QString &body)
{
    Core::Log::i("messagelogger/message/" + head, body);
    message(head, body, "");
}

void MessageLogger::warn(const QString &head, const QString &body)
{
    Core::Log::w("messagelogger/message/" + head, body);
    message(head, body, "green");
}

void MessageLogger::error(const QString &head, const QString &body)
{
    Core::Log::e("messagelogger/message/" + head, body);
    message(head, body, "red");
}

void MessageLogger::clear()
{
    Core::Log::i("messagelogger/clear", "Cleared the message box");
    box->clear();
}

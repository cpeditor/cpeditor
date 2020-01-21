/*
 * Copyright (C) 2019-2020 Ashar Khan <ashar786khan@gmail.com>
 *
 * This file is part of CPEditor.
 *
 * CPEditor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * I will not be responsible if CPEditor behaves in unexpected way and
 * causes your ratings to go down and or loose any important contest.
 *
 * Believe Software is "Software" and it isn't immune to bugs.
 *
 */

#include <MessageLogger.hpp>
#include <QDateTime>

void MessageLogger::setContainer(QTextBrowser *value)
{
    MessageLogger::box = value;
    box->setOpenExternalLinks(true);
}

void MessageLogger::message(const QString &head, const QString &body, const QString &color)
{
    auto newHead = head.toHtmlEscaped().replace(" ", "&nbsp;");
    auto newBody = body.toHtmlEscaped().replace(" ", "&nbsp;");
    QString res = "<b>[" + QTime::currentTime().toString() + "] [" + newHead + "] </b><font face=monospace";
    if (!color.isEmpty())
        res += " color=" + color;
    res += ">[";
    if (newBody.contains('\n'))
        res += "<br>" + newBody.replace("\n", "<br>");
    else
        res += newBody;
    res += "]</font>";
    box->append(res);
}

void MessageLogger::info(const QString &head, const QString &body)
{
    message(head, body, "");
}

void MessageLogger::warn(const QString &head, const QString &body)
{
    message(head, body, "green");
}

void MessageLogger::error(const QString &head, const QString &body)
{
    message(head, body, "red");
}

void MessageLogger::clear()
{
    box->clear();
}

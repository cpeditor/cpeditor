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

void MessageLogger::info(std::string head, std::string body)
{
    auto qHead = QString::fromStdString(head).toHtmlEscaped();
    auto qBody = QString::fromStdString(body).toHtmlEscaped();
    std::string ans = "<b>[";
    int long long timestamp = QDateTime::currentSecsSinceEpoch();
    auto val = QDateTime::fromSecsSinceEpoch(timestamp).time();
    ans += val.toString().toStdString();
    ans += "] [";
    ans += qHead.toStdString();
    ans += "] </b>";
    ans += "[";
    ans += qBody.toStdString();
    ans += "]";
    box->append(QString::fromStdString(ans));
}

void MessageLogger::warn(std::string head, std::string body, bool multiline)
{
    auto qHead = QString::fromStdString(head).toHtmlEscaped();
    auto qBody = QString::fromStdString(body).toHtmlEscaped();
    std::string ans = "<b>[";
    auto timestamp = QDateTime::currentSecsSinceEpoch();
    auto val = QDateTime::fromSecsSinceEpoch(timestamp).time();
    ans += val.toString().toStdString();
    ans += "] [";
    ans += qHead.toStdString();
    ans += "] </b>";
    ans += "<font color=green>[";
    if (multiline)
    {
        ans += "<br>";
        ans += qBody.replace("\n", "<br>").toStdString();
    }
    else
        ans += qBody.toStdString();
    ans += "]</font>";

    box->append(QString::fromStdString(ans));
}
void MessageLogger::error(std::string head, std::string body, bool multiline)
{
    auto qHead = QString::fromStdString(head).toHtmlEscaped();
    auto qBody = QString::fromStdString(body).toHtmlEscaped();
    std::string ans = "<b>[";
    auto timestamp = QDateTime::currentSecsSinceEpoch();
    auto val = QDateTime::fromSecsSinceEpoch(timestamp).time();
    ans += val.toString().toStdString();
    ans += "] [";
    ans += qHead.toStdString();
    ans += "] </b>";
    ans += "<font color=red>[";
    if (multiline)
    {
        ans += "<br>";
        ans += qBody.replace("\n", "<br>").toStdString();
    }
    else
        ans += qBody.toStdString();
    ans += "]</font>";

    box->append(QString::fromStdString(ans));
}
void MessageLogger::clear()
{
    box->clear();
}

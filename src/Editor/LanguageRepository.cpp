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

#include "Editor/LanguageRepository.hpp"
#include "Util/FileUtil.hpp"
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QString>

namespace Editor
{
LanguageRepository::LanguageRepository(const QString &language, QObject *parent) : QObject(parent)
{
    QString jsonDoc = Util::readFile(":/language_config.json");
    doc = QJsonDocument::fromJson(jsonDoc.toLocal8Bit());
    QJsonObject obj = doc[language].toObject();
}

void LanguageRepository::changeLanguage(const QString &newLang)
{
    obj = doc[newLang].toObject();
}

QString LanguageRepository::singleLineCommentToken() const
{
    return obj["singleLineCommentToken"].toString();
}

QPair<QString, QString> LanguageRepository::blockCommentTokens() const
{
    QPair<QString, QString> res;
    res.first = obj["blockCommentTokens"].toArray()[0].toString();
    res.second = obj["blockCommentTokens"].toArray()[1].toString();
    return res;
}

QPair<QString, QString> LanguageRepository::rawStringTokens() const
{
    QPair<QString, QString> res;
    res.first = obj["rawStringTokens"].toArray()[0].toString();
    res.second = obj["rawStringTokens"].toArray()[1].toString();
    return res;
}

QRegExp LanguageRepository::singleLineCommentRegEx() const
{
    auto token = singleLineCommentToken();
    return QRegExp(token + ".*");
}

QRegExp LanguageRepository::blockCommentRegEx() const
{
    auto tokens = blockCommentTokens();
    return QRegExp(tokens.first + R"((\n|.)*?)" + tokens.second);
}

QRegExp LanguageRepository::rawStringRegEx() const
{
    auto tokens = rawStringTokens();
    return QRegExp(tokens.first + R"((\n|.)*?)" + tokens.second);
}
} // namespace Editor

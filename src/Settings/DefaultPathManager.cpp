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

#include "Settings/DefaultPathManager.hpp"
#include "Core/EventLogger.hpp"
#include "Settings/SettingsManager.hpp"
#include "generated/SettingsHelper.hpp"
#include <QFileInfo>
#include <QRegularExpression>

QMap<QString, QString> DefaultPathManager::defaultPath;

QString DefaultPathManager::defaultPathForAction(const QString &action)
{
    const auto result =
        convertPath(SettingsManager::get(QString("Default Path/Action/%1/Uses").arg(action)).toString());
    LOG_INFO(INFO_OF(action) << INFO_OF(result));
    return result;
}

void DefaultPathManager::setDefaultPathForAction(const QString &action, const QString &path)
{
    LOG_INFO(INFO_OF(action) << INFO_OF(path));

    const auto settingsKey = QString("Default Path/Action/%1/Changes").arg(action);

    if (!SettingsManager::contains(settingsKey, true))
    {
        LOG_DEV("Unknown Action: " << action);
        return;
    }

    const QFileInfo info(path);
    const auto dir = info.isFile() ? info.canonicalPath() : info.canonicalFilePath();

    const auto list = SettingsManager::get(settingsKey).toString().split(",");

    for (const auto &key : list)
    {
        const auto trimmedKey = key.trimmed();
        if (trimmedKey.isEmpty())
            continue;
        defaultPath[trimmedKey] = dir;
    }

    SettingsHelper::setDefaultPathNamesAndPaths(toVariantList());
}

QString DefaultPathManager::getExistingDirectory(const QString &action, QWidget *parent, const QString &caption,
                                                 QFileDialog::Options options)
{
    const auto result = QFileDialog::getExistingDirectory(parent, caption, defaultPathForAction(action), options);
    if (!result.isEmpty())
        setDefaultPathForAction(action, result);
    return result;
}

QString DefaultPathManager::getOpenFileName(const QString &action, QWidget *parent, const QString &caption,
                                            const QString &filter, QString *selectedFilter,
                                            QFileDialog::Options options)
{
    const auto result =
        QFileDialog::getOpenFileName(parent, caption, defaultPathForAction(action), filter, selectedFilter, options);
    if (!result.isEmpty())
        setDefaultPathForAction(action, result);
    return result;
}

QStringList DefaultPathManager::getOpenFileNames(const QString &action, QWidget *parent, const QString &caption,
                                                 const QString &filter, QString *selectedFilter,
                                                 QFileDialog::Options options)
{
    const auto result =
        QFileDialog::getOpenFileNames(parent, caption, defaultPathForAction(action), filter, selectedFilter, options);
    if (!result.isEmpty())
        setDefaultPathForAction(action, result.front());
    return result;
}

QString DefaultPathManager::getSaveFileName(const QString &action, QWidget *parent, const QString &caption,
                                            const QString &filter, QString *selectedFilter,
                                            QFileDialog::Options options)
{
    const auto result =
        QFileDialog::getSaveFileName(parent, caption, defaultPathForAction(action), filter, selectedFilter, options);
    if (!result.isEmpty())
        setDefaultPathForAction(action, result);
    return result;
}

void DefaultPathManager::fromVariantList(const QVariantList &list)
{
    defaultPath.clear();
    for (const auto &strListVar : list)
    {
        const auto strList = strListVar.toStringList();
        if (strList.length() != 2)
        {
            LOG_ERR(INFO_OF(strList.length()));
            continue;
        }
        defaultPath[strList[0]] = strList[1];
    }
}

QStringList DefaultPathManager::actionSettingsList()
{
    QStringList list;
    for (const auto &info : SettingsInfo::getSettings())
    {
        if (info.name.startsWith("Default Path/Action/"))
        {
            list.push_back(info.name);
        }
    }
    return list;
}

QString DefaultPathManager::convertPath(const QString &str)
{
    QString result = str;
    for (const auto &key : defaultPath.keys())
        result.replace(QString("${%1}").arg(key), defaultPath[key]);
    const QRegularExpression placeHolderRegex("\\$\\{.*?\\}");
    if (result.contains(placeHolderRegex))
    {
        LOG_WARN("Unknown place holder: " << INFO_OF(str) << INFO_OF(result));
        result.replace(placeHolderRegex, "");
    }
    return result;
}

QVariantList DefaultPathManager::toVariantList()
{
    QVariantList result;
    for (const auto &key : defaultPath.keys())
        result.push_back(QStringList{key, defaultPath[key]});
    return result;
}

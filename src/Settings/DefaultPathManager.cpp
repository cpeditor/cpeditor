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

#include "Settings/DefaultPathManager.hpp"
#include "Core/EventLogger.hpp"
#include "Settings/SettingsManager.hpp"
#include "generated/SettingsHelper.hpp"
#include <QFileInfo>
#include <QRegularExpression>

QString DefaultPathManager::defaultPathForAction(const QString &action)
{
    const auto result = convertPath(SettingsHelper::getDefaultPathAction(action).getUses());
    LOG_INFO(INFO_OF(action) << INFO_OF(result));
    return result;
}

void DefaultPathManager::setDefaultPathForAction(const QString &action, const QString &path)
{
    LOG_INFO(INFO_OF(action) << INFO_OF(path));

    auto list = SettingsHelper::getDefaultPathAction(action).getChanges().split(",");
    const QFileInfo info(path);
    const auto dir = info.isFile() ? info.canonicalPath() : info.canonicalFilePath();

    for (const auto &key : list)
    {
        const auto trimmedKey = key.trimmed();
        if (trimmedKey.isEmpty())
            continue;
        SettingsHelper::getDefaultPath(trimmedKey).setPath(dir);
    }
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
    auto result =
        QFileDialog::getSaveFileName(parent, caption, defaultPathForAction(action), filter, selectedFilter, options);
    if (!result.isEmpty())
        setDefaultPathForAction(action, result);
    return result;
}

QString DefaultPathManager::convertPath(const QString &str)
{
    QString result = str;
    for (const auto &key : SettingsHelper::queryDefaultPath())
        result.replace(QString("${%1}").arg(key), SettingsHelper::getDefaultPath(key).getPath());
    const QRegularExpression placeHolderRegex(R"(\$\{.*?\})");
    if (result.contains(placeHolderRegex))
    {
        LOG_WARN("Unknown place holder: " << INFO_OF(str) << INFO_OF(result));
        result.replace(placeHolderRegex, "");
    }
    return result;
}

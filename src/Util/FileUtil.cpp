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

#include "Util/FileUtil.hpp"
#include "Core/EventLogger.hpp"
#include "Core/MessageLogger.hpp"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSaveFile>
#include <generated/SettingsHelper.hpp>

namespace Util
{
QString fileNameFilter(bool cpp, bool java, bool python)
{
    QString name;

    if (cpp && !java && !python)
        name = "C++ ";
    else if (java && !cpp && !python)
        name = "Java ";
    else if (python && !cpp && !java)
        name = "Python ";

    QString filter;

    if (cpp)
        filter += " *." + cppSuffix.join(" *.");
    if (java)
        filter += " *." + javaSuffix.join(" *.");
    if (python)
        filter += " *." + pythonSuffix.join(" *.");

    return QCoreApplication::translate("Util::FileUtil", "%1Source Files (%2)").arg(name, filter.trimmed());
}

bool saveFile(const QString &path, const QString &content, const QString &head, bool safe, MessageLogger *log,
              bool createDirectory)
{
    if (createDirectory)
    {
        auto dirPath = QFileInfo(path).absolutePath();
        LOG_ERR_IF(!QDir().mkpath(dirPath), QString("Failed to create the directory [%1]").arg(dirPath));
    }
    if (safe && !SettingsHelper::isSaveFaster())
    {
        QSaveFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            if (log != nullptr)
                log->error(head, QCoreApplication::translate("Util::FileUtil",
                                                             "Failed to open [%1]. Do I have write permission?")
                                     .arg(path));
            LOG_ERR("Failed to open [" << path << "]");
            return false;
        }
        file.write(content.toUtf8());
        if (!file.commit())
        {
            if (log != nullptr)
                log->error(head, QCoreApplication::translate("Util::FileUtil",
                                                             "Failed to save to [%1]. Do I have write permission?")
                                     .arg(path));
            LOG_ERR("Failed to save to [" << path << "]");
            return false;
        }
    }
    else
    {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            if (log != nullptr)
                log->error(head, QCoreApplication::translate("Util::FileUtil",
                                                             "Failed to open [%1]. Do I have write permission?")
                                     .arg(path));
            LOG_ERR("unsafe: Failed to open [" << path << "]");
            return false;
        }
        if (file.write(content.toUtf8()) == -1)
        {
            if (log != nullptr)
                log->error(head, QCoreApplication::translate("Util::FileUtil",
                                                             "Failed to save to [%1]. Do I have write permission?")
                                     .arg(path));
            LOG_ERR("unsafe: Failed to save to [" << path << "]");
            return false;
        }
    }
    LOG_INFO("Successfully saved to [" << path << "]");
    return true;
}

QString readFile(const QString &path, const QString &head, MessageLogger *log, bool notExistWarning)
{
    if (!QFile::exists(path))
    {
        if (notExistWarning)
        {
            if (log != nullptr)
                log->warn(head,
                          QCoreApplication::translate("Util::FileUtil", "The file [%1] does not exist").arg(path));
            LOG_WARN(QString("The file [%1] does not exist").arg(path));
        }
        return QString();
    }
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        if (log != nullptr)
            log->error(head,
                       QCoreApplication::translate("Util::FileUtil", "Failed to open [%1]. Do I have read permission?")
                           .arg(path));
        LOG_ERR(QString("Failed to open [%1]").arg(path));
        return QString();
    }
    QString content = file.readAll();
    if (content.isNull())
        return "";
    return content;
}
} // namespace Util

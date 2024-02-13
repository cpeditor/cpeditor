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

#include "Util/FileUtil.hpp"
#include "Core/EventLogger.hpp"
#include "Core/MessageLogger.hpp"
#include "generated/SettingsHelper.hpp"
#include <QCoreApplication>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QSaveFile>
#include <QStandardPaths>
#include <QUrl>

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

QString fileNameWithSuffix(const QString &name, const QString &lang)
{
    QString result = name + ".";
    if (lang == "C++")
        result += cppSuffix[0];
    else if (lang == "Java")
        result += javaSuffix[0];
    else if (lang == "Python")
        result += pythonSuffix[0];
    else
        LOG_WTF("Unknown language: " << lang);
    return result;
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

QString configFilePath(QString path)
{
    QDir configDir(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation));
    const auto currentConfigPath = configDir.absolutePath();
    configDir.cdUp();
    return path.replace("$APPCONFIG", currentConfigPath)
        .replace("$OLDAPPCONFIG", configDir.absoluteFilePath("CP Editor"))
        .replace("$BINARY", QCoreApplication::applicationDirPath());
}

QString firstExistingConfigPath(const QStringList &paths)
{
    for (const QString &path : paths)
    {
        QString realPath = configFilePath(path);
        if (QFile::exists(realPath))
            return realPath;
    }
    return QString();
}

QPair<std::function<void()>, QString> revealInFileManager(const QString &filePath, const QString &name)
{
    LOG_INFO("Revealing " << filePath << "in filemanager");

    // Reference: http://lynxline.com/show-in-finder-show-in-explorer/ and https://forum.qt.io/post/296072

    QPair<std::function<void()>, QString> fallBack = {
        [filePath] { QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(filePath).path())); },
        QCoreApplication::translate("Util::FileUtil", "Open Containing Folder of %1").arg(name)};

    if (!QFile::exists(filePath))
    {
        if (QFile::exists(QFileInfo(filePath).path()))
            return fallBack;
        return {[] {}, QString()};
    }

#if defined(Q_OS_MACOS)
    return {[filePath] {
                QStringList args;
                args << "-e"
                     << "tell application \"Finder\""
                     << "-e"
                     << "activate"
                     << "-e" << QStringLiteral("select POSIX file \"%1\"").arg(filePath) << "-e"
                     << "end tell";
                QProcess::startDetached("osascript", args);
            },
            QCoreApplication::translate("Util::FileUtil", "Reveal %1 in Finder").arg(name)};
#elif defined(Q_OS_WIN)
    return {[filePath] {
                QStringList args;
                args << "/select," << QDir::toNativeSeparators(filePath);
                QProcess::startDetached("explorer", args);
            },
            QCoreApplication::translate("Util::FileUtil", "Reveal %1 in Explorer").arg(name)};
#elif defined(Q_OS_UNIX)
    QProcess proc;
    proc.start("xdg-mime", QStringList() << "query"
                                         << "default"
                                         << "inode/directory");
    auto finished = proc.waitForFinished(2000);
    if (finished)
    {
        auto output = proc.readLine().simplified();
        QString program;
        QStringList args;
        auto nativePath = QUrl::fromLocalFile(filePath).toString();
        if (output == "dolphin.desktop" || output == "org.kde.dolphin.desktop")
        {
            program = "dolphin";
            args << "--select" << nativePath;
        }
        else if (output == "nautilus.desktop" || output == "org.gnome.Nautilus.desktop" ||
                 output == "nautilus-folder-handler.desktop")
        {
            program = "nautilus";
            args << "--no-desktop" << nativePath;
        }
        else if (output == "caja-folder-handler.desktop")
        {
            program = "caja";
            args << "--no-desktop" << nativePath;
        }
        else if (output == "nemo.desktop")
        {
            program = "nemo";
            args << "--no-desktop" << nativePath;
        }
        else if (output == "kfmclient_dir.desktop")
        {
            program = "konqueror";
            args << "--select" << nativePath;
        }
        if (program.isEmpty())
            return fallBack;
        return {[program, args] { QProcess::startDetached(program, args); },
                QCoreApplication::translate("Util::FileUtil", "Reveal %1 in File Manager").arg(name)};
    }
    return fallBack;
#else
    return fallBack;
#endif
}

} // namespace Util

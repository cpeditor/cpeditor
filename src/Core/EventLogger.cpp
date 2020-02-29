/*
 * Copyright (C) 2019-2020 Ashar Khan <ashar786khan@gmail.com>
 *
 * This file is part of cpeditor.
 *
 * cpeditor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * I will not be responsible if cpeditor behaves in unexpected way and
 * causes your ratings to go down and or lose any important contest.
 *
 * Believe Software is "Software" and it isn't immune to bugs.
 *
 */

#include "Core/EventLogger.hpp"
#include <QDateTime>
#include <QDesktopServices>
#include <QDir>
#include <QProcess>
#include <QStandardPaths>
#include <QSysInfo>
#include <QUrl>
#include <QVector>
#include <generated/version.hpp>

namespace Core
{

// In .cpp file because they are used in implementations of EventLogger only

static const int NUMBER_OF_LOGS_TO_KEEP = 50;
static const QString LOG_FILE_NAME("cpeditor");
static const QString LOG_DIR_NAME("cpeditorLogFiles");

QFile Log::logFile;
QTextStream Log::logStream;

void Log::i(const QString &head, const QString &body)
{
    log("INFO", head, body);
}
void Log::w(const QString &head, const QString &body)
{
    log("WARN", head, body);
}
void Log::e(const QString &head, const QString &body)
{
    log("ERR", head, body);
}
void Log::wtf(const QString &head, const QString &body)
{
    log("WTF", head, body);
}

QTextStream &Log::i(const QString &head)
{
    return log("INFO", head);
}
QTextStream &Log::w(const QString &head)
{
    return log("WARN", head);
}
QTextStream &Log::e(const QString &head)
{
    return log("ERR", head);
}
QTextStream &Log::wtf(const QString &head)
{
    return log("WTF", head);
}

void Log::init(int instance, bool dumptoStderr)
{
    logStream.setDevice(&logFile);
    if (!dumptoStderr)
    {
        auto path = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
        if (path.isEmpty())
        {
            e("Logger", "Failed to get writable temp location");
        }
        else
        {
            QDir dir(path);
            dir.mkdir(LOG_DIR_NAME);
            if (dir.cd(LOG_DIR_NAME))
            {
                auto entries = dir.entryList({LOG_FILE_NAME + "*.log"}, QDir::Files, QDir::Time);
                for (int i = NUMBER_OF_LOGS_TO_KEEP; i < entries.length(); ++i)
                    dir.remove(entries[i]);
                logFile.setFileName(dir.filePath(LOG_FILE_NAME +
                                                 QDateTime::currentDateTime().toString("-yyyy-MM-dd-hh-mm-ss-zzz-") +
                                                 QString::number(instance) + ".log"));
                logFile.open(QIODevice::WriteOnly | QFile::Text);
                if (!logFile.isOpen() || !logFile.isWritable())
                {
                    e("Logger", "Failed to open file " + logFile.fileName());
                }
            }
            else
            {
                e("Logger", "Failed to open directory " + dir.filePath(LOG_DIR_NAME));
            }
        }
    }
    else
    {
        logFile.open(stderr, QIODevice::WriteOnly);
    }
    i("Logger", "Event logger has started");
    i("SysInfo", "Gathering system information");
    i("SysInfo", platformInformation());
}

QString Log::dateTimeStamp()
{
    return "[" + QDateTime::currentDateTime().toString(Qt::ISODateWithMs) + "]";
}

QString Log::platformInformation()
{
    QString res;
    // Check https://doc.qt.io/qt-5/qsysinfo.html to know what each identifier means
    res.append("buildABI : " + QSysInfo::buildAbi() + "\n");
    res.append("buildCPUArchitecture : " + QSysInfo::buildCpuArchitecture() + "\n");
    res.append("currentCPUArchitecture : " + QSysInfo::currentCpuArchitecture() + "\n");
    res.append("kernelType : " + QSysInfo::kernelType() + "\n");
    res.append("kernelVersion : " + QSysInfo::kernelVersion() + "\n");
    res.append("prettyProductName : " + QSysInfo::prettyProductName() + "\n");
    res.append("machineHostName : " + QSysInfo::machineHostName() + "\n");
    res.append("machineUniqueId : " + QSysInfo::machineUniqueId() + "\n");
    res.append("productType : " + QSysInfo::productType() + "\n");
    res.append("productVersion : " + QSysInfo::productVersion() + "\n");
    res.append("Appversion : " APP_VERSION "\n");
    res.append("Git commit hash : " GIT_COMMIT_HASH);
    return res;
}

void Log::log(const QString &priority, const QString &head, const QString &body)
{
    log(priority, head) << body << endl;
}

QTextStream &Log::log(const QString &priority, const QString &head)
{
    return logStream << dateTimeStamp() << "[" << priority << "][" << head << "] ";
}

void Log::revealInFileManager()
{
    // Reference: http://lynxline.com/show-in-finder-show-in-explorer/ and https://forum.qt.io/post/296072
    QString filePath = logFile.fileName();
    i("log/revealInFileManager") << "Revealing in filemanager file " << filePath;
#if defined(Q_OS_MACOS)
    QStringList args;
    args << "-e";
    args << "tell application \"Finder\"";
    args << "-e";
    args << "activate";
    args << "-e";
    args << "select POSIX file \"" + filePath + "\"";
    args << "-e";
    args << "end tell";
    QProcess::startDetached("osascript", args);
#elif defined(Q_OS_WIN)
    QStringList args;
    args << "/select," << QDir::toNativeSeparators(filePath);
    QProcess::startDetached("explorer", args);
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
        {
            QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(filePath).path()));
        }
        else
        {
            QProcess openProcess;
            openProcess.startDetached(program, args);
        }
    }
    else
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(filePath).path()));
    }
#else
    QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(filePath).path()));
}
#endif
} // namespace Core

void Log::clearOldLogs()
{
    i("log/clearOldLogs", "Invoked");
    auto path = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QDir dir(path);
    if (dir.cd(LOG_DIR_NAME))
    {
        auto entries = dir.entryList({LOG_FILE_NAME + "*.log"}, QDir::Files, QDir::Time);
        for (auto const &e : entries)
        {
            if (e != logFile.fileName())
            {
                dir.remove(e);
                i("log/clearOldLog") << "Deleted " << e << endl;
            }
        }
    }
}

} // namespace Core

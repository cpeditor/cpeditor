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

#include "Extensions/WakaTime.hpp"
#include "Settings/SettingsManager.hpp"
#include "generated/SettingsHelper.hpp"
#include <QObject>
#include <QProcess>

namespace Extensions
{

WakaTime::WakaTime(const QString &path) : wakaTimePath(path)
{
}

void WakaTime::updatePath(const QString &path)
{
    wakaTimePath = path;
}

void WakaTime::sendHeartBeat(QString filePath, bool isWrite)
{
    QTime now = QTime::currentTime();
    if (!isWrite && filePath == lastFilePath && lastHeartBeat.isValid() && lastHeartBeat.msecsTo(now) > 2 * 60 * 1000)
        return;
    lastHeartBeat = now;
    QProcess *wakaTimeProcess = new QProcess();
    wakaTimeProcess->setProgram(wakaTimePath);
    QStringList arg = {"--file",   filePath,           "--key", SettingsHelper::getWakaTimeApiKey(),
                       "--plugin", "cpeditor-wakatime"};
    if (isWrite)
        arg.append("--write");
    wakaTimeProcess->setArguments(arg);
    wakaTimeProcess->start();
}
} // namespace Extensions
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
#include "Core/EventLogger.hpp"
#include "generated/SettingsHelper.hpp"
#include "generated/version.hpp"
#include <QProcess>

namespace Extensions
{

WakaTime::WakaTime(QObject *parent) : QObject(parent)
{
}

void WakaTime::sendHeartBeat(const QString &filePath, const QString &problemURL, const QString &language, bool isWrite)
{
    if (!SettingsHelper::isWakaTimeEnable())
        return;

    const auto now = QDateTime::currentDateTime();

    QString entity;

    if (!filePath.isEmpty())
        entity = filePath;
    else if (!problemURL.isEmpty())
        entity = problemURL;
    else
        return; // no need to send an unsaved non-problem file

    if (!(isWrite || entity != lastEntity || lastTime.isNull() || lastTime.secsTo(now) > 2 * 60))
        return;
    lastTime = now;
    lastEntity = entity;

    QStringList args;
    args << "--plugin"
         << "cpeditor-wakatime/" APP_VERSION;

    if (filePath.isEmpty())
        args << "--entity-type"
             << "domain";
    args << "--entity" << entity;

    if (!language.isEmpty())
        args << "--language" << language;

    const auto key = SettingsHelper::getWakaTimeApiKey();
    if (!key.isEmpty()) // wakatime-cli uses ~/.wakatime.cfg by default
        args << "--key" << key;

    if (isWrite)
        args << "--write";

    if (SettingsHelper::isWakaTimeProxy() && QStringList{"Http", "Socks5"}.contains(SettingsHelper::getProxyType()))
    {
        QString proxyStr = SettingsHelper::getProxyType().toLower() + "://";
        const auto user = SettingsHelper::getProxyUser();
        const auto passwd = SettingsHelper::getProxyPassword();
        if (!user.isEmpty() && !passwd.isEmpty())
            proxyStr += QString("%1:%2").arg(user, passwd);
        proxyStr += QString("%1:%2").arg(SettingsHelper::getProxyHostName()).arg(SettingsHelper::getProxyPort());
        args << "--proxy" << proxyStr;
    }

    LOG_INFO(INFO_OF(args.join(' ')));

    auto *wakaTimeProcess = new QProcess();
    connect(wakaTimeProcess, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), wakaTimeProcess,
            &QObject::deleteLater);
    wakaTimeProcess->start(SettingsHelper::getWakaTimePath(), args);
}
} // namespace Extensions

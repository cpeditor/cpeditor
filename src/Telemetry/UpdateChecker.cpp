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

#include "Telemetry/UpdateChecker.hpp"
#include "Core/EventLogger.hpp"
#include "Widgets/UpdatePresenter.hpp"
#include "Widgets/UpdateProgressDialog.hpp"
#include <QJsonArray>
#include <QJsonDocument>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <algorithm>
#include <generated/SettingsHelper.hpp>
#include <generated/version.hpp>

namespace Telemetry
{
UpdateChecker::UpdateChecker()
{
    progress = new Widgets::UpdateProgressDialog();
    presenter = new Widgets::UpdatePresenter();
    manager = new QNetworkAccessManager();
    request = new QNetworkRequest(QUrl("https://api.github.com/repos/cpeditor/cpeditor/releases"));
    connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(managerFinished(QNetworkReply *)));
    connect(progress, SIGNAL(canceled()), this, SLOT(cancelCheckUpdate()));
}

UpdateChecker::~UpdateChecker()
{
    delete progress;
    delete presenter;
    delete manager;
    delete request;
}

void UpdateChecker::checkUpdate(bool silent)
{
    LOG_INFO(BOOL_INFO_OF(silent));
    if (!silent)
        progress->start();
    cancelCheckUpdate();
    manager->get(*request);
}

void UpdateChecker::cancelCheckUpdate()
{
    if (manager == nullptr)
        return;
    delete manager;
    manager = new QNetworkAccessManager();
    connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(managerFinished(QNetworkReply *)));
}

UpdateChecker::UpdateMetaInformation UpdateChecker::toMetaInformation(const QJsonDocument &release)
{
    UpdateMetaInformation result;

    auto assets = release["assets"].toArray().toVariantList();

    for (auto const &e : assets)
    {
        auto asset = QJsonDocument::fromVariant(e);
        auto assetName = asset["name"].toString();
#if defined(Q_OS_WIN)
        if (assetName.endsWith(".exe"))
#elif defined(Q_OS_MAC)
        if (assetName.endsWith(".dmg"))
#else
        if (assetName.endsWith(".AppImage"))
#endif
            result.assetDownloadUrl = asset["browser_download_url"].toString();
    }
    result.preview = release["prerelease"].toBool();
    result.version = release["tag_name"].toString();
    result.changelog = release["body"].toString();
    result.releasePageUrl = release["html_url"].toString();
    return result;
}

void UpdateChecker::managerFinished(QNetworkReply *reply)
{
    if (reply->error())
    {
        LOG_ERR(INFO_OF(reply->errorString()));
        progress->onUpdateFailed(reply->errorString());
        return;
    }

    QString jsonReply = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(jsonReply.toUtf8());

    QVector<QPair<Version, UpdateMetaInformation>> releases;

    for (auto const &e : doc.array().toVariantList())
    {
        auto json = QJsonDocument::fromVariant(e);
        auto info = toMetaInformation(json);
        Version version(info.version);
        LOG_INFO(INFO_OF(info.version) << INFO_OF(version.valid));
        if (version.valid)
            releases.push_back(qMakePair(version, info));
    }

    std::sort(releases.begin(), releases.end(),
              [](const QPair<Version, UpdateMetaInformation> &lhs, const QPair<Version, UpdateMetaInformation> &rhs) {
                  return lhs.first < rhs.first;
              });

    while (!SettingsHelper::isBeta() && !releases.isEmpty() && releases.last().second.preview)
        releases.pop_back();

    if (releases.isEmpty())
    {
        progress->onUpdateFailed("No release is found.");
        return;
    }

    auto latestVersion = releases.last().first;
    auto latestInfo = releases.last().second;
    Version currentVersion(APP_VERSION);

    if (latestInfo.assetDownloadUrl.isEmpty())
    {
        LOG_INFO("No download URL");
        progress->onUpdateFailed(QString("No download URL of the version [%1] is found.").arg(latestInfo.version));
    }
    else if (currentVersion < latestVersion)
    {
        LOG_INFO("Update available");
        auto last = currentVersion;
        QStringList changelog;
        for (auto release : releases)
        {
            auto version = release.first;
            bool used = false;
            if (currentVersion < version)
            {
                if ((version.major == latestVersion.major && version.minor == latestVersion.minor) ||
                    (version.major != last.major || version.minor != last.minor))
                {
                    used = true;
                    last = version;
                    changelog.push_back(release.second.changelog);
                }
            }
            LOG_INFO(QString("Version %1 is %2 the changelog")
                         .arg(release.second.version)
                         .arg(used ? "in" : "excluded from"));
        }
        std::reverse(changelog.begin(), changelog.end());
        latestInfo.changelog = changelog.join("\n\n");
        progress->close();
        presenter->showUpdate(latestInfo);
    }
    else
    {
        LOG_INFO("Already up to date");
        progress->onAlreadyUpToDate();
    }
}

void UpdateChecker::closeAll()
{
    progress->close();
    presenter->close();
}

UpdateChecker::Version::Version(const QString &version)
{
    auto splitted = version.split('.');

    valid = true;

    do
    {
        if (splitted.length() != 3)
        {
            valid = false;
            break;
        }

        major = splitted[0].toInt(&valid);
        if (!valid)
            break;

        minor = splitted[1].toInt(&valid);
        if (!valid)
            break;

        patch = splitted[2].toInt(&valid);
        if (!valid)
            break;
    } while (false);

    if (!valid)
        major = minor = patch = 0;
}

bool UpdateChecker::Version::operator<(const Version &rhs) const
{
    if (major != rhs.major)
        return major < rhs.major;
    if (minor != rhs.minor)
        return minor < rhs.minor;
    return patch < rhs.patch;
}
} // namespace Telemetry

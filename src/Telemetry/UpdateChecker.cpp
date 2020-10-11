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
#include "generated/SettingsHelper.hpp"
#include "generated/portable.hpp"
#include "generated/version.hpp"
#include <QJsonArray>
#include <QJsonDocument>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkProxy>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QSslSocket>
#include <algorithm>

namespace Telemetry
{
UpdateChecker::UpdateChecker()
{
    progress = new Widgets::UpdateProgressDialog();
    presenter = new Widgets::UpdatePresenter();
    request = new QNetworkRequest(QUrl("https://api.github.com/repos/cpeditor/cpeditor/releases"));
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
    if (manager)
        delete manager;
    manager = new QNetworkAccessManager();
    updateProxy();
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
#ifdef PORTABLE_VERSION
        if (assetName.contains("windows") && assetName.contains("portable"))
#else
        if (assetName.contains("windows") && !assetName.contains("portable"))
#endif
        {
            result.assetDownloadUrl = asset["browser_download_url"].toString();
            break;
        }

#elif defined(Q_OS_MACOS)
        if (assetName.contains("macos"))
        {
            result.assetDownloadUrl = asset["browser_download_url"].toString();
            break;
        }
#else
        result.assetDownloadUrl = release["html_url"].toString();
        break;
#endif
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
        auto error = reply->errorString();
        if (error.contains("TLS initialization failed"))
        {
            error += "<br /><br />" +
                     tr("This error is probably caused by the lack of the OpenSSL library. You can visit "
                        "<a href=\"https://wiki.openssl.org/index.php/Binaries\">the OpenSSLWiki</a> to find a binary "
                        "to install, or install it via your favourite package manager. You have to install a version "
                        "compatible with this version: [%1]")
                         .arg(QSslSocket::sslLibraryBuildVersionString());
        }
        progress->onUpdateFailed(error);
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
        progress->onUpdateFailed(tr("No release is found."));
        return;
    }

    auto latestVersion = releases.last().first;
    auto latestInfo = releases.last().second;
    Version currentVersion(APP_VERSION);

    if (currentVersion < latestVersion)
    {
        LOG_INFO("Update available");

        if (latestInfo.assetDownloadUrl.isEmpty())
        {
            LOG_INFO("No download URL");
            progress->onUpdateFailed(tr("No download URL of the version [%1] is found.").arg(latestInfo.version));
        }

        auto last = currentVersion;
        QStringList changelog;
        for (auto release : releases)
        {
            auto version = release.first;
            bool used = false;
            if (currentVersion < version)
            {
                do
                {
                    if (version.major == latestVersion.major && version.minor == latestVersion.minor)
                    {
                        if (!latestInfo.preview && release.second.preview)
                        {
                            break;
                        }
                    }
                    else if (version.major == last.major && version.minor == last.minor)
                    {
                        break;
                    }

                    used = true;
                    last = version;
                    changelog.push_back(release.second.changelog);
                } while (false);
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

void UpdateChecker::updateProxy()
{
    if (!SettingsHelper::isProxyEnabled())
        manager->setProxy({QNetworkProxy::NoProxy});
    else if (SettingsHelper::getProxyType() == "System")
        manager->setProxy({QNetworkProxy::DefaultProxy});
    else
    {
        QNetworkProxy proxy;
        if (SettingsHelper::getProxyType() == "Http")
            proxy.setType(QNetworkProxy::HttpProxy);
        else if (SettingsHelper::getProxyType() == "Socks5")
            proxy.setType(QNetworkProxy::Socks5Proxy);
        else
        {
            LOG_WTF("Unknown proxy type: " << SettingsHelper::getProxyType());
            proxy.setType(QNetworkProxy::DefaultProxy);
        }
        proxy.setHostName(SettingsHelper::getProxyHostName());
        proxy.setPort(SettingsHelper::getProxyPort());
        proxy.setUser(SettingsHelper::getProxyUser());
        proxy.setPassword(SettingsHelper::getProxyPassword());
        manager->setProxy(proxy);
    }
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

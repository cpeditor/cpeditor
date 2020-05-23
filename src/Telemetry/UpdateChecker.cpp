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
#include <QJsonArray>
#include <QJsonDocument>
#include <QMessageBox>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <generated/version.hpp>

namespace Telemetry
{
UpdateChecker::UpdateChecker()
{
    manager = new QNetworkAccessManager();
    request = new QNetworkRequest();
    QObject::connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(managerFinished(QNetworkReply *)));
}
UpdateChecker::~UpdateChecker()
{
    delete manager;
    delete request;
}

void UpdateChecker::checkUpdate(bool beta)
{
    LOG_INFO("Beta checks included : " << beta);
    this->beta = beta;

    request->setUrl(QUrl("https://api.github.com/repos/cpeditor/cpeditor/releases"));
    manager->get(*request);
}
void UpdateChecker::cancelCheckUpdate()
{
    delete manager;
    manager = new QNetworkAccessManager();
    QObject::connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(managerFinished(QNetworkReply *)));
}
void UpdateChecker::toMetaInformation(QJsonDocument &release, UpdateChecker::UpdateMetaInformation &result)
{

    auto assets = release["assets"].toArray().toVariantList();

    for (auto const &e : assets)
    {
        auto asset = QJsonDocument::fromVariant(e);
        auto assetName = asset["name"].toString();
#if defined(Q_OS_WIN32)
        if (assetName.endsWith(".exe"))
#elif defined(Q_OS_MAC)
        if (assetName.endsWith(".dmg"))
#else
        if (assetName.endsWith(".AppImage"))
#endif
            result.assetDownloadUrl = asset["browser_download_url"].toString();
    }
    result.body = release["body"].toString();
    result.name = release["name"].toString();
    result.preview = release["prerelease"].toBool();
    result.releasePageUrl = release["html_url"].toString();
    result.result = result.preview ? UpdateCheckerResult::BETA_UPDATE : UpdateCheckerResult::STABLE_UPDATE;
    result.tagName = release["tag_name"].toString().remove("-beta").remove("-rc");
}

bool UpdateChecker::compareVersion(QString const &a, QString const &b)
{
    LOG_INFO(INFO_OF(a) << INFO_OF(b));
    // returns true if a is higher version than b;

    auto aV = a.split(".");
    auto bV = b.split(".");

    if (aV.size() < 3 && bV.size() < 3)
        return false; // invalid versions;

    int aMajor = aV[0].toInt();
    int aMinor = aV[1].toInt();
    int aPatch = aV[2].toInt();

    int bMajor = bV[0].toInt();
    int bMinor = bV[1].toInt();
    int bPatch = bV[2].toInt();

    if (aMajor > bMajor)
        return true;
    if (aMajor < bMajor)
        return false;

    if (aMinor > bMinor)
        return true;
    if (aMinor < bMinor)
        return false;

    if (aPatch > bPatch)
        return true;

    return false;
}

void UpdateChecker::managerFinished(QNetworkReply *reply)
{
    if (reply->error())
    {
        LOG_ERR("Error returned " << reply->errorString());
        emit updateCheckerFailed(reply->errorString());
        return;
    }
    QString jsonReply = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(jsonReply.toUtf8());

    QJsonDocument release;
    UpdateMetaInformation result;
    result.tagName = "0.0.0";
    result.result = UpdateCheckerResult::UNKNOWN;
    result.preview = false;

    for (auto const &e : doc.array().toVariantList())
    {
        release = QJsonDocument::fromVariant(e);
        if (release["prerelease"].toBool())
        {
            if (beta)
            {
                toMetaInformation(release, result);
                break;
            }
        }
        else // stable
        {
            toMetaInformation(release, result);
            break;
        }
    }

    if (result.tagName == "0.0.0")
    {
        UpdateMetaInformation info;
        info.result = UpdateCheckerResult::UNKNOWN;
        emit updateCheckerFinished(info);
        return;
    }

    bool isUpdateAvailable = compareVersion(result.tagName, APP_VERSION);
    LOG_INFO(BOOL_INFO_OF(isUpdateAvailable));

    if (isUpdateAvailable)
    {
        emit updateCheckerFinished(result);
    }
    else
    {
        result.result = UpdateCheckerResult::NO_UPDATES;
        emit updateCheckerFinished(result);
        LOG_INFO("No update available. Silently logged it");
    }
}
} // namespace Telemetry

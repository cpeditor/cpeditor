/*
 * Copyright (C) 2019-2020 Ashar Khan <ashar786khan@gmail.com>
 *
 * This file is part of CPEditor.
 *
 * CPEditor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * I will not be responsible if CPEditor behaves in unexpected way and
 * causes your ratings to go down and or loose any important contest.
 *
 * Believe Software is "Software" and it isn't immune to bugs.
 *
 */

#include "Telemetry/UpdateNotifier.hpp"
#include "Core/EventLogger.hpp"
#include <QJsonArray>
#include <QJsonDocument>
#include <QMessageBox>

namespace Telemetry
{
UpdateNotifier::UpdateNotifier(bool useBeta)
{
    Core::Log::i("updateNotifier/constructed") << "use beta ? " << useBeta << endl;
    manager = new QNetworkAccessManager();
    QObject::connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(managerFinished(QNetworkReply *)));
    beta = useBeta;
}
UpdateNotifier::~UpdateNotifier()
{
    Core::Log::i("updateNotifer/destroyed", "deleting network access manager");
    delete manager;
}
void UpdateNotifier::setBeta(bool value)
{
    Core::Log::i("updateNotifer/setBeta") << "value is ? " << value << endl;
    beta = value;
}

void UpdateNotifier::checkUpdate(bool force)
{
    Core::Log::i("updateNotifier/checkupdate") << "Forceful update : " << force << endl;
    this->force = force;
    request.setUrl(QUrl("https://api.github.com/repos/coder3101/cp-editor/releases"));
    manager->get(request);
}

bool compareVersion(QString const &a, QString const &b)
{
    Core::Log::i("updateNotifier/compareVersion") << "latest : " << a << "current : " << b << endl;
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
    if (aMinor > bMinor)
        return true;
    if (aPatch > bPatch)
        return true;

    return false;
}

void UpdateNotifier::managerFinished(QNetworkReply *reply)
{
    if (reply->error())
    {
        Core::Log::e("updateNotifer/managerFinished") << reply->errorString() << endl;
        qDebug() << reply->errorString();
        return;
    }
    QString jsonReply = reply->readAll();

    Core::Log::i("updateNotifier/managerFinished") << jsonReply << endl;

    QJsonDocument doc = QJsonDocument::fromJson(jsonReply.toUtf8());

    QJsonDocument release;
    QString downloadUrl = "https://github.com/coder3101/cp-editor";
    bool isBeta = false;
    QString latestRelease = "0.0.0";

    for (auto const &e : doc.array().toVariantList())
    {
        release = QJsonDocument::fromVariant(e);

        if (release["prerelease"].toBool())
        {
            if (beta)
            {
                latestRelease = release["tag_name"].toString().remove("-beta").remove("-rc");
                isBeta = true;
                downloadUrl = release["html_url"].toString();
                break;
            }
        }
        else // stable
        {
            latestRelease = release["tag_name"].toString().remove("-stable");
            isBeta = false;
            downloadUrl = release["html_url"].toString();
            break;
        }
    }

    bool isUpdateAvailable = compareVersion(latestRelease, APP_VERSION);

    Core::Log::i("updateNotifier/managerFinished") << "update available ? " << isUpdateAvailable << endl;

    if (beta && isBeta && isUpdateAvailable)
    {
        QMessageBox::about(nullptr, QString::fromStdString("Beta Update available"),

                           QString::fromStdString("A new beta update " + latestRelease.toStdString() +
                                                  " is available. <a href = " + downloadUrl.toStdString() +
                                                  ">Please Download" + "</a>"));
    }
    else if (!isBeta && isUpdateAvailable)
    {
        QMessageBox::about(nullptr, QString::fromStdString("New Update available"),

                           QString::fromStdString("A new stable update " + latestRelease.toStdString() +
                                                  " is available. <a href = " + downloadUrl.toStdString() +
                                                  ">Please Download" + "</a>"));
    }
    else if (force)
    {
        QMessageBox::about(
            nullptr, QString::fromStdString("No new update"),
            QString::fromStdString(
                "You are already running latest release. Keep checking so you dont miss on important update."));
        force = false;
    }
    else
    {
        Core::Log::i("updateNotifier/managerFinshed", "No update available. Silently logged it");
    }
}
} // namespace Telemetry

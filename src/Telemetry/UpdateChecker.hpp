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

#ifndef UPDATENOTIFIER_HPP
#define UPDATENOTIFIER_HPP

#include <QObject>

class QNetworkAccessManager;
class QNetworkReply;
class QNetworkRequest;

namespace Telemetry
{
class UpdateChecker : public QObject
{
    Q_OBJECT

  public:
    enum class UpdateCheckerResult
    {
        UNKNOWN,
        STABLE_UPDATE,
        BETA_UPDATE,
        NO_UPDATES,
    };

    struct UpdateMetaInformation
    {
        bool preview;
        QString name;
        QString body;
        QString assetDownloadUrl;
        QString releasePageUrl;
        QString tagName;
        UpdateCheckerResult result;
    };

    UpdateChecker();
    void checkUpdate(bool beta = false);
    void cancelCheckUpdate();
    ~UpdateChecker();

  private slots:
    void managerFinished(QNetworkReply *reply);

  signals:
    void updateCheckerFinished(const UpdateMetaInformation &information);
    void updateCheckerFailed(const QString &errorText);

  private:
    /**
     * @returns whether the *newVersion* is higher than the *currentVersion*
     */
    bool compareVersion(const QString &newVersion, const QString &currentVersion);
    UpdateMetaInformation toMetaInformation(const QJsonDocument &release);

    QNetworkAccessManager *manager;
    QNetworkRequest *request;

    bool beta;
};
} // namespace Telemetry

#endif // UPDATENOTIFIER_HPP

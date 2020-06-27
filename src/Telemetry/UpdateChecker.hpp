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

namespace Widgets
{
class UpdatePresenter;
class UpdateProgressDialog;
} // namespace Widgets

namespace Telemetry
{
class UpdateChecker : public QObject
{
    Q_OBJECT

  public:
    struct UpdateMetaInformation
    {
        bool preview;
        QString version;
        QString changelog;
        QString assetDownloadUrl;
        QString releasePageUrl;
    };

    explicit UpdateChecker();
    void checkUpdate(bool silent);
    void closeAll();
    ~UpdateChecker();

  private slots:
    void cancelCheckUpdate();
    void managerFinished(QNetworkReply *reply);

  private:
    struct Version
    {
        bool valid;
        int major;
        int minor;
        int patch;

        Version(const QString &version);

        bool operator<(const Version &rhs) const;
    };

    void updateProxy();
    UpdateMetaInformation toMetaInformation(const QJsonDocument &release);

    Widgets::UpdateProgressDialog *progress = nullptr;
    Widgets::UpdatePresenter *presenter = nullptr;

    QNetworkAccessManager *manager = nullptr;
    QNetworkRequest *request = nullptr;
};
} // namespace Telemetry

#endif // UPDATENOTIFIER_HPP

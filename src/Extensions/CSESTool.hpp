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

#ifndef CSESTOOL_HPP
#define CSESTOOL_HPP

#include <QJsonObject>
#include <QObject>
#include <QString>

class QNetworkAccessManager;
class QNetworkReply;
class QNetworkRequest;
class QUrl;

namespace Extensions
{

class CSESTool : public QObject
{
    Q_OBJECT

  public:
    explicit CSESTool(QObject *parent = nullptr);

    void login();
    void checkLoginStatus();
    bool isLoggedIn() const;
    void clearToken(); // drop the stored token (e.g. when it's invalid); isLoggedIn() then returns false

    void submitFile(const QString &scope, const QString &filePath, const QString &langName, const QString &taskId = {});

    void fetchSubmission(const QString &scope, qint64 submissionId, bool longPoll = true);

    static bool parseCsesUrl(const QString &url, QString &scope, QString &taskId);

  signals:
    void loginFlowStarted(const QString &authUrl);
    void loginSucceeded(const QString &username);
    void loginPending();
    void loginFailed(const QString &reason);

    void submissionCreated(qint64 submissionId);
    void submissionUpdated(const QJsonObject &info);
    void submissionFinished(const QJsonObject &info);
    void submitError(const QString &errorCode, const QString &message);

    void networkError(const QString &message);

  private:
    QNetworkAccessManager *m_nam;
    QString m_token;
    bool m_loginFlowActive = false; // true between POST /login and confirmed browser login
    int m_loginPollCount = 0;
    int m_pollRetries = 0;

    void persistToken(const QString &token);
    void updateProxy();

    static QString apiUrl(const QString &path);
    static QString scopedUrl(const QString &scope, const QString &endpoint);

    QNetworkRequest authRequest(const QUrl &url) const;

    bool handleError(QNetworkReply *reply);
    static QString parseErrorCode(const QByteArray &body);
    static QString parseErrorMessage(const QByteArray &body);
};

} // namespace Extensions

#endif // CSESTOOL_HPP

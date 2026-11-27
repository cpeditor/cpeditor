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

#include "Extensions/CSESTool.hpp"
#include "Core/EventLogger.hpp"
#include "generated/SettingsHelper.hpp"
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QNetworkProxy>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QRegularExpression>
#include <QTimer>
#include <QUrl>
#include <QUrlQuery>

namespace Extensions
{

constexpr qint64 MaxSourceSize = 128LL * 1024;
// ponytail: fixed 2 s retry interval, capped so a dead connection stops after ~5 min
constexpr int MaxRetryCount = 150;

CSESTool::CSESTool(QObject *parent) : QObject(parent), m_nam(new QNetworkAccessManager(this))
{
    updateProxy();
    m_token = SettingsHelper::getCSESToken();
}

void CSESTool::updateProxy()
{
    if (!SettingsHelper::isProxyEnabled())
        m_nam->setProxy({QNetworkProxy::NoProxy});
    else if (SettingsHelper::getProxyType() == "System")
        m_nam->setProxy({QNetworkProxy::DefaultProxy});
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
        m_nam->setProxy(proxy);
    }
}

void CSESTool::persistToken(const QString &token)
{
    m_token = token;
    SettingsHelper::setCSESToken(token);
}

void CSESTool::clearToken()
{
    m_token.clear();
    m_loginFlowActive = false;
    SettingsHelper::setCSESToken({});
}

bool CSESTool::isLoggedIn() const
{
    return !m_token.isEmpty();
}

QString CSESTool::apiUrl(const QString &path)
{
    return "https://cses.fi/api" + path;
}

QString CSESTool::scopedUrl(const QString &scope, const QString &endpoint)
{
    bool isNumber = false;
    scope.toLongLong(&isNumber);
    if (isNumber)
        return apiUrl(QString("/contests/%1/%2").arg(scope, endpoint));
    return apiUrl(QString("/courses/%1/%2").arg(scope, endpoint));
}

QNetworkRequest CSESTool::authRequest(const QUrl &url) const
{
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    if (!m_token.isEmpty())
        req.setRawHeader("X-Auth-Token", m_token.toUtf8());
    return req;
}

bool CSESTool::handleError(QNetworkReply *reply)
{
    int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (status >= 400)
    {
        // Qt reports every HTTP 401 as AuthenticationRequiredError ("Host requires authentication"),
        // so check the HTTP status first and surface the real CSES error code from the JSON body.
        QByteArray body = reply->readAll();
        QString code = parseErrorCode(body);
        QString msg = parseErrorMessage(body);
        // The real server returns "invalid_api_key" for a fresh, not-yet-confirmed token from
        // POST /login (the documented "pending_api_key" only exists in the mock server). So while
        // a login flow is active, "invalid_api_key" means "still waiting for the browser login",
        // not "the token is dead".
        if (code == "pending_api_key" || (code == "invalid_api_key" && m_loginFlowActive))
        {
            emit loginPending();
        }
        else if (code == "invalid_api_key")
        {
            emit loginFailed(msg.isEmpty() ? tr("Invalid API key") : msg);
        }
        else
        {
            emit submitError(code, msg);
        }
        return true;
    }
    if (reply->error() != QNetworkReply::NoError)
    {
        emit networkError(reply->errorString());
        return true;
    }
    return false;
}

QString CSESTool::parseErrorCode(const QByteArray &body)
{
    QJsonDocument doc = QJsonDocument::fromJson(body);
    return doc.object()["code"].toString();
}

QString CSESTool::parseErrorMessage(const QByteArray &body)
{
    QJsonDocument doc = QJsonDocument::fromJson(body);
    return doc.object()["message"].toString();
}

void CSESTool::login()
{
    m_loginPollCount = 0;
    QNetworkRequest req(authRequest(QUrl(apiUrl("/login"))));
    QNetworkReply *reply = m_nam->post(req, QByteArray());
    connect(reply, &QNetworkReply::finished, this, [this, reply] {
        reply->deleteLater();
        if (handleError(reply))
            return;
        QByteArray body = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(body);
        if (doc.isNull())
        {
            emit networkError(tr("Invalid response from the server"));
            return;
        }
        QJsonObject obj = doc.object();
        QString token = obj["X-Auth-Token"].toString();
        QString authUrl = obj["authentication_url"].toString();
        persistToken(token);
        m_loginFlowActive = true;
        emit loginFlowStarted(authUrl);
        checkLoginStatus();
    });
}

void CSESTool::checkLoginStatus()
{
    QNetworkRequest req(authRequest(QUrl(apiUrl("/login"))));
    QNetworkReply *reply = m_nam->get(req);
    connect(reply, &QNetworkReply::finished, this, [this, reply] {
        reply->deleteLater();
        int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (status == 200)
        {
            m_loginFlowActive = false;
            QByteArray body = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(body);
            if (doc.isNull())
            {
                emit networkError(tr("Invalid response from the server"));
                return;
            }
            QString username = doc.object()["username"].toString();
            emit loginSucceeded(username);
        }
        else if (status == 401)
        {
            QByteArray body = reply->readAll();
            QString code = parseErrorCode(body);
            // Keep polling while the login flow is active: the real server reports
            // "invalid_api_key" for a fresh token until the browser login completes.
            if (code == "pending_api_key" || (code == "invalid_api_key" && m_loginFlowActive))
            {
                if (++m_loginPollCount > MaxRetryCount)
                {
                    m_loginFlowActive = false;
                    emit loginFailed(tr("Login timed out"));
                    return;
                }
                emit loginPending();
                QTimer::singleShot(2000, this, &CSESTool::checkLoginStatus);
            }
            else
            {
                m_loginFlowActive = false;
                emit loginFailed(parseErrorMessage(body));
            }
        }
        else
        {
            handleError(reply);
            m_loginFlowActive = false;
        }
    });
}

void CSESTool::submitFile(const QString &scope, const QString &filePath, const QString &langName, const QString &taskId)
{
    if (m_loginFlowActive)
    {
        emit submitError("client_error", tr("The login is not confirmed yet"));
        return;
    }
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
    {
        emit submitError("client_error", "Failed to open file");
        return;
    }
    QByteArray content = file.readAll();
    if (content.size() > MaxSourceSize)
    {
        emit submitError("client_error", "File is too large (limit 128 kB)");
        return;
    }
    QString filename = QFileInfo(filePath).fileName();
    QString url = scopedUrl(scope, "submissions");
    if (!taskId.isEmpty())
    {
        QUrlQuery query;
        query.addQueryItem("task", taskId);
        url += "?" + query.toString();
    }
    QJsonObject langObj;
    if (!langName.isEmpty())
        langObj["name"] = langName;
    QJsonObject body;
    body["filename"] = filename;
    body["language"] = langObj;
    body["content"] = QString::fromLatin1(content.toBase64());
    QNetworkRequest req(authRequest(QUrl(url)));
    QNetworkReply *reply = m_nam->post(req, QJsonDocument(body).toJson());
    connect(reply, &QNetworkReply::finished, this, [this, reply, scope] {
        reply->deleteLater();
        int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (status == 200)
        {
            QByteArray body = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(body);
            if (doc.isNull())
            {
                emit networkError(tr("Invalid response from the server"));
                return;
            }
            QJsonObject info = doc.object();
            qint64 id = info["id"].toVariant().toLongLong();
            emit submissionCreated(id);
            fetchSubmission(scope, id, true);
        }
        else
        {
            handleError(reply);
        }
    });
}

void CSESTool::fetchSubmission(const QString &scope, qint64 submissionId, bool longPoll)
{
    QString url = scopedUrl(scope, QString("submissions/%1").arg(submissionId));
    if (longPoll)
        url += "?poll=true";
    QNetworkRequest req(authRequest(QUrl(url)));
    if (longPoll)
        req.setTransferTimeout(35000);
    QNetworkReply *reply = m_nam->get(req);
    connect(reply, &QNetworkReply::finished, this, [this, reply, scope, submissionId] {
        reply->deleteLater();
        int status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (status == 0 && reply->error() != QNetworkReply::NoError)
        {
            // No HTTP response (timeout, connection dropped): keep polling instead of giving up.
            if (++m_pollRetries <= MaxRetryCount)
            {
                QTimer::singleShot(2000, this,
                                   [this, scope, submissionId] { fetchSubmission(scope, submissionId, true); });
                return;
            }
            m_pollRetries = 0;
            emit networkError(reply->errorString());
            return;
        }
        if (handleError(reply))
            return;
        m_pollRetries = 0;
        QByteArray body = reply->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(body);
        if (doc.isNull())
        {
            emit networkError(tr("Invalid response from the server"));
            return;
        }
        QJsonObject info = doc.object();
        emit submissionUpdated(info);
        if (info["pending"].toBool())
            fetchSubmission(scope, submissionId, true);
        else
            emit submissionFinished(info);
    });
}

bool CSESTool::parseCsesUrl(const QString &url, QString &scope, QString &taskId)
{
    QRegularExpression re(R"(cses\.fi/(problemset|contest/(\d+))/(problem|task)/(\d+))");
    auto match = re.match(url);
    if (match.hasMatch())
    {
        if (match.captured(1) == "problemset")
            scope = "problemset";
        else
            scope = match.captured(2);
        taskId = match.captured(4);
        return true;
    }
    return false;
}

} // namespace Extensions

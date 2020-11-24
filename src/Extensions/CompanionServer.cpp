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

#include "Extensions/CompanionServer.hpp"
#include "Core/EventLogger.hpp"
#include "Core/MessageLogger.hpp"
#include "third_party/qhttp/src/qhttpfwd.hpp"
#include "third_party/qhttp/src/qhttpserver.hpp"
#include "third_party/qhttp/src/qhttpserverconnection.hpp"
#include "third_party/qhttp/src/qhttpserverrequest.hpp"
#include "third_party/qhttp/src/qhttpserverresponse.hpp"
#include <QJsonArray>
#include <QJsonObject>

#define USER_INFO(x)                                                                                                   \
    if (log)                                                                                                           \
        log->info("Companion", x);

#define USER_WARN(x)                                                                                                   \
    if (log)                                                                                                           \
        log->warn("Companion", x);

#define USER_ERR(x)                                                                                                    \
    if (log)                                                                                                           \
        log->error("Companion", x);

namespace Extensions
{
CompanionServer::CompanionServer(int port, QObject *parent) : QObject(parent)
{
    if (startListeningOn(port))
    {
        lastListeningPort = port;
    }
}

void CompanionServer::setMessageLogger(MessageLogger *log)
{
    this->log = log;
}

bool CompanionServer::startListeningOn(int port)
{
    if (server)
        delete server;
    server = new qhttp::server::QHttpServer(this);
    server->listen(QString::number(port), [this](qhttp::server::QHttpRequest *req, qhttp::server::QHttpResponse *res) {
        LOG_INFO("\n--> " << req->methodString() << " : " << qPrintable(req->url().toString().toUtf8()));
        req->headers().forEach(
            [](auto iter) { LOG_INFO(iter.key().constData() << " : " << iter.value().constData()); });

        const QString methodType = req->methodString();
        const bool isJson = req->headers().keyHasValue("content-type", "application/json");
        req->collectData();

        req->onEnd([=] {
            res->addHeader("connection", "close");
            res->addHeader("pragma", "no-cache");

            if (methodType != "POST")
            {
                USER_WARN(tr("A %1 request is received and ignored").arg(methodType));
            }
            else if (!isJson)
            {
                USER_ERR(tr("The request received is not JSON"));
            }
            else
            {
                res->setStatusCode(qhttp::ESTATUS_ACCEPTED);
                QByteArray data = req->collectedData();
                LOG_INFO(INFO_OF(data));
                res->end();
                parseAndEmit(data);
                return;
            }

            res->setStatusCode(qhttp::ESTATUS_NOT_ACCEPTABLE);
            res->end();
        });
    });
    return server->isListening();
}

void CompanionServer::updatePort(int port)
{
    LOG_INFO(INFO_OF(port));
    if (port == 0) // Close the server
    {
        if (server)
            delete server;
        server = nullptr;
        USER_INFO(tr("Server is closed"));
        lastListeningPort = -1;
        return;
    }

    if (lastListeningPort != port) // actually update the port. This involves restarting the qhttpserver
    {
        bool started = startListeningOn(port);
        if (started)
        {
            lastListeningPort = port;
            USER_INFO(tr("Port is set to %1").arg(port));
        }
        else
        {
            USER_ERR(tr("Failed to listen to port %1. Is there another process listening?").arg(port));
        }
    }
}

void CompanionServer::parseAndEmit(QByteArray &data)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(QString(data).toUtf8(), &error);
    if (error.error == QJsonParseError::NoError)
    {
        CompanionData payload;

        payload.doc = doc;
        payload.url = doc["url"].toString();
        payload.timeLimit = doc["timeLimit"].toInt();
        QJsonArray testArray = doc["tests"].toArray();

        for (auto tests : testArray)
        {
            auto in = tests.toObject()["input"].toString();
            auto out = tests.toObject()["output"].toString();
            payload.testcases.push_back({in, out});
        }

        emit onRequestArrived(payload);
    }
    else
    {
        USER_ERR(tr("JSON parser reported errors:\n%1").arg(error.errorString()));
        LOG_WARN("JSON parser reported error " << error.errorString());
    }
}

CompanionServer::~CompanionServer()
{
    USER_INFO(tr("Stopped Server"));
    if (server)
        delete server;
}

} // namespace Extensions

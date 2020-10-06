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
#include "qhttpfwd.hpp"
#include "qhttpserver.hpp"
#include "qhttpserverconnection.hpp"
#include "qhttpserverrequest.hpp"
#include "qhttpserverresponse.hpp"
#include <QJsonArray>
#include <QJsonObject>

#define SAFEILOG(x, y)                                                                                                 \
    if (log)                                                                                                           \
        log->info(x, y);

#define SAFEELOG(x, y)                                                                                                 \
    if (log)                                                                                                           \
        log->error(x, y);

#define SAFEWLOG(x, y)                                                                                                 \
    if (log)                                                                                                           \
        log->warn(x, y);

namespace Extensions
{
CompanionServer::CompanionServer(int port, QObject *parent) : QObject(parent)
{
    startListeningOn(port);
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
    server->listen(QString::number(port),
                   [&, this](qhttp::server::QHttpRequest *req, qhttp::server::QHttpResponse *res) {
                       LOG_INFO("\n--> " << req->methodString() << " : " << qPrintable(req->url().toString().toUtf8()));
                       req->headers().forEach(
                           [](auto iter) { LOG_INFO(iter.key().constData() << " : " << iter.value().constData()); });

                       QString methodType = req->methodString();
                       bool isJson = req->headers().keyHasValue("content-type", "application/json");
                       req->collectData();

                       req->onEnd([res, req, methodType, isJson, this]() {
                           res->addHeader("connection", "close");
                           res->addHeader("pragma", "no-cache");

                           if (methodType != "POST")
                           {
                               SAFEWLOG("Companion", tr("Discarded %1 request").arg(methodType));
                               res->setStatusCode(qhttp::ESTATUS_NOT_ACCEPTABLE);
                               res->end();
                               return;
                           }

                           if (!isJson)
                           {
                               SAFEELOG("Companion", tr("Competitive Companion sent an unknown application/data type"));
                               res->setStatusCode(qhttp::ESTATUS_NOT_ACCEPTABLE);
                               res->end();
                               return;
                           }

                           res->setStatusCode(qhttp::ESTATUS_ACCEPTED);
                           QByteArray data = req->collectedData();
                           LOG_INFO(data)
                           res->end();

                           parseAndEmit(data);
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
        SAFEILOG("Companion", tr("Server is closed"));
        lastListeningPort = -1;
        return;
    }

    if (lastListeningPort != port) // actually update the port. This involves restarting the qhttpserver
    {
        bool started = startListeningOn(port);
        if (started)
        {
            lastListeningPort = port;
            SAFEILOG("Companion", tr("Port is set to %1").arg(port));
        }
        else
        {
            SAFEELOG("Companion", tr("Failed to listen to port %1. Is there another process listening?").arg(port));
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
        SAFEELOG("Companion", tr("JSON parser reported errors:\n%1").arg(error.errorString()));
        LOG_WARN("JSON parser reported error " << error.errorString());
    }
}

CompanionServer::~CompanionServer()
{
    SAFEILOG("Companion", tr("Stopped Server"));
    if (server)
        delete server;
}

} // namespace Extensions

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
#include <QJsonArray>
#include <QJsonObject>
#include <QTcpServer>
#include <QTcpSocket>

namespace Extensions
{
CompanionServer::CompanionServer(int port)
{
    updatePort(port);
}

void CompanionServer::setMessageLogger(MessageLogger *log)
{
    this->log = log;
}

void CompanionServer::updatePort(int port)
{
    LOG_INFO(INFO_OF(port));
    if (portNumber != port)
    {
        portNumber = port;
        if (server != nullptr)
            delete server;
        if (port == 0)
        {
            server = nullptr;
            if (log != nullptr)
                log->info("Companion", tr("Server is closed"));
        }
        else
        {
            server = new QTcpServer(this);
            portNumber = port;
            // server->setMaxPendingConnections(1);
            connect(server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
            server->listen(QHostAddress::LocalHost, static_cast<quint16>(port));
            if (log != nullptr)
            {
                log->info("Companion", tr("Port is set to %1").arg(port));
                if (!server->isListening())
                {
                    log->error("Companion",
                               tr("Failed to listen to port %1. Is there another process listening?").arg(portNumber));
                }
            }
        }
    }
}

CompanionServer::~CompanionServer()
{
    if (log != nullptr)
        log->info("Companion", tr("Stopped Server"));
    delete server;
}

void CompanionServer::onNewConnection()
{
    LOG_INFO("New connection has arrived, reading from queue");
    socket = server->nextPendingConnection();
    socket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(onReadReady()));
    QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(onTerminateConnection()));
}

void CompanionServer::onReadReady()
{
    QString request = socket->readAll();

    LOG_INFO("Connection content is\n" << request);

    if (request.startsWith("POST") && request.contains("Content-Type: application/json"))
    {
        if (log != nullptr)
            log->info("Companion", tr("Got a POST Request"));

        socket->write("HTTP/1.1  OK\r\n"); // \r needs to be before \n
        socket->write("Content-Type: text/html\r\n");
        socket->write("Connection: close\r\n");
        socket->write("Pragma: no-cache\r\n");
        socket->write("\r\n");
        socket->write("<!DOCTYPE html>\r\n");
        socket->write("<html><body><h1>Okay, Accepted"
                      "</h1></body></html>");
        socket->write(QByteArray());
        socket->disconnectFromHost();

        request = request.mid(request.indexOf('{'));

        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(request.toUtf8(), &error);
        if (error.error == QJsonParseError::NoError)
        {
            CompanionData payload;

            payload.doc = doc;

            payload.url = doc["url"].toString();

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
            if (log != nullptr)
                log->error("Companion", tr("JSON parser reported errors:\n%1").arg(error.errorString()));
            LOG_WARN("JSON parser reported error " << error.errorString());
        }
    }
    else
    {
        if (log != nullptr)
            log->warn("Companion", "An Invalid Payload was delivered on the listening port");
        else
            LOG_WARN("Invalid payload delivered on listing port");
        socket->write("HTTP/1.1  OK\r\n"); // \r needs to be before \n
        socket->write("Content-Type: text/html\r\n");
        socket->write("Connection: close\r\n");
        socket->write("Pragma: no-cache\r\n");
        socket->write("\r\n");
        socket->write("<!DOCTYPE html>\r\n");
        socket->write("<html><body><h1>Rejected!!! Only POST requests with Content-type JSON "
                      "are allowed "
                      "here.</h1></body></html>");
        socket->write(QByteArray());
        socket->disconnectFromHost();
    }
}
void CompanionServer::onTerminateConnection()
{
    LOG_INFO("Socket scheduled to be Deleted");
    socket->deleteLater();
}

} // namespace Extensions

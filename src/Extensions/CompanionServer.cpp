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
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

namespace Network
{
CompanionServer::CompanionServer(int port)
{
    updatePort(port);
}

void CompanionServer::setMessageLogger(MessageLogger *log)
{
    Core::Log::i("companionServer/setMessageLogger") << "Message logger is " << BOOLEAN(log == nullptr) << endl;
    this->log = log;
}

void CompanionServer::updatePort(int port)
{
    Core::Log::i("companionServer/updatePort") << INFO_OF(port) << endl;
    if (portNumber != port)
    {
        portNumber = port;
        if (server != nullptr)
            delete server;
        if (port == 0)
        {
            server = nullptr;
            if (log != nullptr)
                log->info("Companion", "Server is closed");
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
                log->info("Companion", "Port is set to " + QString::number(port));
                if (!server->isListening())
                {
                    log->error("Companion", "Failed to listen to port " + QString::number(portNumber) +
                                                ". Is there another process listening?");
                }
            }
        }
    }
    Core::Log::i("companionServer/updatePort", "Finished");
}

CompanionServer::~CompanionServer()
{
    Core::Log::i("companionServer/destroyed", "killing companion server");
    if (log != nullptr)
        log->info("Companion", "Stopped Server");
    delete server;
}

void CompanionServer::onNewConnection()
{
    Core::Log::i("companionServer/onNewConnection", "New connection has arrived, reading from the queue");
    socket = server->nextPendingConnection();
    socket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(onReadReady()));
    QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(onTerminateConnection()));
}

void CompanionServer::onReadReady()
{
    QString request = socket->readAll();

    Core::Log::i("companionServer/onReadReady") << "\n" << request << endl;

    if (request.startsWith("POST") && request.contains("Content-Type: application/json"))
    {
        if (log != nullptr)
            log->info("Companion", "Got a POST Request");

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

        Core::Log::i("companionServer/onReadReady") << "Truncated request is : \n" << request << endl;

        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(request.toUtf8(), &error);
        if (error.error == QJsonParseError::NoError)
        {
            CompanionData payload;

            payload.name = doc["name"].toString();
            payload.contest = doc["group"].toString();
            payload.url = doc["url"].toString();
            payload.interactive = doc["interactive"].toBool();
            payload.memoryLimit = doc["memoryLimit"].toInt();
            payload.timeLimit = doc["timeLimit"].toInt();
            payload.isInputstdin = doc["input"].toObject()["type"].toString() == "stdin";
            payload.isOutputstdout = doc["output"].toObject()["type"].toString() == "stdout";

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
                log->error("Companion", "JSONParser reported errors. \n" + error.errorString());
            else
                Core::Log::i("companionServer/onReadReady")
                    << "JSON Parser reported error " << error.errorString() << endl;
        }
    }
    else
    {
        if (log != nullptr)
            log->warn("Companion", "An Invalid Payload was delivered on the listening port");
        else
            Core::Log::w("companionServer/onReadReady", "Invalid payload delivered on listing port");
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
    Core::Log::i("companionServer/onTerminateConnection", "socket scheduled to be deleted");
    socket->deleteLater();
}

} // namespace Network

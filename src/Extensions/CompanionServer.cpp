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
    Core::Log::i("companionServer/constructed") << "port is : " << port << endl;
    server = new QTcpServer(this);
    portNumber = port;
    // server->setMaxPendingConnections(1);
    QObject::connect(server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
    server->listen(QHostAddress::LocalHost, static_cast<unsigned short>(port));
    Core::Log::i("companionServer/constructed")
        << "server is listening to url :" << server->serverAddress().toString() << endl;
}

void CompanionServer::setMessageLogger(MessageLogger *log)
{
    Core::Log::i("companionServer/setMessageLogger") << "log is null ? : " << (log == nullptr) << endl;
    this->log = log;
}

void CompanionServer::checkServer()
{
    if (log != nullptr)
    {

        if (server->serverPort() == 0)
        {
            log->error("Companion", "Failed to listen to specified "
                                    " port. Is another process listening there?");
        }
        else
        {
            log->info("Companion", "Listening for requests on " + server->serverAddress().toString() + ":" +
                                       QString::number(server->serverPort()));
        }
    }
    else
    {
        Core::Log::w("companionServer/checkServer", "Failure in logging the status, log is nullptr");
        Core::Log::i("companionServer/checkServer")
            << "server listening on : " << server->serverAddress().toString() << ":" << server->serverPort() << endl;
    }
}

void CompanionServer::updatePort(int port)
{
    Core::Log::i("companionServer/updatePort") << "new port is " << port << endl;
    this->portNumber = port;
    delete server;
    server = new QTcpServer(this);
    portNumber = port;
    // server->setMaxPendingConnections(1);
    QObject::connect(server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
    server->listen(QHostAddress::LocalHost, static_cast<unsigned short>(port));
    if (log != nullptr)
        log->warn("Companion", "Port changed to " + QString::number(port));
    Core::Log::i("companionServer/updatePort", "Connection was reset and old TCP Server was destroyed");
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

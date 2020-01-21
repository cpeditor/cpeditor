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

#include <CompanionServer.hpp>
#include <MessageLogger.hpp>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

namespace Network
{
CompanionServer::CompanionServer(int port)
{
    server = new QTcpServer(this);
    portNumber = port;
    // server->setMaxPendingConnections(1);
    QObject::connect(server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
    server->listen(QHostAddress::LocalHost, static_cast<unsigned short>(port));
}

void CompanionServer::setMessageLogger(MessageLogger *log)
{
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
}

void CompanionServer::updatePort(int port)
{
    this->portNumber = port;
    delete server;
    server = new QTcpServer(this);
    portNumber = port;
    // server->setMaxPendingConnections(1);
    QObject::connect(server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
    server->listen(QHostAddress::LocalHost, static_cast<unsigned short>(port));
    if (log != nullptr)
        log->warn("Companion", "Port changed to " + QString::number(port));
}

CompanionServer::~CompanionServer()
{
    if (log != nullptr)
        log->info("Companion", "Stopped Server");
    delete server;
}

void CompanionServer::onNewConnection()
{
    socket = server->nextPendingConnection();
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(onReadReady()));
    QObject::connect(socket, SIGNAL(disconnected()), this, SLOT(onTerminateConnection()));
}
void CompanionServer::onReadReady()
{
    QString request = socket->readAll();

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

        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(request.toUtf8(), &error);
        if (error.error == QJsonParseError::NoError)
        {
            CompanionData payload;

            payload.name = doc["name"].toString();
            payload.contest = doc["group"].toString();
            payload.url = doc["url"].toString();
            payload.interactive = doc["interactive"].toBool();
            payload.memoryLimit = doc["memoryLimit"].toDouble();
            payload.timeLimit = doc["timeLimit"].toDouble();
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
        }
    }
    else
    {
        if (log != nullptr)
            log->warn("Companion", "An Invalid Payload was delivered on the listening port");
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
    socket->deleteLater();
}

} // namespace Network

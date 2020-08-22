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

#ifndef COMPANIONSERVER_HPP
#define COMPANIONSERVER_HPP

#include <QJsonDocument>
#include <QObject>
#include <QVector>

class MessageLogger;
class QTcpServer;
class QTcpSocket;

namespace Extensions
{

struct CompanionData
{
    struct TestCases
    {
        QString input;
        QString output;
    };

    QString url;
    int timeLimit;
    QJsonDocument doc;
    QVector<TestCases> testcases;
};

class CompanionServer : public QObject
{
    Q_OBJECT

  public:
    explicit CompanionServer(int port);
    void setMessageLogger(MessageLogger *log);

    void updatePort(int port);
    ~CompanionServer();

  signals:
    void onRequestArrived(const CompanionData &data);

  private slots:
    void onNewConnection();
    void onTerminateConnection();
    void onReadReady();

  private:
    QTcpServer *server = nullptr;
    QTcpSocket *socket = nullptr;
    int portNumber = 0;
    MessageLogger *log = nullptr;
};
} // namespace Extensions
#endif // COMPANIONSERVER_HPP

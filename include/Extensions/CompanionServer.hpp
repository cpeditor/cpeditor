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

#ifndef COMPANIONSERVER_HPP
#define COMPANIONSERVER_HPP
#include "Core/MessageLogger.hpp"
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

namespace Network
{

struct CompanionData
{
    struct TestCases
    {
        QString input;
        QString output;
    };

    QString name;
    QString contest;
    QString url;
    bool interactive;
    int memoryLimit;
    int timeLimit;
    bool isInputstdin;
    bool isOutputstdout;
    QVector<TestCases> testcases;

    QString toMetaString() const
    {
        QString ans;
        ans += "Problem : " + name + "\n";
        ans += "Contest : " + contest + "\n";
        ans += "URL : " + url + "\n";
        ans += "Memory Limit : " + QString::number(memoryLimit) + " MB\n";
        ans += "Time Limit : " + QString::number(timeLimit) + " ms\n";
        return ans;
    }
};

class CompanionServer : public QObject
{
    Q_OBJECT

  public:
    CompanionServer(int port);
    void setMessageLogger(MessageLogger *log);
    void checkServer();

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
    int portNumber;
    MessageLogger *log = nullptr;
};
} // namespace Network
#endif // COMPANIONSERVER_HPP

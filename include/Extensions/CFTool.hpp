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
 * causes your ratings to go down and or lose any important contest.
 *
 * Believe Software is "Software" and it isn't immune to bugs.
 *
 */

#ifndef CFTOOL_HPP
#define CFTOOL_HPP

#include "Core/MessageLogger.hpp"
#include <QProcess>
#include <QRegularExpression>
#include <QSystemTrayIcon>

namespace Network
{
class CFTool : public QObject
{
    Q_OBJECT

  public:
    CFTool(const QString &path, MessageLogger *logger);
    ~CFTool();
    void submit(const QString &filePath, const QString &url, const QString &lang);
    static bool check(const QString &path);
    void updatePath(const QString &p);

  private slots:
    void onReadReady();
    void onFinished(int exitCode);

  private:
    void showToastMessage(const QString &message);

    QString problemContestId, problemCode, lastStatus;
    QProcess *CFToolProcess = nullptr;
    QSystemTrayIcon *icon = nullptr;
    MessageLogger *log;
    QString CFToolPath;
};
} // namespace Network

#endif // CFTOOL_HPP

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

#ifndef CFTOOL_HPP
#define CFTOOL_HPP

#include <QObject>
#include <QProcess>

class MessageLogger;

namespace Extensions
{
class CFTool : public QObject
{
    Q_OBJECT

  public:
    CFTool(const QString &path, MessageLogger *logger);
    ~CFTool() override;
    void submit(const QString &filePath, const QString &url);
    static bool check(const QString &path);
    void updatePath(const QString &p);
    static bool parseCfUrl(const QString &url, QString &contestId, QString &problemCode);

  signals:
    void requestToastMessage(const QString &head, const QString &body);

  private slots:
    void onReadReady();
    void onFinished(int exitCode, QProcess::ExitStatus);

  private:
    QString problemContestId, problemCode, lastStatus;
    QProcess *CFToolProcess = nullptr;
    MessageLogger *log;
    QString CFToolPath;

    void showToastMessage(const QString &message);
    QString getCFToolVersion() const;
};
} // namespace Extensions

#endif // CFTOOL_HPP

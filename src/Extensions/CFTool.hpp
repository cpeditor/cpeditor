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
    void updatePath(const QString &p);

  signals:
    void requestToastMessage(const QString &head, const QString &body);

  private slots:
    void onReadReady();
    void onFinished(int exitCode, QProcess::ExitStatus);

  private:
    QString lastStatus, lastUrl;
    QProcess *process = nullptr;
    MessageLogger *log;
    QString path;

    void showToastMessage(const QString &message);
    QString getCFToolVersion() const;
};
} // namespace Extensions

#endif // CFTOOL_HPP

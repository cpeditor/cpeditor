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

#ifndef RUNNER_HPP
#define RUNNER_HPP

#include <QElapsedTimer>
#include <QProcess>
#include <QTimer>

namespace Core
{

class Runner : public QObject
{
    Q_OBJECT

  public:
    explicit Runner(int index);
    ~Runner();
    void run(const QString &filePath, const QString &lang, const QString &runCommand, const QString &args,
             const QString &input, int timeLimit);
    void runDetached(const QString &filePath, const QString &lang, const QString &runCommand, const QString &args);

  signals:
    void runStarted(int index);
    void runFinished(int index, const QString &out, const QString &err, int exitCode, int timeUsed);
    void runErrorOccured(int index, const QString &error);
    void runTimeout(int index);
    void runKilled(int index);

  private slots:
    void onFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onStarted();
    void onTimeout();

  private:
    const int runnerIndex;
    QString runCommand;
    QProcess *runProcess = nullptr;
    QTimer *killTimer = nullptr;
    QElapsedTimer *runTimer = nullptr;

    QString getCommand(const QString &filePath, const QString &lang, const QString &runCommand, const QString &args);
};

} // namespace Core

#endif // RUNNER_HPP
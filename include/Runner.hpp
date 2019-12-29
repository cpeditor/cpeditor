/*
 * Copyright (C) 2019 Ashar Khan <ashar786khan@gmail.com>
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
 * Believe Software is "Software" and it isn't not immune to bugs.
 *
 */

#ifndef RUNNER_HPP
#define RUNNER_HPP

#include <Core.hpp>
#include <IO.hpp>
#include <QCodeEditor>
#include <QObject>
#include <QProcess>
namespace Core
{
class Runner : public QObject, private Base::Files
{
    Q_OBJECT
  public:
    Runner(QString runCommand, QString compileCommand, QString startRunCommand, int index, MessageLogger* log);
    ~Runner();

    void run(QCodeEditor *editor, QVector<bool> _isRun, QString lang = "Cpp");
    void run(QVector<bool> _isRun, QString lang = "Cpp");

    void runDetached(QCodeEditor *editor, QString lang = "Cpp");

    void removeExecutable();
    void updateRunCommand(QString newCommand);
    void updateCompileCommand(QString newCommand);
    void updateRunStartCommand(QString newCommand);

    void killAll();
  private slots:
    void compilationFinished(bool success);

    void runError(int, QProcess::ProcessError);
    void runFinished(int, int, QProcess::ExitStatus);
    void runStarted(int);

  signals:
    void executionFinished(int, int, QString);

  private:
    QString runCommand;
    QString startRunCommand;
    QString language;
    Core::Compiler *compiler = nullptr;
    QVector<bool> isRun = QVector<bool>(3, false);
    bool detached = false;
    QProcess *detachedHandle = nullptr;
    MessageLogger* log;
    QVector<QProcess *> runner = QVector<QProcess *>(3, nullptr);
    QVector<QTime *> timers = QVector<QTime *>(3, nullptr);
};

} // namespace Core
#endif // RUNNER_HPP

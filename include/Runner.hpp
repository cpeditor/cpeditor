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
namespace Core {
class Runner : public QObject, private Base::Files {
  Q_OBJECT
 public:
  Runner(QString runCommand, QString compileCommand);
  ~Runner();

  void run(QCodeEditor* editor,
           bool runA,
           bool runB,
           bool runC,
           QString lang = "Cpp");
  void run(bool runA, bool runB, bool runC, QString lang = "Cpp");

  void runDetached(QCodeEditor* editor, QString lang = "Cpp");

  void removeExecutable();
  void updateRunCommand(QString newCommand);
  void updateCompileCommand(QString newCommand);

  void killAll();
 private slots:
  void compilationFinished(bool success);

  void firstError(QProcess::ProcessError);
  void firstFinished(int, QProcess::ExitStatus);
  void firstStarted();

  void secondError(QProcess::ProcessError);
  void secondFinished(int, QProcess::ExitStatus);
  void secondStarted();

  void thirdError(QProcess::ProcessError);
  void thirdFinished(int, QProcess::ExitStatus);
  void thirdStarted();

 signals:
  void firstExecutionFinished(QString, QString);
  void secondExecutionFinished(QString, QString);
  void thirdExecutionFinished(QString, QString);

 private:
  QString runCommand;
  QString getLatestModifiedBinaryLang();
  Core::Compiler* compiler = nullptr;
  bool a_ = false, b_ = false, c_ = false, detached = false;
  QProcess *first = nullptr, *second = nullptr, *third = nullptr,
           *detachedHandle = nullptr;
};

}  // namespace Core
#endif  // RUNNER_HPP

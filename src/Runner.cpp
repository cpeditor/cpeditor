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

#include <MessageLogger.hpp>
#include <QTimer>
#include <Runner.hpp>
namespace Core {

Runner::Runner(QString runCommand, QString compileCommand) {
  this->runCommand = runCommand;
  compiler = new Core::Compiler(compileCommand);
  QObject::connect(compiler, SIGNAL(compilationFinished(bool)), this,
                   SLOT(compilationFinished(bool)));
}

Runner::~Runner() {
  if (first != nullptr)
    delete first;
  if (second != nullptr)
    delete second;
  if (third != nullptr)
    delete third;
  delete compiler;
}

void Runner::updateRunCommand(QString newCommand) {
  this->runCommand = newCommand;
}

void Runner::updateCompileCommand(QString newCommand) {
  this->compiler->updateCommand(newCommand);
}

void Runner::run(QCodeEditor* editor, bool runA, bool runB, bool runC) {
  if (first != nullptr) {
    first->kill();
    delete first;
    first = nullptr;
  }
  if (second != nullptr) {
    second->kill();
    delete second;
    second = nullptr;
  }
  if (third != nullptr) {
    third->kill();
    delete third;
    third = nullptr;
  }
  a_ = runA;
  b_ = runB;
  c_ = runC;
  compiler->compile(editor);
}

void Runner::run(bool runA, bool runB, bool runC) {
  if (first != nullptr) {
    first->kill();
    delete first;
    first = nullptr;
  }
  if (second != nullptr) {
    second->kill();
    delete second;
    second = nullptr;
  }
  if (third != nullptr) {
    third->kill();
    delete third;
    third = nullptr;
  }
  a_ = runA;
  b_ = runB;
  c_ = runC;

  if (QFile::exists(getBinaryOutput())) {
    Log::MessageLogger::info("Runner", "Reusuing executable");
    compilationFinished(true);
  } else {
    Log::MessageLogger::error(
        "Runner",
        "Cannot run, have you successfully compiled your code earlier?");
  }
}

void Runner::removeExecutable() {
  if (QFile::exists(getBinaryOutput()))
    QFile::remove(getBinaryOutput());
}

void Runner::compilationFinished(bool success) {
  if (success) {
    if (a_) {
      first = new QProcess();

      QTimer* killtimer = new QTimer(first);
      killtimer->setSingleShot(true);
      killtimer->setInterval(5000);
      QObject::connect(killtimer, SIGNAL(timeout()), first, SLOT(terminate()));

      first->setProgram(getBinaryOutput());
      first->setStandardInputFile(getInputFirst());
      if (!runCommand.trimmed().isEmpty())
        first->setArguments(runCommand.trimmed().split(" "));
      // first->waitForFinished(5000);
      QObject::connect(first, SIGNAL(finished(int, QProcess::ExitStatus)), this,
                       SLOT(firstFinished(int, QProcess::ExitStatus)));
      QObject::connect(first, SIGNAL(errorOccurred(QProcess::ProcessError)),
                       this, SLOT(firstError(QProcess::ProcessError)));
      QObject::connect(first, SIGNAL(started(void)), this,
                       SLOT(firstStarted()));
      first->start();
      killtimer->start();
    }
    if (b_) {
      second = new QProcess();

      QTimer* killtimer2 = new QTimer(second);
      killtimer2->setSingleShot(true);
      killtimer2->setInterval(5000);
      QObject::connect(killtimer2, SIGNAL(timeout()), second, SLOT(terminate()));

      second->setProgram(getBinaryOutput());
      second->setStandardInputFile(getInputSecond());
      if (!runCommand.trimmed().isEmpty())
        second->setArguments(runCommand.trimmed().split(" "));
      // second->waitForFinished(5000);
      QObject::connect(second, SIGNAL(finished(int, QProcess::ExitStatus)),
                       this, SLOT(secondFinished(int, QProcess::ExitStatus)));
      QObject::connect(second, SIGNAL(errorOccurred(QProcess::ProcessError)),
                       this, SLOT(secondError(QProcess::ProcessError)));
      QObject::connect(second, SIGNAL(started(void)), this,
                       SLOT(secondStarted()));
      second->start();
      killtimer2->start();
    }
    if (c_) {
      third = new QProcess();
      QTimer* killtimer3 = new QTimer(third);
      killtimer3->setSingleShot(true);
      killtimer3->setInterval(5000);
      QObject::connect(killtimer3, SIGNAL(timeout()), third, SLOT(terminate()));
      third->setProgram(getBinaryOutput());
      third->setStandardInputFile(getInputThird());
      if (!runCommand.trimmed().isEmpty())
        third->setArguments(runCommand.trimmed().split(" "));
      // third->waitForFinished(5000);
      QObject::connect(third, SIGNAL(finished(int, QProcess::ExitStatus)), this,
                       SLOT(thirdFinished(int, QProcess::ExitStatus)));
      QObject::connect(third, SIGNAL(errorOccurred(QProcess::ProcessError)),
                       this, SLOT(thirdError(QProcess::ProcessError)));
      QObject::connect(third, SIGNAL(started(void)), this,
                       SLOT(thirdStarted()));
      third->start();
      killtimer3->start();
    }

  } else {
    Log::MessageLogger::warn(
        "Runner",
        "Compiler reported errors. No executables were produced to run");
  }
}

void Runner::firstError(QProcess::ProcessError e) {
  if (e == QProcess::ProcessError::Timedout)
    Log::MessageLogger::error("Runner[1]", "Time Limit of 5 sec reached");
  else
    Log::MessageLogger::error(
        "Runner[1]", "Error occurred during execution on first testcase");
}
void Runner::secondError(QProcess::ProcessError e) {
  if (e == QProcess::ProcessError::Timedout)
    Log::MessageLogger::error("Runner[1]", "Time Limit of 5 sec reached");
  else
    Log::MessageLogger::error(
        "Runner[2]", "Error occurred during execution on second testcase");
}
void Runner::thirdError(QProcess::ProcessError e) {
  if (e == QProcess::ProcessError::Timedout)
    Log::MessageLogger::error("Runner[1]", "Time Limit of 5 sec reached");
  else
    Log::MessageLogger::error(
        "Runner[3]", "Error occurred during execution on third testcase");
}

void Runner::firstFinished(int exitCode, QProcess::ExitStatus exitStatus) {
  if (exitCode == 0 && exitStatus == QProcess::ExitStatus::NormalExit) {
    emit firstExecutionFinished(
        QString::fromLocal8Bit(first->readAllStandardOutput()),
        QString::fromLocal8Bit(first->readAllStandardError()));
  } else if (exitCode == 15) {
    // Sigterm is called by timeout timer;
    Log::MessageLogger::error("Runner[1]",
                              "Timeout 5 sec, your program didn't returned");
  } else {
    Log::MessageLogger::error("Runner[1]",
                              "Non-zero exit code " + std::to_string(exitCode));
  }
}
void Runner::secondFinished(int exitCode, QProcess::ExitStatus exitStatus) {
  if (exitCode == 0 && exitStatus == QProcess::ExitStatus::NormalExit) {
    emit secondExecutionFinished(
        QString::fromLocal8Bit(second->readAllStandardOutput()),
        QString::fromLocal8Bit(second->readAllStandardError()));
  } else if (exitCode == 15) {
    // Sigterm is called by timeout timer;
    Log::MessageLogger::error("Runner[2]",
                              "Timeout 5 sec, your program didn't returned");
  } else {
    Log::MessageLogger::error("Runner[2]",
                              "Non-zero exit code " + std::to_string(exitCode));
  }
}
void Runner::thirdFinished(int exitCode, QProcess::ExitStatus exitStatus) {
  if (exitCode == 0 && exitStatus == QProcess::ExitStatus::NormalExit) {
    emit thirdExecutionFinished(
        QString::fromLocal8Bit(third->readAllStandardOutput()),
        QString::fromLocal8Bit(third->readAllStandardError()));
  } else if (exitCode == 15) {
    // Sigterm is called by timeout timer;
    Log::MessageLogger::error("Runner[3]",
                              "Timeout 5 sec, your program didn't returned");
  } else {
    Log::MessageLogger::error("Runner[3]",
                              "Non-zero exit code " + std::to_string(exitCode));
  }
}

void Runner::firstStarted() {
  Log::MessageLogger::info("Runner[1]", "Started execution");
}
void Runner::secondStarted() {
  Log::MessageLogger::info("Runner[2]", "Started execution");
}
void Runner::thirdStarted() {
  Log::MessageLogger::info("Runner[3]", "Started execution");
}
}  // namespace Core

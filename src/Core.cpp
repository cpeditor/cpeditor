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


#include <Core.hpp>
#include <MessageLogger.hpp>
#include <QProcess>

namespace Core {
Compiler::Compiler(QString command) {
  runCommand = command;
  file = new QFile(getProgramFile());
  file->open(QIODevice::ReadWrite | QFile::Text);
  if (!file->isOpen()) {
    Log::MessageLogger::error("Compiler",
                              "Cannot create temporary compilation file");
  } else {
    file->resize(0);
  }
}

Compiler::~Compiler() {
  if (file->isOpen())
    file->close();
  delete file;
  if (compilationProcess != nullptr)
    delete compilationProcess;
}

void Compiler::compile(QCodeEditor* editor) {
  if (compilationProcess != nullptr) {
    compilationProcess->kill();
    delete compilationProcess;  // calls sigkill
    compilationProcess = nullptr;
  }

  if (!file->isOpen())
    file->open(QIODevice::ReadWrite | QFile::Text);

  Log::MessageLogger::info("Compiler", "Compilation requested");
  file->resize(0);
  file->write(editor->toPlainText().toUtf8().toStdString().c_str());
  file->close();

  compilationProcess = new QProcess();

  auto lists = runCommand.trimmed().split(" ");
  compilationProcess->setProgram(lists[0]);
  lists.removeAt(0);
  lists.append(getProgramFile());
  lists.append("-o");
  lists.append(getBinaryOutput());
  compilationProcess->setArguments(lists);

  QObject::connect(compilationProcess,
                   SIGNAL(finished(int, QProcess::ExitStatus)), this,
                   SLOT(finished(int, QProcess::ExitStatus)));

  QObject::connect(compilationProcess,
                   SIGNAL(errorOccurred(QProcess::ProcessError)), this,
                   SLOT(errorOccurred(QProcess::ProcessError)));

  QObject::connect(compilationProcess, SIGNAL(started(void)), this,
                   SLOT(started(void)));

  compilationProcess->start();
}

void Compiler::updateCommand(QString newCommand) {
  runCommand = newCommand;
}

bool Compiler::check(QString comm) {
  auto lst = comm.trimmed().split(" ");
  auto com = lst[0] + " --version";
  auto status = std::system(com.toStdString().c_str());
  return status == 0;
}

void Compiler::started() {
  Log::MessageLogger::info("Compiler", "Compilation has started...");
  emit compilationStarted();
}

void Compiler::errorOccurred(QProcess::ProcessError e) {
  switch (e) {
    case QProcess::ProcessError::Crashed:
      Log::MessageLogger::error("Compiler", "Compiler process crashed");
      break;
    case QProcess::ProcessError::Timedout:
      Log::MessageLogger::error("Compiler", "Compiler process timedout");
      break;
    case QProcess::ProcessError::ReadError:
      Log::MessageLogger::error(
          "Compiler",
          "Compiler process readError. Is compiler command correct?");
      break;
    case QProcess::ProcessError::WriteError:
      Log::MessageLogger::error("Compiler", "Compiler process writeError");
      break;
    case QProcess::ProcessError::UnknownError:
      Log::MessageLogger::error(
          "Compiler", "An unknown error caused the compiler process to abort");
      break;
    case QProcess::ProcessError::FailedToStart:
      Log::MessageLogger::error("Compiler",
                                "Failed to start the compiler process");
  }
  emit compilationError();
}

void Compiler::finished(int exitCode, QProcess::ExitStatus exitStatus) {
  if (exitStatus == QProcess::ExitStatus::NormalExit) {
    if (exitCode != 0)
      Log::MessageLogger::error(
          "Compiler",
          "Failed!! process returned exit code " + std::to_string(exitCode));
    else
      Log::MessageLogger::info(
          "Compiler",
          "Success!! process returned exit code " + std::to_string(exitCode));

  } else {
    Log::MessageLogger::error(
        "Compiler",
        "Crashed!! process returned exit code " + std::to_string(exitCode));
  }

  if (exitCode != 0 && compilationProcess != nullptr) {
    Log::MessageLogger::error(
        "Compiler Message",
        QString::fromLocal8Bit(compilationProcess->readAllStandardError())
            .toStdString(),
        true);
    emit compilationFinished(false);
  } else {
    emit compilationFinished(true);
  }
}

}  // namespace Core

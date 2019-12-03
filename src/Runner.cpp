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

Runner::Runner(QString runCommand,
               QString compileCommand,
               QString runCommandStart) {
  this->runCommand = runCommand;
  this->startRunCommand = runCommandStart;
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
  if (detachedHandle != nullptr)
    delete detachedHandle;
}

void Runner::updateRunCommand(QString newCommand) {
  this->runCommand = newCommand;
}

void Runner::updateCompileCommand(QString newCommand) {
  this->compiler->updateCommand(newCommand);
}

void Runner::updateRunStartCommand(QString newCommand) {
  this->startRunCommand = newCommand;
}

void Runner::killAll() {
  if (first != nullptr) {
    if (first->state() == QProcess::Running)
      Log::MessageLogger::info("Runner",
                               "Killed Program executing first testcase");
    first->kill();
    delete first;
    first = nullptr;
  }
  if (second != nullptr) {
    if (second->state() == QProcess::Running)
      Log::MessageLogger::info("Runner",
                               "Killed Program executing second testcase");
    second->kill();
    delete second;
    second = nullptr;
  }
  if (third != nullptr) {
    if (third->state() == QProcess::Running)
      Log::MessageLogger::info("Runner",
                               "Killed Program executing third testcase");
    third->kill();
    delete third;
    third = nullptr;
  }

  if (detachedHandle != nullptr) {
    if (detachedHandle->state() == QProcess::Running) {
      Log::MessageLogger::info("Detached", "Killing detached process");
    }
    detachedHandle->kill();
    delete detachedHandle;
    detachedHandle = nullptr;
  }
}

void Runner::run(QCodeEditor* editor,
                 bool runA,
                 bool runB,
                 bool runC,
                 QString lang) {
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

  this->language = lang;
  compiler->compile(editor, lang);
}

void Runner::run(bool runA, bool runB, bool runC, QString lang) {
  if (first != nullptr) {
    first->terminate();
    delete first;
    first = nullptr;
  }
  if (second != nullptr) {
    second->terminate();
    delete second;
    second = nullptr;
  }
  if (third != nullptr) {
    third->terminate();
    delete third;
    third = nullptr;
  }
  a_ = runA;
  b_ = runB;
  c_ = runC;

  this->language = lang;

  if (language == "Cpp" && QFile::exists(getBinaryOutput())) {
    Log::MessageLogger::info("Runner", "Reusuing executable");
    compilationFinished(true);
  } else if (language == "Python" && QFile::exists(getProgramFile(".py"))) {
    Log::MessageLogger::warn("Runner",
                             "Running last buffered script. To run current "
                             "script use Compile and Run");
    compilationFinished(true);
  } else if (language == "Java" && QFile::exists(getBinaryOutput(".class"))) {
    Log::MessageLogger::info("Runner", "Re-using same old class");
    compilationFinished(true);
  } else {
    Log::MessageLogger::error(
        "Runner",
        "Cannot run, Have you successfully compiled your code earlier?");
  }
}

void Runner::runDetached(QCodeEditor* editor, QString lang) {
#if defined(__unix__)
  if (std::system("xterm -version") != 0) {
    Log::MessageLogger::error(
        "Detached Execution",
        "This requires xterm to be installed on your system");
    return;
  }
#endif
  if (detachedHandle != nullptr) {
    if (detachedHandle->state() == QProcess::Running)
      Log::MessageLogger::info(
          "Detached Runner",
          "Already running a detached process. Killing it now");
    detachedHandle->kill();
    delete detachedHandle;
    detachedHandle = nullptr;
  }

  detached = true;
  this->language = lang;
  compiler->compile(editor, lang);
}

void Runner::removeExecutable() {
  if (QFile::exists(getBinaryOutput()))
    QFile::remove(getBinaryOutput());
  if (QFile::exists(getBinaryOutput(".class")))
    QFile::remove(getBinaryOutput(".class"));
}

void Runner::compilationFinished(bool success) {
  if (success) {
    if (detached) {
      detachedHandle = new QProcess();
#if defined(__unix__)
      detachedHandle->setProgram("xterm");
      QString command;
      QString runCommandLine;
      if (!runCommand.trimmed().isEmpty())
        runCommandLine = "," + runCommand.trimmed().replace(" ", ",");

      if (language == "Cpp")
        command = "-e," + getBinaryOutput() + runCommandLine +
                  "; echo '\nExecution Done\nPress any key to exit'; read";

      else if (language == "Python")
        command = "-e," + startRunCommand + "," + getProgramFile(".py") +
                  runCommandLine; /*+
                  "; echo '\nExecution Done\nPress any key to exit'; read";*/

      else if (language == "Java")
        command = "-e," + startRunCommand + ",-classpath," +
                  getBaseDirectory() + ",a" + runCommandLine; /* +
                   "; echo '\nExecution Done\nPress any key to exit'; read";*/

      else {
        Log::MessageLogger::error(
            "Language Error", "Execution language was not identified. Restart");
        return;
      }
      detachedHandle->setArguments(command.split(","));
#else
      detachedHandle->setProgram("cmd.exe");
      QString command;
      QString runCommandLine;
      if (!runCommand.trimmed().isEmpty())
        runCommandLine = "," + runCommand.trimmed().replace(" ", ",");

      if (language == "Cpp")
        command = "/C,start," + getBinaryOutput() + runCommandLine;
      else if (language == "Python")
        command = "/C,start," + startRunCommand + "," + getProgramFile(".py") +
                  runCommandLine;
      else if (language == "Java")
        command = "/C,start," + startRunCommand + ",-classpath," +
                  getBaseDirectory() + ",a" + runCommandLine;

      else {
        Log::MessageLogger::error(
            "Language Error", "Execution language was not identified. Restart");
        return;
      }
      detachedHandle->setArguments(command.split(","));

#endif
      detachedHandle->start();
      detached = false;
      return;
    }

    if (a_) {
      first = new QProcess();

      QTimer* killtimer = new QTimer(first);
      killtimer->setSingleShot(true);
      killtimer->setInterval(5000);
      QObject::connect(killtimer, SIGNAL(timeout()), first, SLOT(terminate()));

      QString args;

      if (language == "Cpp")
        first->setProgram(getBinaryOutput());
      else if (language == "Python" || language == "Java") {
        first->setProgram(startRunCommand);
        if (language == "Python")
          args += getProgramFile(".py");
        else
          args += "-classpath," + getBaseDirectory() + ",a";
      } else {
        Log::MessageLogger::error(
            "Language Error", "Execution language was not identified. Restart");
        return;
      }

      first->setStandardInputFile(getInputFirst());
      if (!runCommand.trimmed().isEmpty())
        args += "," + runCommand.trimmed().replace(" ", ",");

      first->setArguments(args.split(","));

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
      QObject::connect(killtimer2, SIGNAL(timeout()), second,
                       SLOT(terminate()));

      QString args;

      if (language == "Cpp")
        second->setProgram(getBinaryOutput());
      else if (language == "Python" || language == "Java") {
        second->setProgram(startRunCommand);
        if (language == "Python")
          args += getProgramFile(".py");
        else
          args += "-classpath," + getBaseDirectory() + ",a";
      } else {
        Log::MessageLogger::error(
            "Language Error", "Execution language was not identified. Restart");
        return;
      }

      second->setStandardInputFile(getInputSecond());
      if (!runCommand.trimmed().isEmpty())
        args += "," + runCommand.trimmed().replace(" ", ",");

      second->setArguments(args.split(","));
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
      QString args;

      if (language == "Cpp")
        third->setProgram(getBinaryOutput());
      else if (language == "Python" || language == "Java") {
        third->setProgram(startRunCommand);
        if (language == "Python")
          args += getProgramFile(".py");
        else
          args += "-classpath," + getBaseDirectory() + ",a";
      } else {
        Log::MessageLogger::error(
            "Language Error", "Execution language was not identified. Restart");
        return;
      }

      third->setStandardInputFile(getInputThird());
      if (!runCommand.trimmed().isEmpty())
        args += "," + runCommand.trimmed().replace(" ", ",");

      third->setArguments(args.split(","));

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
    auto stderrMsg =
        QString::fromLocal8Bit(first->readAllStandardError()).toStdString();

    Log::MessageLogger::error("Runner[1]",
                              "Non-zero exit code " + std::to_string(exitCode));
    if (!stderrMsg.empty())
      Log::MessageLogger::error("Runner[1]/STDERR", stderrMsg, true);
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
    auto stderrMsg =
        QString::fromLocal8Bit(second->readAllStandardError()).toStdString();

    Log::MessageLogger::error("Runner[2]",
                              "Non-zero exit code " + std::to_string(exitCode));
    if (!stderrMsg.empty())
      Log::MessageLogger::error("Runner[2]/STDERR", stderrMsg, true);
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
    auto stderrMsg =
        QString::fromLocal8Bit(third->readAllStandardError()).toStdString();
    Log::MessageLogger::error("Runner[3]",
                              "Non-zero exit code " + std::to_string(exitCode));
    if (!stderrMsg.empty())
      Log::MessageLogger::error("Runner[3]/STDERR", stderrMsg, true);
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

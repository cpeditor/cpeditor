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

#include <MessageLogger.hpp>
#include <QDateTime>
#include <QDebug>
#include <QTimer>
#include <Runner.hpp>
namespace Core
{

Runner::Runner(int index, MessageLogger *log) : Base::Files(index)
{
    this->log = log;
    compiler = new Core::Compiler("", "", index, log);
    QObject::connect(compiler, SIGNAL(compilationFinished(bool)), this, SLOT(compilationFinished(bool)));
}

Runner::~Runner()
{
    for (int i = 0; i < 3; ++i)
    {
        if (runner[i] != nullptr)
        {
            delete runner[i];
        }
    }
    delete compiler;
    if (detachedHandle != nullptr)
        delete detachedHandle;
}

void Runner::updateRunCommandPython(QString newCommand)
{
    this->runCommandPython = newCommand;
}
void Runner::updateRunCommandJava(QString newCommand)
{
    this->runCommandJava = newCommand;
}

void Runner::updateCompileCommandCpp(QString newCommand)
{
    this->compiler->updateCommandCpp(newCommand);
}
void Runner::updateCompileCommandJava(QString newCommand)
{
    this->compiler->updateCommandJava(newCommand);
}

void Runner::updateRuntimeArgumentsCommandCpp(QString newCommand)
{
    this->runtimeArgsCpp = newCommand;
}
void Runner::updateRuntimeArgumentsCommandPython(QString newCommand)
{
    this->runtimeArgsPython = newCommand;
}
void Runner::updateRuntimeArgumentsCommandJava(QString newCommand)
{
    this->runtimeArgsJava = newCommand;
}

void Runner::killAll()
{
    for (int i = 0; i < 3; ++i)
    {
        if (runner[i] != nullptr)
        {
            if (runner[i]->state() == QProcess::Running)
                log->info("Runner[" + std::to_string(i + 1) + "]",
                          "Program running on case #" + std::to_string(i + 1) + " is killed");
            runner[i]->kill();
            delete runner[i];
            runner[i] = nullptr;
        }
    }

    if (detachedHandle != nullptr)
    {
        if (detachedHandle->state() == QProcess::Running)
        {
            log->info("Detached", "Killing detached process");
        }
        detachedHandle->kill();
        delete detachedHandle;
        detachedHandle = nullptr;
    }
}

void Runner::run(QCodeEditor *editor, QVector<bool> _isRun, QString lang)
{
    for (int i = 0; i < 3; ++i)
    {
        if (runner[i] != nullptr)
        {
            runner[i]->kill();
            delete runner[i];
            runner[i] = nullptr;
        }
    }

    isRun = _isRun;

    this->language = lang;
    compiler->compile(editor, lang);
}

void Runner::run(QVector<bool> _isRun, QString lang)
{
    for (int i = 0; i < 3; ++i)
    {
        if (runner[i] != nullptr)
        {
            runner[i]->terminate();
            delete runner[i];
            runner[i] = nullptr;
        }
        if (timers[i] != nullptr)
        {
            delete timers[i];
            runner[i] = nullptr;
        }
    }

    isRun = _isRun;

    this->language = lang;

    if (language == "Cpp" && QFile::exists(getBinaryOutput()))
    {
        log->info("Runner", "Reusuing executable");
        compilationFinished(true);
    }
    else if (language == "Python" && QFile::exists(getProgramFile(".py")))
    {
        log->warn("Runner", "Running last buffered script. To run current "
                            "script use Compile and Run");
        compilationFinished(true);
    }
    else if (language == "Java" && QFile::exists(getBinaryOutput(".class")))
    {
        log->info("Runner", "Re-using same old class");
        compilationFinished(true);
    }
    else
    {
        log->error("Runner", "Cannot run, Have you successfully compiled your code earlier?");
    }
}

void Runner::runDetached(QCodeEditor *editor, QString lang)
{
#if defined(__unix__)
    if (std::system("xterm -version") != 0)
    {
        log->error("Detached Execution", "This requires xterm to be installed on your system");
        return;
    }
#endif
    if (detachedHandle != nullptr)
    {
        if (detachedHandle->state() == QProcess::Running)
            log->info("Detached Runner", "Already running a detached process. Killing it now");
        detachedHandle->kill();
        delete detachedHandle;
        detachedHandle = nullptr;
    }

    detached = true;
    this->language = lang;
    compiler->compile(editor, lang);
}

void Runner::removeExecutable()
{
    if (QFile::exists(getBinaryOutput()))
        QFile::remove(getBinaryOutput());
    if (QFile::exists(getBinaryOutput(".class")))
        QFile::remove(getBinaryOutput(".class"));
}

void Runner::compilationFinished(bool success)
{
    if (success)
    {
        if (detached)
        {
            detachedHandle = new QProcess();
#if defined(__unix__)
            detachedHandle->setProgram("xterm");
            QString command;

            if (language == "Cpp")
            {
                command = "-e," + getBinaryOutput() + runtimeArgsCpp.trimmed().replace(" ", ",") +
                          "; echo '\nExecution Done\nPress any key to exit'; read";
            }

            else if (language == "Python")
            {
                command = "-e," + runCommandPython.trimmed().replace(" ", ",") + "," + getProgramFile(".py") +
                          runtimeArgsPython.trimmed().replace(" ", ",") +
                          "; echo '\nExecution Done\nPress any key to exit'; read";
            }

            else if (language == "Java")
            {
                command = "-e," + runCommandJava.trimmed().replace(" ", ",") + ",-classpath," + getBaseDirectory() +
                          ",a" + runtimeArgsJava.trimmed().replace(" ", ",") +
                          "; echo '\nExecution Done\nPress any key to exit'; read";
            }

            else
            {
                log->error("Language Error", "Execution language was not identified. Restart");
                return;
            }
            detachedHandle->setArguments(command.split(","));
#else
            detachedHandle->setProgram("cmd.exe");
            QString command;

            if (language == "Cpp")
                command = "/C,start," + getBinaryOutput() + runtimeArgsCpp.trimmed().replace(" ", ",");
            else if (language == "Python")
                command = "/C,start," + runCommandPython.trimmed().replace(" ", ",") + "," + getProgramFile(".py") +
                          runtimeArgsPython.trimmed().replace(" ", ",");
            else if (language == "Java")
                command = "/C,start," + runCommandJava.trimmed().replace(" ", ",") + ",-classpath," +
                          getBaseDirectory() + ",a" + runtimeArgsJava.trimmed().replace(" ", ",");

            else
            {
                log->error("Language Error", "Execution language was not identified. Restart");
                return;
            }
            detachedHandle->setArguments(command.split(","));

#endif
            detachedHandle->start();
            detached = false;
            return;
        }

        for (int i = 0; i < 3; ++i)
        {
            if (isRun[i])
            {
                runner[i] = new QProcess();
                timers[i] = new QElapsedTimer();

                QTimer *killtimer = new QTimer(runner[i]);
                killtimer->setSingleShot(true);
                killtimer->setInterval(5000);
                QObject::connect(killtimer, SIGNAL(timeout()), runner[i], SLOT(terminate()));

                QList<QString> args;

                if (language == "Cpp")
                {
                    runner[i]->setProgram(getBinaryOutput());
                    args.append(runtimeArgsCpp.trimmed().split(" "));
                }
                else if (language == "Python")
                {
                    auto list = runCommandPython.trimmed();
                    runner[i]->setProgram(list);
                    args.append(getProgramFile(".py"));
                    args.append(runtimeArgsPython.split(" "));
                }
                else if (language == "Java")
                {
                    auto list = runCommandJava.trimmed();
                    runner[i]->setProgram(list);
                    args.append("-classpath");
                    args.append(getBaseDirectory());
                    args.append("a");
                    args.append(runtimeArgsJava.split(" "));
                }

                else
                {
                    log->error("Language Error", "Execution language was not identified. Restart");
                    return;
                }

                runner[i]->setStandardInputFile(getInput(i));
                runner[i]->setArguments(args);

                QObject::connect(
                    runner[i], QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
                    [this, i](int exitCode, QProcess::ExitStatus exitStatus) { runFinished(i, exitCode, exitStatus); });
                QObject::connect(runner[i], &QProcess::errorOccurred, this,
                                 [this, i](QProcess::ProcessError e) { runError(i, e); });
                QObject::connect(runner[i], &QProcess::started, this, [this, i]() { runStarted(i); });
                runner[i]->start();
                timers[i]->start();
                killtimer->start();
            }
        }
    }
    else
    {
        log->warn("Runner", "Compiler reported errors. No executables were produced to run");
    }
}

void Runner::runError(int id, QProcess::ProcessError e)
{
    if (e == QProcess::ProcessError::Timedout)
        log->error("Runner[" + std::to_string(id + 1) + "]", "Time Limit of 5 sec reached");
    else
        log->error("Runner[" + std::to_string(id + 1) + "]", "Error occurred during execution");
}

void Runner::runFinished(int id, int exitCode, QProcess::ExitStatus exitStatus)
{
    auto stderrMsg = QString::fromLocal8Bit(runner[id]->readAllStandardError()).toStdString();
    if (!stderrMsg.empty())
        log->error("Runner[" + std::to_string(id + 1) + "]/stderr", stderrMsg, true);
    if (exitCode == 0 && exitStatus == QProcess::ExitStatus::NormalExit)
    {
        emit executionFinished(id, timers[id]->elapsed(), QString::fromUtf8(runner[id]->readAllStandardOutput()));
    }
    else if (exitCode == 15)
    {
        // Sigterm is called by timeout timer;
        log->error("Runner[" + std::to_string(id + 1) + "]", "Timeout 5 sec, your program didn't returned");
    }
    else
    {
        log->error("Runner[" + std::to_string(id + 1) + "]", "Non-zero exit code " + std::to_string(exitCode));
    }
}

void Runner::runStarted(int id)
{
    log->info("Runner[" + std::to_string(id + 1) + "]", "Started execution");
}
} // namespace Core

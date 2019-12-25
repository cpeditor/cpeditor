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
namespace Core
{

Runner::Runner(QString runCommand, QString compileCommand, QString runCommandStart)
{
    this->runCommand = runCommand;
    this->startRunCommand = runCommandStart;
    compiler = new Core::Compiler(compileCommand);
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

void Runner::updateRunCommand(QString newCommand)
{
    this->runCommand = newCommand;
}

void Runner::updateCompileCommand(QString newCommand)
{
    this->compiler->updateCommand(newCommand);
}

void Runner::updateRunStartCommand(QString newCommand)
{
    this->startRunCommand = newCommand;
}

void Runner::killAll()
{
    for (int i = 0; i < 3; ++i)
    {
        if (runner[i] != nullptr)
        {
            if (runner[i]->state() == QProcess::Running)
                Log::MessageLogger::info("Runner[" + std::to_string(i + 1) + "]",
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
            Log::MessageLogger::info("Detached", "Killing detached process");
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
    }

    isRun = _isRun;

    this->language = lang;

    if (language == "Cpp" && QFile::exists(getBinaryOutput()))
    {
        Log::MessageLogger::info("Runner", "Reusuing executable");
        compilationFinished(true);
    }
    else if (language == "Python" && QFile::exists(getProgramFile(".py")))
    {
        Log::MessageLogger::warn("Runner", "Running last buffered script. To run current "
                                           "script use Compile and Run");
        compilationFinished(true);
    }
    else if (language == "Java" && QFile::exists(getBinaryOutput(".class")))
    {
        Log::MessageLogger::info("Runner", "Re-using same old class");
        compilationFinished(true);
    }
    else
    {
        Log::MessageLogger::error("Runner", "Cannot run, Have you successfully compiled your code earlier?");
    }
}

void Runner::runDetached(QCodeEditor *editor, QString lang)
{
#if defined(__unix__)
    if (std::system("xterm -version") != 0)
    {
        Log::MessageLogger::error("Detached Execution", "This requires xterm to be installed on your system");
        return;
    }
#endif
    if (detachedHandle != nullptr)
    {
        if (detachedHandle->state() == QProcess::Running)
            Log::MessageLogger::info("Detached Runner", "Already running a detached process. Killing it now");
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
                command = "-e," + startRunCommand + ",-classpath," + getBaseDirectory() + ",a" +
                          runCommandLine; /* +
"; echo '\nExecution Done\nPress any key to exit'; read";*/

            else
            {
                Log::MessageLogger::error("Language Error", "Execution language was not identified. Restart");
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
                command = "/C,start," + startRunCommand + "," + getProgramFile(".py") + runCommandLine;
            else if (language == "Java")
                command = "/C,start," + startRunCommand + ",-classpath," + getBaseDirectory() + ",a" + runCommandLine;

            else
            {
                Log::MessageLogger::error("Language Error", "Execution language was not identified. Restart");
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

                QTimer *killtimer = new QTimer(runner[i]);
                killtimer->setSingleShot(true);
                killtimer->setInterval(5000);
                QObject::connect(killtimer, SIGNAL(timeout()), runner[i], SLOT(terminate()));

                QString args;

                if (language == "Cpp")
                    runner[i]->setProgram(getBinaryOutput());
                else if (language == "Python" || language == "Java")
                {
                    runner[i]->setProgram(startRunCommand);
                    if (language == "Python")
                        args += getProgramFile(".py");
                    else
                        args += "-classpath," + getBaseDirectory() + ",a";
                }
                else
                {
                    Log::MessageLogger::error("Language Error", "Execution language was not identified. Restart");
                    return;
                }

                runner[i]->setStandardInputFile(getInput(i));
                if (!runCommand.trimmed().isEmpty())
                    args += "," + runCommand.trimmed().replace(" ", ",");

                runner[i]->setArguments(args.split(","));

                QObject::connect(
                    runner[i], QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
                    [this, i](int exitCode, QProcess::ExitStatus exitStatus) { runFinished(i, exitCode, exitStatus); });
                QObject::connect(runner[i], &QProcess::errorOccurred, this,
                                 [this, i](QProcess::ProcessError e) { runError(i, e); });
                QObject::connect(runner[i], &QProcess::started, this, [this, i]() { runStarted(i); });
                runner[i]->start();
                killtimer->start();
            }
        }
    }
    else
    {
        Log::MessageLogger::warn("Runner", "Compiler reported errors. No executables were produced to run");
    }
}

void Runner::runError(int id, QProcess::ProcessError e)
{
    if (e == QProcess::ProcessError::Timedout)
        Log::MessageLogger::error("Runner[" + std::to_string(id + 1) + "]", "Time Limit of 5 sec reached");
    else
        Log::MessageLogger::error("Runner[" + std::to_string(id + 1) + "]", "Error occurred during execution");
}

void Runner::runFinished(int id, int exitCode, QProcess::ExitStatus exitStatus)
{
    auto stderrMsg = QString::fromLocal8Bit(runner[id]->readAllStandardError()).toStdString();
    if (!stderrMsg.empty())
        Log::MessageLogger::error("Runner[" + std::to_string(id + 1) + "]/stderr", stderrMsg, true);
    if (exitCode == 0 && exitStatus == QProcess::ExitStatus::NormalExit)
    {
        emit executionFinished(id, QString::fromUtf8(runner[id]->readAllStandardOutput()));
    }
    else if (exitCode == 15)
    {
        // Sigterm is called by timeout timer;
        Log::MessageLogger::error("Runner[" + std::to_string(id + 1) + "]",
                                  "Timeout 5 sec, your program didn't returned");
    }
    else
    {
        Log::MessageLogger::error("Runner[" + std::to_string(id + 1) + "]",
                                  "Non-zero exit code " + std::to_string(exitCode));
    }
}

void Runner::runStarted(int id)
{
    Log::MessageLogger::info("Runner[" + std::to_string(id + 1) + "]", "Started execution");
}
} // namespace Core

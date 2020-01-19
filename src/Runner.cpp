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

#include <QFileInfo>
#include <Runner.hpp>

namespace Core
{

Runner::Runner(int index) : runnerIndex(index)
{
    runProcess = new QProcess();
}

Runner::~Runner()
{
    if (killTimer != nullptr)
    {
        delete killTimer;
    }
    if (runProcess != nullptr)
    {
        if (runProcess->state() == QProcess::Running)
        {
            runProcess->kill();
            emit runKilled(runnerIndex);
        }
        delete runProcess;
    }
    if (runTimer != nullptr)
    {
        delete runTimer;
    }
}

void Runner::run(const QString &filePath, const QString &lang, const QString &runCommand, const QString &args,
                 const QString &input, int timeLimit)
{
    if (!QFile::exists(filePath))
    {
        emit runErrorOccured(runnerIndex, "The source file " + filePath + " doesn't exist");
        return;
    }

    QString command = getCommand(filePath, lang, runCommand, args);
    if (command.isEmpty())
    {
        emit runErrorOccured(runnerIndex, "Failed to get run command. It's probably a bug");
        return;
    }

    connect(runProcess, SIGNAL(started()), this, SLOT(onStarted()));
    connect(runProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onFinished(int, QProcess::ExitStatus)));

    killTimer = new QTimer(runProcess);
    killTimer->setSingleShot(true);
    killTimer->setInterval(timeLimit);
    connect(killTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));

    runTimer = new QElapsedTimer();

    killTimer->start();
    runTimer->start();

    runProcess->start(command);
    runProcess->write(input.toStdString().c_str());
    runProcess->closeWriteChannel();
}

void Runner::runDetached(const QString &filePath, const QString &lang, const QString &runCommand, const QString &args)
{
    QString command;

#if defined(__unix__)
    QProcess testProcess;
    testProcess.start("xterm -v");
    bool finished = testProcess.waitForFinished(2000);
    if (!finished || testProcess.exitCode() != 0)
    {
        emit runErrorOccured(runnerIndex, "Please install xterm in order to use Detached Run");
        return;
    }
    command = "xterm -e \"" + getCommand(filePath, lang, runCommand, args) +
              "; echo '\nExecution Done\nPress any key to exit'; read\"";
#else
    command = "start cmd /C " + getCommand(filePath, lang, runCommand, args) + "^& pause";
#endif

    runProcess->start(command);
}

void Runner::onFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    emit runFinished(runnerIndex, runProcess->readAllStandardOutput(), runProcess->readAllStandardError(), exitCode,
                     runTimer->elapsed());
}

void Runner::onStarted()
{
    emit runStarted(runnerIndex);
}

void Runner::onTimeout()
{
    if (runProcess->state() == QProcess::Running)
    {
        runProcess->kill();
        emit runTimeout(runnerIndex);
    }
}

QString Runner::getCommand(const QString &filePath, const QString &lang, const QString &runCommand, const QString &args)
{
    QFileInfo fileInfo(filePath);

    if (lang == "Cpp")
    {
        return fileInfo.canonicalPath() + "/" + fileInfo.completeBaseName() + " " + args;
    }
    else if (lang == "Java")
    {
        return runCommand + " -classpath " + fileInfo.canonicalPath() + " a " + args;
    }
    else if (lang == "Python")
    {
        return runCommand + " " + fileInfo.canonicalFilePath() + " " + args;
    }
    else
    {
        return "";
    }
}

} // namespace Core

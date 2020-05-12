/*
 * Copyright (C) 2019-2020 Ashar Khan <ashar786khan@gmail.com>
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

#include "Core/Runner.hpp"
#include "Core/EventLogger.hpp"
#include "Util.hpp"
#include <QElapsedTimer>
#include <QFileInfo>
#include <QTimer>
#include <generated/SettingsHelper.hpp>

namespace Core
{

Runner::Runner(int index) : runnerIndex(index)
{
    runProcess = new QProcess();
    connect(runProcess, SIGNAL(started()), this, SLOT(onStarted()));
}

Runner::~Runner()
{
    // The order of destructions is important, runTimer is used when emitting signals

    if (killTimer != nullptr)
        delete killTimer;

    if (runProcess != nullptr)
    {
        if (runProcess->state() == QProcess::Running)
        {
            // Kill the process if it's still running when the Runner is destructed
            LOG_WARN("Runner at index:" << runnerIndex << " was running and forcefully killed");
            runProcess->kill();
            emit runKilled(runnerIndex);
        }
        delete runProcess;
    }

    if (runTimer != nullptr)
        delete runTimer;
}

void Runner::run(const QString &filePath, const QString &lang, const QString &runCommand, const QString &args,
                 const QString &input, int timeLimit)
{
    LOG_INFO(INFO_OF(filePath) << INFO_OF(lang) << INFO_OF(runCommand) << INFO_OF(args) << INFO_OF(timeLimit));
    if (!QFile::exists(filePath)) // make sure the source file exists, this usually means the executable file exists
    {
        emit failedToStartRun(runnerIndex, "The source file " + filePath + " doesn't exist");
        return;
    }

    // get the command for execution
    Command command = getCommand(filePath, lang, runCommand, args);
    if (command.full.isEmpty())
    {
        emit failedToStartRun(runnerIndex, "Failed to get run command. It's probably a bug");
        return;
    }

    // connect signals and set timers

    connect(runProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onFinished(int, QProcess::ExitStatus)));
    connect(runProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(onReadyReadStandardOutput()));
    connect(runProcess, SIGNAL(readyReadStandardError()), this, SLOT(onReadyReadStandardError()));

    killTimer = new QTimer(runProcess);
    killTimer->setSingleShot(true);
    killTimer->setInterval(timeLimit);
    connect(killTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));

    runTimer = new QElapsedTimer();

    killTimer->start();
    runTimer->start();

    runProcess->start(command.prog, command.args);
    bool started = runProcess->waitForStarted(2000);

    if (!started)
    {
        // usually, fail to start is because it's not compiled
        emit failedToStartRun(runnerIndex, "Failed to start running. Please compile first");
        runProcess->kill();
        return;
    }

    // write input to the program
    runProcess->write(input.toStdString().c_str());
    runProcess->closeWriteChannel();
}

void Runner::runDetached(const QString &filePath, const QString &lang, const QString &runCommand, const QString &args)
{
    // different steps on different OSs
#if defined(__unix__)
    // use xterm on Linux
    // check whether xterm is installed at first
    LOG_INFO("Using xterm on unix");
    QProcess testProcess;
    testProcess.start("xterm -v");
    bool finished = testProcess.waitForFinished(2000);
    if (!finished || testProcess.exitCode() != 0)
    {
        emit failedToStartRun(runnerIndex, "Please install xterm in order to use Detached Run");
        return;
    }
    runProcess->setProgram("xterm");
    runProcess->setArguments({"-e", getCommand(filePath, lang, runCommand, args).full +
                                        "; read -n 1 -s -r -p '\nExecution Done\nPress any key to exit'"});
    LOG_INFO("Xterm args " << runProcess->arguments().join(" "));
    runProcess->start();
#elif defined(__APPLE__)
    // use apple script on Mac OS
    runProcess->setProgram("osascript");
    runProcess->setArguments({"-l", "AppleScript"});
    QString script = "tell app \"Terminal\" to do script \"" +
                     getCommand(filePath, lang, runCommand, args).full.replace("\"", "'") + "\"";
    runProcess->start();
    LOG_INFO("Running apple script\n" << script);
    runProcess->write(script.toUtf8());
    runProcess->closeWriteChannel();
#else
    // use cmd on Windows
    runProcess->start("cmd /C \"start cmd /C " +
                      getCommand(filePath, lang, runCommand, args).full.replace("\"", "^\"") + " ^& pause\"");
    LOG_INFO("CMD Arguemnts " << runProcess->arguments().join(" "));

#endif
}

void Runner::onFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    emit runFinished(runnerIndex, processStdout + runProcess->readAllStandardOutput(),
                     processStderr + runProcess->readAllStandardError(), exitCode, runTimer->elapsed());
}

void Runner::onStarted()
{
    emit runStarted(runnerIndex);
}

void Runner::onTimeout()
{
    if (runProcess->state() == QProcess::Running)
    {
        LOG_INFO("Process was running, and forcefully killed it because time limit was reached");
        runProcess->kill();
        emit runTimeout(runnerIndex);
    }
}

void Runner::onReadyReadStandardOutput()
{
    processStdout.append(runProcess->readAllStandardOutput());
    if (!outputLimitExceededEmitted && processStdout.length() > SettingsHelper::getOutputLengthLimit())
    {
        outputLimitExceededEmitted = true;
        runProcess->kill();
        LOG_INFO("Process was running, and forcefully killed it because stdout limit was reached");
        emit runOutputLimitExceeded(runnerIndex, "stdout");
    }
}

void Runner::onReadyReadStandardError()
{
    processStderr.append(runProcess->readAllStandardError());
    if (!outputLimitExceededEmitted && processStderr.length() > SettingsHelper::getOutputLengthLimit())
    {
        outputLimitExceededEmitted = true;
        runProcess->kill();
        LOG_INFO("Process was running, and forcefully killed it because stderr limit was reached");
        emit runOutputLimitExceeded(runnerIndex, "stderr");
    }
}

Runner::Command Runner::getCommand(const QString &filePath, const QString &lang, const QString &runCommand,
                                   const QString &arg)
{
    // get the execution command by the file path and the language
    // please remember to add quotes for the paths

    QFileInfo fileInfo(filePath);
    Command cmd;
    if (lang == "C++")
    {
        cmd.prog = fileInfo.canonicalPath() + "/" + fileInfo.completeBaseName();
        cmd.args = Util::splitArgument(arg);
        cmd.full = QString("\"%1\" %2").arg(cmd.prog, arg);
    }
    else if (lang == "Java")
    {
        cmd.prog = runCommand;
        cmd.args << "-classpath" << fileInfo.canonicalPath() << "a" << Util::splitArgument(arg);
        cmd.full = QString("%1 -classpath \"%2\" a %3").arg(runCommand, fileInfo.canonicalPath(), arg);
    }
    else if (lang == "Python")
    {
        cmd.prog = runCommand;
        cmd.args << fileInfo.canonicalPath() << Util::splitArgument(arg);
        cmd.full = QString("%1 \"%2\" %3").arg(runCommand, fileInfo.canonicalPath(), arg);
    }

    LOG_INFO("Returning runCommand as : " << cmd.full);

    return cmd;
}

} // namespace Core

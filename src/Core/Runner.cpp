/*
 * Copyright (C) 2019-2021 Ashar Khan <ashar786khan@gmail.com>
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
#include "Core/Compiler.hpp"
#include "Core/EventLogger.hpp"
#include "Util/FileUtil.hpp"
#include <QElapsedTimer>
#include <QFileInfo>
#include <QTemporaryFile>
#include <QTimer>
#include <generated/SettingsHelper.hpp>

namespace Core
{

Runner::Runner(int index) : runnerIndex(index)
{
    runProcess = new QProcess();
    connect(runProcess, &QProcess::started, this, &Runner::onStarted);
    connect(runProcess, &QProcess::errorOccurred, this, &Runner::onErrorOccurred);
}

Runner::~Runner()
{
    // The order of destructions is important, runTimer is used when emitting signals

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

    delete runTimer;
}

void Runner::run(const QString &tmpFilePath, const QString &sourceFilePath, const QString &lang,
                 const QString &runCommand, const QString &args, const QString &input, int timeLimit)
{
    LOG_INFO(INFO_OF(tmpFilePath) << INFO_OF(sourceFilePath) << INFO_OF(lang) << INFO_OF(runCommand) << INFO_OF(args)
                                  << INFO_OF(timeLimit));

    isDetachedRun = false;

    if (!QFile::exists(tmpFilePath)) // make sure the source file exists, this usually means the executable file exists
    {
        emit failedToStartRun(runnerIndex, tr("The source file %1 doesn't exist.").arg(tmpFilePath));
        return;
    }

    // get the command for execution
    QStringList command = QProcess::splitCommand(getCommand(tmpFilePath, sourceFilePath, lang, runCommand, args));
    if (command.isEmpty())
    {
        emit failedToStartRun(runnerIndex, tr("Failed to get run command. It's probably a bug."));
        return;
    }

    // connect signals and set timers

    connect(runProcess, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, &Runner::onFinished);
    connect(runProcess, &QProcess::readyReadStandardOutput, this, &Runner::onReadyReadStandardOutput);
    connect(runProcess, &QProcess::readyReadStandardError, this, &Runner::onReadyReadStandardError);

    QString program = command.takeFirst();

    setWorkingDirectory(tmpFilePath, sourceFilePath, lang);

    inputFile = new QTemporaryFile(this);
    if (!inputFile->open())
    {
        emit failedToStartRun(runnerIndex, tr("Failed to create temporary file."));
        return;
    }
    Util::saveFile(inputFile->fileName(), input, "Runner Input", false);
    runProcess->setStandardInputFile(inputFile->fileName());

    killTimer = new QTimer(runProcess);
    killTimer->setSingleShot(true);
    killTimer->setInterval(timeLimit);
    connect(killTimer, &QTimer::timeout, this, &Runner::onTimeout);

    runTimer = new QElapsedTimer();

    killTimer->start();

    runProcess->start(program, command);
}

void Runner::runDetached(const QString &tmpFilePath, const QString &sourceFilePath, const QString &lang,
                         const QString &runCommand, const QString &args)
{
    isDetachedRun = true;

    setWorkingDirectory(tmpFilePath, sourceFilePath, lang);

    // different steps on different OSs
#if defined(Q_OS_MACOS)
    // use apple script on Mac OS
    runProcess->setProgram("osascript");
    runProcess->setArguments({"-l", "AppleScript"});
    QString script = R"(tell app "Terminal" to do script ")" +
                     getCommand(tmpFilePath, sourceFilePath, lang, runCommand, args).replace("\"", "'") + "\"";
    runProcess->start();
    LOG_INFO("Running apple script\n" << script);
    runProcess->write(script.toUtf8());
    runProcess->closeWriteChannel();
#elif defined(Q_OS_WIN)
    // use cmd on Windows
    runProcess->start("cmd", QProcess::splitCommand(
                                 "/C \"start cmd /C " +
                                 getCommand(tmpFilePath, sourceFilePath, lang, runCommand, args).replace("\"", "^\"") +
                                 " ^& pause\""));
    LOG_INFO("CMD Arguemnts " << runProcess->arguments().join(" "));
#elif defined(Q_OS_UNIX)
    auto terminal = SettingsHelper::getDetachedRunTerminalProgram();
    LOG_INFO("Using: " << terminal << " on UNIX");
    auto quotedCommand = getCommand(tmpFilePath, sourceFilePath, lang, runCommand, args);
    auto execArgs = QProcess::splitCommand(SettingsHelper::getDetachedRunTerminalArguments()) +
                    QStringList{"/bin/bash", "-c",
                                QStringLiteral("%1 ; echo \"\n%2\" ; read -n 1")
                                    .arg(quotedCommand)
                                    .arg(tr("Program finished with exit code %1\nPress any key to exit").arg("$?"))};
    runProcess->start(terminal, execArgs);
#else
    emit failedToStartRun(runnerIndex, tr("Detached execution is not supported on your platform"));
#endif
}

void Runner::onFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    const auto timeUsed = runTimer->isValid() ? runTimer->elapsed() : 0;
    emit runFinished(runnerIndex, processStdout + runProcess->readAllStandardOutput(),
                     processStderr + runProcess->readAllStandardError(), exitCode, timeUsed, timeLimitExceeded);
}

void Runner::onStarted()
{
    if (!isDetachedRun)
        runTimer->start();
    emit runStarted(runnerIndex);
}

void Runner::onTimeout()
{
    if (runProcess->state() == QProcess::Running)
    {
        LOG_INFO("Process was running, and forcefully killed it because time limit was reached");
        timeLimitExceeded = true;
        runProcess->kill();
    }
}

void Runner::onReadyReadStandardOutput()
{
    processStdout.append(runProcess->readAllStandardOutput().replace('\0', ""));
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
    processStderr.append(runProcess->readAllStandardError().replace('\0', ""));
    if (!outputLimitExceededEmitted && processStderr.length() > SettingsHelper::getOutputLengthLimit())
    {
        outputLimitExceededEmitted = true;
        runProcess->kill();
        LOG_INFO("Process was running, and forcefully killed it because stderr limit was reached");
        emit runOutputLimitExceeded(runnerIndex, "stderr");
    }
}

void Runner::onErrorOccurred(QProcess::ProcessError error)
{
    if (error == QProcess::FailedToStart)
    {
        if (isDetachedRun)
        {
            emit failedToStartRun(
                runnerIndex,
                tr("Failed to start detached execution. Please check your terminal emulator settings in %1.")
                    .arg(SettingsHelper::pathOfDetachedRunTerminalProgram(true)));
        }
        else
        {
            emit failedToStartRun(runnerIndex, tr("Failed to start running. Please compile first."));
        }
    }
}

QString Runner::getCommand(const QString &tmpFilePath, const QString &sourceFilePath, const QString &lang,
                           const QString &runCommand, const QString &args)
{
    // get the execution command by the file path and the language
    // please remember to add quotes for the paths

    QString res;

    if (lang == "C++")
    {
        res = QString("\"%1\" %2").arg(Compiler::outputPath(tmpFilePath, sourceFilePath, "C++")).arg(args);
    }
    else if (lang == "Java")
    {
        res = QString("%1 -classpath \"%2\" %3 %4")
                  .arg(runCommand)
                  .arg(Compiler::outputPath(tmpFilePath, sourceFilePath, "Java"))
                  .arg(SettingsHelper::getLanguageConfig("Java").getClassName())
                  .arg(args);
    }
    else if (lang == "Python")
    {
        res = QString("%1 \"%2\" %3").arg(runCommand).arg(QFileInfo(tmpFilePath).canonicalFilePath()).arg(args);
    }

    LOG_INFO("Returning runCommand as : " << res);

    return res;
}

void Runner::setWorkingDirectory(const QString &tmpFilePath, const QString &sourceFilePath, const QString &lang)
{
    runProcess->setWorkingDirectory(
        QFileInfo(Compiler::outputFilePath(tmpFilePath, sourceFilePath, lang, false)).path());
}

} // namespace Core

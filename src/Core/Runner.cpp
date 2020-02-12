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

#include "Core/Runner.hpp"
#include "Core/EventLogger.hpp"
#include <QFileInfo>

namespace Core
{

Runner::Runner(int index) : runnerIndex(index)
{
    Core::Log::i("runner/constructed") << "index : " << index << endl;
    runProcess = new QProcess();
    connect(runProcess, SIGNAL(started()), this, SLOT(onStarted()));
}

Runner::~Runner()
{
    Core::Log::i("runner/destructed", "Destruction of this runner has began");
    if (killTimer != nullptr)
    {
        Core::Log::i("runner/destructed", "killtimer is being deleted");
        delete killTimer;
    }
    if (runProcess != nullptr)
    {
        Core::Log::i("runner/destructed", "runProcess is being deleted");
        if (runProcess->state() == QProcess::Running)
        {
            Core::Log::i("runner/destructed", "runProcess is was running, now killing");
            runProcess->kill();
            emit runKilled(runnerIndex);
        }
        delete runProcess;
    }
    if (runTimer != nullptr)
    {
        Core::Log::i("runner/destructed", "runTimer is being deleted");
        delete runTimer;
    }
}

void Runner::run(const QString &filePath, const QString &lang, const QString &runCommand, const QString &args,
                 const QString &input, int timeLimit)
{
    Core::Log::i("runner/run") << "filepath" << filePath << " lang " << lang << " runCommand " << runCommand << " args "
                               << args << " input " << input << "timelimit " << timeLimit << endl;
    if (!QFile::exists(filePath))
    {
        Core::Log::e("runner/run", "source file does not exist on disk now");
        emit runErrorOccured(runnerIndex, "The source file " + filePath + " doesn't exist");
        return;
    }

    QString command = getCommand(filePath, lang, runCommand, args);
    if (command.isEmpty())
    {
        emit runErrorOccured(runnerIndex, "Failed to get run command. It's probably a bug");
        return;
    }

    connect(runProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onFinished(int, QProcess::ExitStatus)));

    killTimer = new QTimer(runProcess);
    killTimer->setSingleShot(true);
    killTimer->setInterval(timeLimit);
    connect(killTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));

    Core::Log::i("runner/run", "killshort timer started and attached");

    runTimer = new QElapsedTimer();

    killTimer->start();
    runTimer->start();

    Core::Log::i("runner/run", "timers have started waiting 2 sec for process to start now");

    runProcess->start(command);
    bool started = runProcess->waitForStarted(2000);
    Core::Log::w("runner/run", "process wait timeout");

    if (!started)
    {
        Core::Log::e("runner/run", "process was not started");
        emit runErrorOccured(runnerIndex, "Failed to start running. Please compile first");
        runProcess->kill();
        return;
    }
    runProcess->write(input.toStdString().c_str());
    runProcess->closeWriteChannel();
    Core::Log::i("runner/run", "All inputs were written to stdin of process");
}

void Runner::runDetached(const QString &filePath, const QString &lang, const QString &runCommand, const QString &args)
{
#if defined(__unix__)
    Core::Log::i("runner/runDetached", "using xterm on unix");
    QProcess testProcess;
    testProcess.start("xterm -v");
    bool finished = testProcess.waitForFinished(2000);
    if (!finished || testProcess.exitCode() != 0)
    {
        Core::Log::e("runner/runDetached", "xterm is not installed");
        emit runErrorOccured(runnerIndex, "Please install xterm in order to use Detached Run");
        return;
    }
    Core::Log::i("runner/runDetached", "xterm is reachable");
    runProcess->setProgram("xterm");
    runProcess->setArguments({"-e", getCommand(filePath, lang, runCommand, args) +
                                        "; read -n 1 -s -r -p '\nExecution Done\nPress any key to exit'"});
    Core::Log::i("runner/runDetached") << "xterm args " << runProcess->arguments().join(" ") << endl;
    runProcess->start();
#else
    Core::Log::i("runner/runDetached", "on windows. Using cmd");
    runProcess->start("cmd /C \"start cmd /C " + getCommand(filePath, lang, runCommand, args).replace("\"", "^\"") +
                      " ^& pause\"");
    Core::Log::i("runner/runDetached") << "cmd args " << runProcess->arguments().join(" ") << endl;

#endif
}

void Runner::onFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Core::Log::i("runner/onfinished") << "index is : " << runnerIndex << endl;
    emit runFinished(runnerIndex, runProcess->readAllStandardOutput(), runProcess->readAllStandardError(), exitCode,
                     runTimer->elapsed());
}

void Runner::onStarted()
{
    Core::Log::i("runner/onstarted") << "index is : " << runnerIndex << endl;
    emit runStarted(runnerIndex);
}

void Runner::onTimeout()
{
    Core::Log::i("runner/ontimeout") << "index is : " << runnerIndex << endl;
    if (runProcess->state() == QProcess::Running)
    {
        Core::Log::i("runner/ontimeout", "process was running, now killing");
        runProcess->kill();
        emit runTimeout(runnerIndex);
    }
}

QString Runner::getCommand(const QString &filePath, const QString &lang, const QString &runCommand, const QString &args)
{
    Core::Log::i("runner/getCommand") << "filePath : " << filePath << " lang : " << lang
                                      << " runCommand : " << runCommand << "args : " << args << endl;
    QFileInfo fileInfo(filePath);
    QString res;
    if (lang == "C++")
    {
        res = "\"" + fileInfo.canonicalPath() + "/" + fileInfo.completeBaseName() + "\" " + args;
    }
    else if (lang == "Java")
    {
        res = runCommand + " -classpath \"" + fileInfo.canonicalPath() + "\" a " + args;
    }
    else if (lang == "Python")
    {
        res = runCommand + " \"" + fileInfo.canonicalFilePath() + "\" " + args;
    }
    Core::Log::i("runner/getCommand") << "Returning runCommand as : " << res << endl;
    return res;
}

} // namespace Core

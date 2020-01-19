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

#include <Compiler.hpp>
#include <QFileInfo>

namespace Core
{

Compiler::Compiler()
{
    compileProcess = new QProcess();
    connect(compileProcess, SIGNAL(started()), this, SIGNAL(compilationStarted()));
    connect(compileProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this,
            SLOT(onProcessFinished(int, QProcess::ExitStatus)));
}

Compiler::~Compiler()
{
    if (compileProcess != nullptr)
    {
        delete compileProcess;
    }
}

void Compiler::start(const QString &filePath, const QString &compileCommand, const QString &lang)
{
    if (!QFile::exists(filePath))
    {
        emit compilationErrorOccured("The source file [" + filePath + "] doesn't exist");
        return;
    }

    QString command;

    if (lang == "Cpp")
    {
        command = compileCommand + " " + filePath + " -o " + QFileInfo(filePath).canonicalPath() + "/" +
                  QFileInfo(filePath).completeBaseName();
    }
    else if (lang == "Java")
    {
        command = compileCommand + " " + filePath;
    }
    else if (lang == "Python")
    {
        emit compilationFinished("");
        return;
    }
    else
    {
        emit compilationErrorOccured("Unsupported programming language \"" + lang + "\"");
        return;
    }

    compileProcess->start(command);
}

bool Compiler::check(const QString &compileCommand)
{
    QProcess checkProcess;
    checkProcess.start(compileCommand.trimmed().split(' ').front() + " --version");
    bool finished = checkProcess.waitForFinished(1000);
    if (finished && checkProcess.exitCode() == 0)
        return true;
    checkProcess.kill();
    checkProcess.start(compileCommand.trimmed().split(' ').front() + " -version");
    finished = checkProcess.waitForFinished(1000);
    return finished && checkProcess.exitCode() == 0;
}

void Compiler::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitCode == 0)
    {
        emit compilationFinished(compileProcess->readAllStandardError());
    }
    else
    {
        emit compilationErrorOccured(compileProcess->readAllStandardError());
    }
}

} // namespace Core

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

#include "Core/Compiler.hpp"
#include "Core/EventLogger.hpp"
#include "Settings/SettingsManager.hpp"
#include <QDir>
#include <QFileInfo>
#include <QProcess>

namespace Core
{

Compiler::Compiler()
{
    // create compiliation process and connect signals
    compileProcess = new QProcess();
    connect(compileProcess, SIGNAL(started()), this, SIGNAL(compilationStarted()));
    connect(compileProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onProcessFinished(int)));
}

Compiler::~Compiler()
{
    if (compileProcess != nullptr)
    {
        if (compileProcess->state() != QProcess::NotRunning)
        {
            // kill the compilation process if it's still running when the Compiler is being destructed
            LOG_WARN("Compiler process was running and is being forcefully killed");
            compileProcess->kill();
            emit compilationKilled();
        }
        delete compileProcess;
    }
}

void Compiler::start(const QString &tmpFilePath, const QString &sourceFilePath, const QString &compileCommand,
                     const QString &lang)
{
    if (!QFile::exists(tmpFilePath))
    {
        // quit with error if the source file is not found
        emit compilationErrorOccurred(tr("The source file [%1] doesn't exist").arg(tmpFilePath));
        return;
    }

    if (lang == "Python")
    {
        emit compilationFinished(""); // we don't actually compile Python
        return;
    }

    // get the compile command

    QStringList args = QProcess::splitCommand(compileCommand);

    if (args.isEmpty())
    {
        emit compilationErrorOccurred(tr("The compile command for %1 is empty").arg(lang));
        return;
    }

    QString program = args.takeFirst();

    if (lang == "C++")
    {
        args << QFileInfo(tmpFilePath).canonicalFilePath() << "-o" << outputPath(tmpFilePath, sourceFilePath, "C++");
    }
    else if (lang == "Java")
    {
        args << QFileInfo(tmpFilePath).canonicalFilePath() << "-d" << outputPath(tmpFilePath, sourceFilePath, "Java");
    }
    else
    {
        emit compilationErrorOccurred(tr("Unsupported programming language \"%1\"").arg(lang));
        return;
    }

    LOG_INFO(INFO_OF(lang) << INFO_OF(args.join(" ")));
    // start compilation
    compileProcess->start(program, args);
}

bool Compiler::check(const QString &compileCommand)
{
    if (compileCommand.isEmpty())
    {
        LOG_WARN("The compile command is empty");
        return false;
    }

    QProcess checkProcess;

    // check both "--version" and "-version", "-version" is mainly for Java

    checkProcess.start(QProcess::splitCommand(compileCommand)[0], {"--version"});
    bool finished = checkProcess.waitForFinished(2000);
    if (finished && checkProcess.exitCode() == 0)
        return true;
    checkProcess.kill(); // kill it if it's not finished, no harm if it's finished with non-zero exit code

    checkProcess.start(QProcess::splitCommand(compileCommand)[0], {"-version"});
    finished = checkProcess.waitForFinished(2000);

    LOG_INFO(BOOL_INFO_OF(finished) << INFO_OF(checkProcess.exitCode()));

    return finished && checkProcess.exitCode() == 0;
}

QString Compiler::outputPath(const QString &tmpFilePath, const QString &sourceFilePath, const QString &lang)
{
    QFileInfo fileInfo(sourceFilePath.isEmpty() ? tmpFilePath : sourceFilePath);
    QString res = fileInfo.dir().filePath(SettingsManager::get(lang + "/Output Path")
                                              .toString()
                                              .replace("${filename}", fileInfo.fileName())
                                              .replace("${basename}", fileInfo.completeBaseName())
                                              .replace("${tmpdir}", QFileInfo(tmpFilePath).absolutePath()));
    if (lang == "C++")
        QDir().mkpath(QFileInfo(res).absolutePath());
    else if (lang == "Java")
        QDir().mkpath(res);
    return res;
}

void Compiler::onProcessFinished(int exitCode)
{
    // emit different signals due to different exit codes
    if (exitCode == 0)
        emit compilationFinished(compileProcess->readAllStandardError());
    else
        emit compilationErrorOccurred(compileProcess->readAllStandardError());
}

} // namespace Core

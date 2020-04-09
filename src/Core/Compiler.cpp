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
#include <QFileInfo>

namespace Core
{

Compiler::Compiler()
{
    Core::Log::i("compiler/Constructor", "Compiler instance is being created");

    // create compiliation process and connect signals
    compileProcess = new QProcess();
    connect(compileProcess, SIGNAL(started()), this, SIGNAL(compilationStarted()));
    connect(compileProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onProcessFinished(int)));
}

Compiler::~Compiler()
{
    Core::Log::i("compiler/Destructor", "Coompiler instance is being deleted");
    if (compileProcess != nullptr)
    {
        Core::Log::i("compiler/Destructor", "Compiler object destroyed");
        if (compileProcess->state() != QProcess::NotRunning)
        {
            // kill the compilation process if it's still running when the Compiler is being destructed
            Core::Log::i("Compiler/destructor", "Compile Process is running, now killing it");
            compileProcess->kill();
            emit compilationKilled();
        }
        delete compileProcess;
    }
    else
    {
        Core::Log::w("compiler/Destructor", "compile Process is already Null");
    }
}

void Compiler::start(const QString &filePath, const QString &compileCommand, const QString &lang)
{
    if (!QFile::exists(filePath))
    {
        // quit with error if the source file is not found
        Core::Log::i("compiler/start", "The source file [" + filePath + "] doesn't exist");
        emit compilationErrorOccured("The source file [" + filePath + "] doesn't exist");
        return;
    }

    // get the full compile command
    // please remember to quote the file paths

    QString command;

    if (lang == "C++")
    {
        command = compileCommand + " \"" + QFileInfo(filePath).canonicalFilePath() + "\" -o \"" +
                  QFileInfo(filePath).canonicalPath() + "/" + QFileInfo(filePath).completeBaseName() + "\"";
        Core::Log::i("Compiler/start[C++]") << command << endl;
    }
    else if (lang == "Java")
    {
        command = compileCommand + " \"" + QFileInfo(filePath).canonicalFilePath() + "\"";
        Core::Log::i("Compiler/start[Java]") << command << endl;
    }
    else if (lang == "Python")
    {
        Core::Log::i("Compiler/start[Python]", "No compilation is required");
        emit compilationFinished(""); // we don't actually compile Python
        return;
    }
    else
    {
        Core::Log::i("compiler/start", "Language is unsupported");
        emit compilationErrorOccured("Unsupported programming language \"" + lang + "\"");
        return;
    }

    // start compilation
    compileProcess->start(command);
}

bool Compiler::check(const QString &compileCommand)
{
    QProcess checkProcess;

    // check both "--version" and "-version", "-version" is mainly for Java

    checkProcess.start(compileCommand.trimmed().split(' ').front() + " --version");
    bool finished = checkProcess.waitForFinished(1000);
    if (finished && checkProcess.exitCode() == 0)
        return true;
    checkProcess.kill(); // kill it if it's not finished, no harm if it's finished with non-zero exit code

    checkProcess.start(compileCommand.trimmed().split(' ').front() + " -version");
    finished = checkProcess.waitForFinished(1000);
    Core::Log::i("Compiler/check", "Checking compiler command " + compileCommand);
    return finished && checkProcess.exitCode() == 0;
}

void Compiler::onProcessFinished(int exitCode)
{

    // emit different signals due to different exit codes
    if (exitCode == 0)
    {
        emit compilationFinished(compileProcess->readAllStandardError());
    }
    else
    {
        emit compilationErrorOccured(compileProcess->readAllStandardError());
    }
    Core::Log::i("Compiler/onProcessFinished") << "Exit code is: " << exitCode << endl;
}

} // namespace Core

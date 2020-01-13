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
* Believe Software is "Software" and it isn't immune to bugs.
* 
*/


#include <Core.hpp>
#include <MessageLogger.hpp>
#include <QProcess>

namespace Core
{
Compiler::Compiler(QString commandCpp, QString commandJava, int index, MessageLogger *log)
    : Core::Base::Files(index), log(log)
{
    runCommandJava = commandJava;
    runCommandCpp = commandCpp;
    file = new QFile(getProgramFile());
    file->open(QIODevice::ReadWrite | QFile::Text);
    if (!file->isOpen())
    {
        log->error("Compiler", "Cannot create temporary compilation file for C/C++ language");
    }
    else
    {
        file->resize(0);
    }

    pyFile = new QFile(getProgramFile(".py"));
    pyFile->open(QIODevice::ReadWrite | QFile::Text);
    if (!pyFile->isOpen())
    {
        log->error("Compiler", "Cannot create temporary compilation file for python language");
    }
    else
    {
        pyFile->resize(0);
    }

    javaFile = new QFile(getProgramFile(".java"));
    javaFile->open(QIODevice::ReadWrite | QFile::Text);
    if (!javaFile->isOpen())
    {
        log->error("Compiler", "Cannot create temporary compilation file for java language");
    }
    else
    {
        javaFile->resize(0);
    }
}

Compiler::~Compiler()
{
    if (file->isOpen())
        file->close();
    delete file;

    if (pyFile->isOpen())
        pyFile->close();
    delete pyFile;

    if (javaFile->isOpen())
        javaFile->close();
    delete javaFile;

    if (compilationProcess != nullptr)
        delete compilationProcess;
}

void Compiler::syncToBuffer(QCodeEditor *editor)
{
    if (!file->isOpen())
        file->open(QIODevice::ReadWrite | QFile::Text);

    if (!pyFile->isOpen())
        pyFile->open(QIODevice::ReadWrite | QFile::Text);

    if (!javaFile->isOpen())
        javaFile->open(QIODevice::ReadWrite | QFile::Text);

    file->resize(0);
    file->write(editor->toPlainText().toUtf8().toStdString().c_str());
    file->close();

    pyFile->resize(0);
    pyFile->write(editor->toPlainText().toUtf8().toStdString().c_str());
    pyFile->close();

    javaFile->resize(0);
    javaFile->write(editor->toPlainText().toUtf8().toStdString().c_str());
    javaFile->close();
}

void Compiler::compile(QCodeEditor *editor, QString lang)
{
    if (compilationProcess != nullptr)
    {
        compilationProcess->kill();
        delete compilationProcess; // calls sigkill
        compilationProcess = nullptr;
    }

    syncToBuffer(editor);
    log->info("Compiler", "Compilation requested");

    if (lang == "Python")
    {
        log->info("Compiler", "Internal buffer updated");
        emit compilationFinished(true);
        return;
    }

    QString extension;
    if (lang == "Java")
        extension = ".java";

    compilationProcess = new QProcess();

    if (lang == "Cpp")
    {
        auto lists = runCommandCpp.trimmed().split(" ");
        compilationProcess->setProgram(lists[0]);
        lists.removeAt(0);
        lists.append(getProgramFile(extension));
        lists.append("-o");
        lists.append(getBinaryOutput(extension));
        compilationProcess->setArguments(lists);
    }
    else if (lang == "Java")
    {
        auto lists = runCommandJava.trimmed().split(" ");
        compilationProcess->setProgram(lists[0]);
        lists.removeAt(0);
        lists.append(getProgramFile(extension));
        compilationProcess->setArguments(lists);
    }
    else
    {
        emit compilationFinished(true);
        return;
    }

    QObject::connect(compilationProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this,
                     SLOT(finished(int, QProcess::ExitStatus)));

    QObject::connect(compilationProcess, SIGNAL(errorOccurred(QProcess::ProcessError)), this,
                     SLOT(errorOccurred(QProcess::ProcessError)));

    QObject::connect(compilationProcess, SIGNAL(started(void)), this, SLOT(started(void)));

    compilationProcess->start();
}

void Compiler::updateCommandCpp(QString newCommand)
{
    runCommandCpp = newCommand;
}
void Compiler::updateCommandJava(QString newCommand)
{
    runCommandJava = newCommand;
}

bool Compiler::check(QString comm)
{
    auto lst = comm.trimmed().split(" ");
    auto com = lst[0] + " --version";
    QProcess program;
    QString commandToStart = com;
    QStringList environment = program.systemEnvironment();
    program.start(commandToStart);
    bool started = program.waitForStarted();
    if (started) // 10 Second timeout
        program.kill();

    QString stdOutput = QString::fromLocal8Bit(program.readAllStandardOutput());
    QString stdError = QString::fromLocal8Bit(program.readAllStandardError());
    return started;
}

void Compiler::started()
{
    log->info("Compiler", "Compilation has started...");
    emit compilationStarted();
}

void Compiler::errorOccurred(QProcess::ProcessError e)
{
    switch (e)
    {
    case QProcess::ProcessError::Crashed:
        log->error("Compiler", "Compiler process crashed");
        break;
    case QProcess::ProcessError::Timedout:
        log->error("Compiler", "Compiler process timedout");
        break;
    case QProcess::ProcessError::ReadError:
        log->error("Compiler", "Compiler process readError. Is compiler command correct?");
        break;
    case QProcess::ProcessError::WriteError:
        log->error("Compiler", "Compiler process writeError");
        break;
    case QProcess::ProcessError::UnknownError:
        log->error("Compiler", "An unknown error caused the compiler process to abort");
        break;
    case QProcess::ProcessError::FailedToStart:
        log->error("Compiler", "Failed to start the compiler process");
    }
    emit compilationError();
}

void Compiler::finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitStatus == QProcess::ExitStatus::NormalExit)
    {
        if (exitCode != 0)
            log->error("Compiler", "Failed!! process returned exit code " + std::to_string(exitCode));
        else
            log->info("Compiler", "Success!! process returned exit code " + std::to_string(exitCode));
    }
    else
    {
        log->error("Compiler", "Crashed!! process returned exit code " + std::to_string(exitCode));
    }

    if (exitCode != 0 && compilationProcess != nullptr)
    {
        log->error("Compiler Message", QString::fromLocal8Bit(compilationProcess->readAllStandardError()).toStdString(),
                   true);

        emit compilationFinished(false);
    }
    else
    {
        QString StdErr = QString::fromLocal8Bit(compilationProcess->readAllStandardError());
        if (!StdErr.isEmpty())
        {
            log->warn("Compiler Warnings", StdErr.toStdString(), true);
        }
        emit compilationFinished(true);
    }
}

} // namespace Core

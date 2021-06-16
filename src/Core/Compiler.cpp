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

#include "Core/Compiler.hpp"
#include "Core/EventLogger.hpp"
#include "Settings/SettingsManager.hpp"
#include "Util/FileUtil.hpp"
#include "generated/SettingsHelper.hpp"
#include <QDir>
#include <QFileInfo>
#include <QTextCodec>

namespace Core
{

Compiler::Compiler()
{
    // create compiliation process and connect signals
    compileProcess = new QProcess();
    connect(compileProcess, &QProcess::started, this, &Compiler::compilationStarted);
    connect(compileProcess, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this,
            &Compiler::onProcessFinished);
    connect(compileProcess, &QProcess::errorOccurred, this, &Compiler::onProcessErrorOccurred);
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
    this->lang = lang;
    if (!QFile::exists(tmpFilePath))
    {
        // quit with error if the source file is not found
        emit compilationFailed(tr("The source file [%1] doesn't exist").arg(tmpFilePath));
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
        emit compilationFailed(
            tr("%1 is empty")
                .arg(SettingsManager::getPathText(SettingsHelper::getLanguageConfig(lang).pathOfCompileCommand())));
        return;
    }

    QString program = args.takeFirst();

    if (lang == "C++")
    {
        args << QFileInfo(tmpFilePath).canonicalFilePath() << "-o" << outputPath(tmpFilePath, sourceFilePath, "C++");
        if (QFile::exists(sourceFilePath))
            args << "-I" << QFileInfo(sourceFilePath).canonicalPath();
    }
    else if (lang == "Java")
    {
        args << QFileInfo(tmpFilePath).canonicalFilePath() << "-d" << outputPath(tmpFilePath, sourceFilePath, "Java");
    }
    else
    {
        emit compilationFailed(tr("Unsupported programming language \"%1\"").arg(lang));
        return;
    }

    LOG_INFO(INFO_OF(lang) << INFO_OF(program) << INFO_OF(args.join(" ")));

    compileProcess->setWorkingDirectory(
        QFileInfo(QFile::exists(sourceFilePath) ? sourceFilePath : tmpFilePath).canonicalPath());

    compileProcess->start(program, args);
}

QString Compiler::outputPath(const QString &tmpFilePath, const QString &sourceFilePath, const QString &lang,
                             bool createDirectory)
{
    if (lang == "Python")
        return tmpFilePath;

    QFileInfo fileInfo(sourceFilePath.isEmpty() ? tmpFilePath : sourceFilePath);
    QString res = fileInfo.dir().filePath(SettingsHelper::getLanguageConfig(lang)
                                              .getOutputPath()
                                              .replace("${filename}", fileInfo.fileName())
                                              .replace("${basename}", fileInfo.completeBaseName())
                                              .replace("${tmpdir}", QFileInfo(tmpFilePath).absolutePath())
                                              .replace("${tempdir}", QFileInfo(tmpFilePath).absolutePath()));
    if (createDirectory)
    {
        if (lang == "C++")
            QDir().mkpath(QFileInfo(res).absolutePath() + Util::exeSuffix); // Note: Util::exeSuffix is empty on UNIX
        else if (lang == "Java")
            QDir().mkpath(res);
    }
    return res;
}

QString Compiler::outputFilePath(const QString &tmpFilePath, const QString &sourceFilePath, const QString &lang,
                                 bool createDirectory)
{
    const auto &path = outputPath(tmpFilePath, sourceFilePath, lang, createDirectory);

    if (lang == "Java")
        return QDir(path).filePath(SettingsHelper::getLanguageConfig("Java").getClassName() + ".class");

    return path;
}

void Compiler::onProcessFinished(int exitCode, QProcess::ExitStatus e)
{
    QString codecName = "UTF-8";
    if (lang == "C++" || lang == "Java")
        codecName = SettingsHelper::getLanguageConfig(lang).getCompilerOutputCodec();
    QTextCodec *codec = QTextCodec::codecForName(codecName.toUtf8());
    if (!codec)
        codec = QTextCodec::codecForName("UTF-8");
    QString output = codec->toUnicode(compileProcess->readAllStandardError());
    // emit different signals due to different exit codes
    if (exitCode == 0)
        emit compilationFinished(output);
    else
        emit compilationErrorOccurred(output);
}

void Compiler::onProcessErrorOccurred(QProcess::ProcessError error)
{
    LOG_WARN(INFO_OF(error));
    if (error == QProcess::FailedToStart)
    {
        emit compilationFailed(
            tr("Failed to start the compiler. Please check %1 or add the compiler in the PATH environment variable.")
                .arg(SettingsHelper::getLanguageConfig(lang).pathOfCompileCommand()));
    }
}

} // namespace Core

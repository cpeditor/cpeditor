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
 * causes your ratings to go down and or lose any important contest.
 *
 * Believe Software is "Software" and it isn't immune to bugs.
 *
 */

#include "Core/Checker.hpp"
#include "Core/EventLogger.hpp"
#include "Util.hpp"
#include <QFile>

namespace Core
{

Checker::Checker(CheckerType type, MessageLogger *logger, int timeLimit, QObject *parent) : QObject(parent)
{
    Log::i("Checker/constructor") << INFO_OF(type) << endl;
    checkerType = type;
    log = logger;
    this->timeLimit = timeLimit;
}

Checker::Checker(const QString &path, MessageLogger *logger, int timeLimit, QObject *parent)
    : Checker(Custom, logger, timeLimit, parent)
{
    checkerPath = path;
}

Checker::~Checker()
{
    Log::i("Checker/destructor", "Invoked");
    if (compiler)
        delete compiler;
    for (auto &t : runner)
        delete t;
    if (tmpDir)
        delete tmpDir;
}

void Checker::prepare(const QString &command)
{
    for (auto &t : runner)
        delete t;
    if (compiler)
        delete compiler;
    runner.clear();
    pendingTasks.clear();

    if (!compiled)
    {
        QString checkerResource;

        switch (checkerType)
        {
        case IgnoreTrailingSpaces:
        case Strict:
            onCompilationFinished();
            return;
        case Ncmp:
            checkerResource = ":/testlib/checkers/ncmp.cpp";
            break;
        case Rcmp4:
            checkerResource = ":/testlib/checkers/rcmp4.cpp";
            break;
        case Rcmp6:
            checkerResource = ":/testlib/checkers/rcmp6.cpp";
            break;
        case Rcmp9:
            checkerResource = ":/testlib/checkers/rcmp9.cpp";
            break;
        case Wcmp:
            checkerResource = ":/testlib/checkers/wcmp.cpp";
            break;
        case Nyesno:
            checkerResource = ":/testlib/checkers/nyesno.cpp";
            break;
        case Custom:
            checkerResource = checkerPath;
            break;
        }

        QFile checkerResourceReader(checkerResource);
        if (!checkerResourceReader.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            log->error("Checker", "Failed to read the checker from [" + checkerResource + "]");
            return;
        }
        QString checkerCode = checkerResourceReader.readAll();

        tmpDir = new QTemporaryDir();
        if (!tmpDir->isValid())
        {
            log->error("Checker", "Failed to create temporary directory");
            return;
        }

        checkerPath = tmpDir->filePath("checker.cpp");
        if (!Util::saveFile(checkerPath, checkerCode, "Checker", false, log))
            return;

        QFile testlibReader(":/testlib/testlib.h");
        if (!testlibReader.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            log->error("Checker", "Failed to read testlib.h from resource");
            return;
        }
        auto testlib_h = testlibReader.readAll();
        if (!Util::saveFile(tmpDir->filePath("testlib.h"), testlib_h, "Checker", false, log))
            return;

        compiler = new Compiler();
        connect(compiler, SIGNAL(compilationFinished(const QString &)), this, SLOT(onCompilationFinished()));
        connect(compiler, SIGNAL(compilationErrorOccured(const QString &)), this,
                SLOT(onCompilationErrorOccured(const QString &)));
        connect(compiler, SIGNAL(compilationKilled()), this, SLOT(onCompilationKilled()));
        compiler->start(checkerPath, command, "C++");
    }
}

void Checker::reqeustCheck(int index, const QString &input, const QString &output, const QString &expected)
{
    if (compiled)
        check(index, input, output, expected);
    else
        pendingTasks.push_back({index, input, output, expected});
}

void Checker::onCompilationFinished()
{
    Log::i("Checker/onCompilationFinished", "Invoked");
    compiled = true;
    for (auto t : pendingTasks)
        check(t.index, t.input, t.output, t.expected);
    pendingTasks.clear();
}

void Checker::onCompilationErrorOccured(const QString &error)
{
    log->error("Checker", "Error occurred while compiling the checker:\n" + error);
}

void Checker::onCompilationKilled()
{
    disconnect(compiler, SIGNAL(compilationErrorOccured(const QString &)), this,
               SLOT(onCompilationErrorOccured(const QString &)));
}

void Checker::onRunFinished(int index, const QString &out, const QString &err, int exitCode, int timeUsed)
{
    if (exitCode == 0)
    {
        emit checkFinished(index, AC);
    }
    else if (QList<int>({1, 2, 3, 4, 5, 8, 16}).contains(exitCode)) // from testlib.h::TResult
    {
        if (err.isEmpty())
            log->error("Checker[" + QString::number(index + 1) + "]",
                       "Checker exited with exit code " + QString::number(exitCode));
        else
            log->error("Checker[" + QString::number(index + 1) + "]", err);
        emit checkFinished(index, WA);
    }
    else
    {
        log->error("Checker[" + QString::number(index + 1) + "]",
                   "Checker exited with unknown exit code " + QString::number(exitCode));
        if (!err.isEmpty())
            log->error("Checker[" + QString::number(index + 1) + "]", err);
    }
}

void Checker::onRunErrorOccured(int index, const QString &error)
{
    log->error("Checker[" + QString::number(index + 1) + "]", error);
}

void Checker::onRunTimeout(int index)
{
    log->warn("Checker[" + QString::number(index + 1) + "]", "Time Limit Exceeded");
}

void Checker::onRunKilled(int index)
{
    log->error("Checker[" + QString::number(index + 1) + "]", "Killed");
}

bool Checker::checkIgnoreTrailingSpaces(const QString &output, const QString &expected)
{
    Core::Log::i("Checker/checkIgnoreTrailingSpaces", "Invoked");
    auto out = output;
    out.replace("\r\n", "\n").replace("\r", "\n");
    auto ans = expected;
    ans.replace("\r\n", "\n").replace("\r", "\n");
    auto a_lines = out.split('\n');
    auto b_lines = ans.split('\n');
    while (!a_lines.isEmpty() && a_lines.back().trimmed().isEmpty())
        a_lines.pop_back();
    while (!b_lines.isEmpty() && b_lines.back().trimmed().isEmpty())
        b_lines.pop_back();
    if (a_lines.size() != b_lines.size())
        return false;
    for (int i = 0; i < a_lines.size(); ++i)
    {
        auto a_line = a_lines[i];
        auto b_line = b_lines[i];
        while (!a_line.isEmpty() && a_line.back().isSpace())
            a_line.chop(1);
        while (!b_line.isEmpty() && b_line.back().isSpace())
            b_line.chop(1);
        if (a_line != b_line)
            return false;
    }
    return true;
}

bool Checker::checkStrict(const QString &output, const QString &expected)
{
    Log::i("Checker/checkStrict", "Invoked");
    auto a = output;
    auto b = expected;
    return a.replace("\r\n", "\n").replace("\r", "\n") == b.replace("\r\n", "\n").replace("\r", "\n");
}

void Checker::check(int index, const QString &input, const QString &output, const QString &expected)
{
    Log::i("Checker/check") << "Invoked. " << INFO_OF(index) << endl;
    switch (checkerType)
    {
    case IgnoreTrailingSpaces:
        emit checkFinished(index, checkIgnoreTrailingSpaces(output, expected) ? AC : WA);
        break;
    case Strict:
        emit checkFinished(index, checkStrict(output, expected) ? AC : WA);
        break;
    default:
        auto inputPath = tmpDir->filePath(QString::number(index) + ".in");
        auto outputPath = tmpDir->filePath(QString::number(index) + ".out");
        auto expectedPath = tmpDir->filePath(QString::number(index) + ".ans");
        if (Util::saveFile(inputPath, input, "Checker", false, log) &&
            Util::saveFile(outputPath, output, "Checker", false, log) &&
            Util::saveFile(expectedPath, expected, "Checker", false, log))
        {
            auto tmp = new Runner(index);
            runner.push_back(tmp);
            connect(tmp, SIGNAL(runFinished(int, const QString &, const QString &, int, int)), this,
                    SLOT(onRunFinished(int, const QString &, const QString &, int, int)));
            connect(tmp, SIGNAL(runErrorOccured(int, const QString &)), this,
                    SLOT(onRunErrorOccured(int, const QString &)));
            connect(tmp, SIGNAL(runTimeout(int)), this, SLOT(onRunTimeout(int)));
            connect(tmp, SIGNAL(runKilled(int)), this, SLOT(onRunKilled(int)));
            tmp->run(checkerPath, "C++", "", "\"" + inputPath + "\" \"" + outputPath + "\" \"" + expectedPath + "\"",
                     "", timeLimit);
        }
        break;
    }
}

} // namespace Core
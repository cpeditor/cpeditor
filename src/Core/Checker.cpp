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

void Checker::prepare(const QString &compileCommand)
{
    // clear everything
    for (auto &t : runner)
        delete t;
    if (compiler)
        delete compiler;
    runner.clear();
    pendingTasks.clear();

    if (!compiled)
    {
        // compile the checker if it's not compiled

        QString checkerResource;

        // get the checker resource
        switch (checkerType)
        {
        case IgnoreTrailingSpaces:
        case Strict:
            onCompilationFinished(); // terminate compilation if this is a built-in checker
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

        // get the code of the checker
        QFile checkerResourceReader(checkerResource);
        if (!checkerResourceReader.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            log->error("Checker", "Failed to read the checker from [" + checkerResource + "]");
            return;
        }
        QString checkerCode = checkerResourceReader.readAll();

        // create a temporary directory
        tmpDir = new QTemporaryDir();
        if (!tmpDir->isValid())
        {
            log->error("Checker", "Failed to create temporary directory");
            return;
        }

        // save the checker source file on the disk
        checkerPath = tmpDir->filePath("checker.cpp");
        if (!Util::saveFile(checkerPath, checkerCode, "Checker", false, log))
            return;

        // save testlib.h on the disk
        QFile testlibReader(":/testlib/testlib.h");
        if (!testlibReader.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            log->error("Checker", "Failed to read testlib.h from resource");
            return;
        }
        auto testlib_h = testlibReader.readAll();
        if (!Util::saveFile(tmpDir->filePath("testlib.h"), testlib_h, "Checker", false, log))
            return;

        // start the compilation of the checker
        compiler = new Compiler();
        connect(compiler, SIGNAL(compilationFinished(const QString &)), this, SLOT(onCompilationFinished()));
        connect(compiler, SIGNAL(compilationErrorOccured(const QString &)), this,
                SLOT(onCompilationErrorOccured(const QString &)));
        connect(compiler, SIGNAL(compilationKilled()), this, SLOT(onCompilationKilled()));
        compiler->start(checkerPath, compileCommand, "C++");
    }
}

void Checker::reqeustCheck(int index, const QString &input, const QString &output, const QString &expected)
{
    if (compiled)
        check(index, input, output, expected); // check immediately if the checker is compiled
    else
        pendingTasks.push_back({index, input, output, expected}); // otherwise push it into the pending tasks list
}

void Checker::onCompilationFinished()
{
    Log::i("Checker/onCompilationFinished", "Invoked");
    compiled = true; // mark that the checker is compiled
    for (auto t : pendingTasks)
        check(t.index, t.input, t.output, t.expected); // solve the pending tasks
    pendingTasks.clear();
}

void Checker::onCompilationErrorOccured(const QString &error)
{
    log->error("Checker", "Error occurred while compiling the checker:\n" + error);
}

void Checker::onCompilationKilled()
{
    // It will be confusing to show "the checker failed" when it's killed,
    // but the user is also unlikely willing to see the message "the checker compilation is killed",
    // so we can simply show nothing when the compilation is killed
    disconnect(compiler, SIGNAL(compilationErrorOccured(const QString &)), this,
               SLOT(onCompilationErrorOccured(const QString &)));
}

void Checker::onRunFinished(int index, const QString &out, const QString &err, int exitCode)
{
    if (exitCode == 0)
    {
        // the check process succeeded
        emit checkFinished(index, AC);
    }
    else if (QList<int>({1, 2, 3, 4, 5, 8, 16}).contains(exitCode)) // this list is from testlib.h::TResult
    {
        // This exit code is a normal exit code of a testlib checker, means WA or something else
        if (err.isEmpty())
            log->error("Checker[" + QString::number(index + 1) + "]",
                       "Checker exited with exit code " + QString::number(exitCode));
        else
            log->error("Checker[" + QString::number(index + 1) + "]", err);
        emit checkFinished(index, WA);
    }
    else
    {
        // This exit code is not one of the normal exit codes of a testlib checker, maybe the checker crashed
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

    // first, replace \r\n and \r by \n
    auto out = output;
    out.replace("\r\n", "\n").replace("\r", "\n");
    auto ans = expected;
    ans.replace("\r\n", "\n").replace("\r", "\n");

    // split output and answer into lines
    auto outputLines = out.split('\n');
    auto answerLines = ans.split('\n');

    // remove trailing empty lines
    while (!outputLines.isEmpty() && outputLines.back().trimmed().isEmpty())
        outputLines.pop_back();
    while (!answerLines.isEmpty() && answerLines.back().trimmed().isEmpty())
        answerLines.pop_back();

    // if they are considered the same, they must have the same number of lines after removing trailing empty lines
    if (outputLines.size() != answerLines.size())
        return false;

    for (int i = 0; i < outputLines.size(); ++i)
    {
        auto outputLine = outputLines[i];
        auto answerLine = answerLines[i];

        // remove trailing spaces at the end of the current line
        while (!outputLine.isEmpty() && outputLine.back().isSpace())
            outputLine.chop(1);
        while (!answerLine.isEmpty() && answerLine.back().isSpace())
            answerLine.chop(1);

        // if they are considered the same, the current line should be exactly the same after removing trailing spaces
        if (outputLine != answerLine)
            return false;
    }

    // all tests are passed, this output is accepted
    return true;
}

bool Checker::checkStrict(const QString &output, const QString &expected)
{
    Log::i("Checker/checkStrict", "Invoked");
    auto a = output;
    auto b = expected;
    // replace \r\n and \r with \n, then directly compare them
    return a.replace("\r\n", "\n").replace("\r", "\n") == b.replace("\r\n", "\n").replace("\r", "\n");
}

void Checker::check(int index, const QString &input, const QString &output, const QString &expected)
{
    Log::i("Checker/check") << "Invoked. " << INFO_OF(index) << endl;
    switch (checkerType)
    {
    // check directly if it's a built-in checker
    case IgnoreTrailingSpaces:
        emit checkFinished(index, checkIgnoreTrailingSpaces(output, expected) ? AC : WA);
        break;
    case Strict:
        emit checkFinished(index, checkStrict(output, expected) ? AC : WA);
        break;
    default:
        // if it's a testlib checker, save the input, output and expected files first
        auto inputPath = tmpDir->filePath(QString::number(index) + ".in");
        auto outputPath = tmpDir->filePath(QString::number(index) + ".out");
        auto expectedPath = tmpDir->filePath(QString::number(index) + ".ans");
        if (Util::saveFile(inputPath, input, "Checker", false, log) &&
            Util::saveFile(outputPath, output, "Checker", false, log) &&
            Util::saveFile(expectedPath, expected, "Checker", false, log))
        {
            // if files are successfully saved, run the checker
            auto tmp = new Runner(index);
            runner.push_back(tmp); // save the checkers in a list, so we can delete them when destructing the checker
            connect(tmp, SIGNAL(runFinished(int, const QString &, const QString &, int, int)), this,
                    SLOT(onRunFinished(int, const QString &, const QString &, int)));
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
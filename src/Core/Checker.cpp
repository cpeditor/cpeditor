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

#include "Core/Checker.hpp"
#include "Core/Compiler.hpp"
#include "Core/EventLogger.hpp"
#include "Core/MessageLogger.hpp"
#include "Core/Runner.hpp"
#include "Util/FileUtil.hpp"
#include "generated/SettingsHelper.hpp"
#include <QFile>
#include <QTemporaryDir>

namespace Core
{

Checker::Checker(CheckerType type, MessageLogger *logger, QObject *parent)
    : QObject(parent), checkerType(type), log(logger), compiled(false)
{
    LOG_INFO("Checker of type " << type << "created");
    switch (checkerType)
    {
    case IgnoreTrailingSpaces:
    case Strict:
    case Custom:
        break;
    case Ncmp:
        checkerOriginalPath = ":/testlib/checkers/ncmp.cpp";
        break;
    case Rcmp4:
        checkerOriginalPath = ":/testlib/checkers/rcmp4.cpp";
        break;
    case Rcmp6:
        checkerOriginalPath = ":/testlib/checkers/rcmp6.cpp";
        break;
    case Rcmp9:
        checkerOriginalPath = ":/testlib/checkers/rcmp9.cpp";
        break;
    case Wcmp:
        checkerOriginalPath = ":/testlib/checkers/wcmp.cpp";
        break;
    case Nyesno:
        checkerOriginalPath = ":/testlib/checkers/nyesno.cpp";
        break;
    }
}

Checker::Checker(const QString &path, MessageLogger *logger, QObject *parent) : Checker(Custom, logger, parent)
{
    checkerOriginalPath = path;
    LOG_INFO("Updated checker path to " << path);
}

Checker::~Checker()
{
    delete compiler;
    for (auto &t : runners)
        delete t;
    delete tmpDir;
    LOG_INFO("Destroyed checker of type " << checkerType);
}

void Checker::prepare()
{
    if (checkerType == IgnoreTrailingSpaces || checkerType == Strict)
    {
        compiled = true;
        return;
    }

    compiled = false;

    checkerCode = Util::readFile(checkerOriginalPath, tr("Read Checker"), log);
    if (checkerCode.isNull())
        return;

    tmpDir = new QTemporaryDir();
    if (!tmpDir->isValid())
    {
        log->error(tr("Checker"), tr("Failed to create temporary directory"));
        return;
    }

    checkerTmpPath = tmpDir->filePath("checker.cpp");
    if (!Util::saveFile(checkerTmpPath, checkerCode, tr("Checker"), false, log))
        return;

    auto testlib_h = Util::readFile(":/testlib/testlib.h", tr("Read testlib.h"), log);
    if (testlib_h.isNull())
        return;
    if (!Util::saveFile(tmpDir->filePath("testlib.h"), testlib_h, tr("Save testlib.h"), false, log))
        return;

    delete compiler;
    compiler = new Compiler();
    connect(compiler, &Compiler::compilationStarted, this, &Checker::onCompilationStarted);
    connect(compiler, &Compiler::compilationFinished, this, &Checker::onCompilationFinished);
    connect(compiler, &Compiler::compilationErrorOccurred, this, &Checker::onCompilationErrorOccurred);
    connect(compiler, &Compiler::compilationFailed, this, &Checker::onCompilationFailed);
    connect(compiler, &Compiler::compilationKilled, this, &Checker::onCompilationKilled);
    compiler->start(checkerTmpPath, "", SettingsHelper::getCppCompileCommand(), "");
}

void Checker::reqeustCheck(int index, const QString &input, const QString &output, const QString &expected)
{
    if (!isLatest())
    {
        LOG_INFO("Recompiling checker");
        prepare();
    }
    LOG_INFO(BOOL_INFO_OF(compiled));
    if (compiled)
        check(index, input, output, expected); // check immediately if the checker is compiled
    else
        pendingTasks.push_back({index, input, output, expected}); // otherwise push it into the pending tasks list
}

void Checker::onCompilationStarted()
{
    log->info(tr("Checker"), tr("Started compiling the checker"));
}

void Checker::onCompilationFinished()
{
    if (!isLatest())
    {
        prepare();
        return;
    }
    compiled = true;
    log->info(tr("Checker"), tr("The checker is compiled"));
    for (auto const &t : pendingTasks)
        check(t.index, t.input, t.output, t.expected); // solve the pending tasks
    pendingTasks.clear();
}

void Checker::onCompilationErrorOccurred(const QString &error)
{
    log->error(tr("Checker"), tr("Error occurred while compiling the checker:\n%1").arg(error));
}

void Checker::onCompilationFailed(const QString &reason)
{
    log->error(tr("Checker"), tr("Failed to compile the checker: %1").arg(reason), false);
}

void Checker::onCompilationKilled()
{
    // It will be confusing to show "the checker failed" when it's killed,
    // but the user is also unlikely willing to see the message "the checker compilation is killed",
    // so we can simply show nothing when the compilation is killed
    disconnect(compiler, &Compiler::compilationErrorOccurred, this, &Checker::onCompilationErrorOccurred);
}

void Checker::onRunFinished(int index, const QString & /*unused*/, const QString &err, int exitCode, int /*unused*/,
                            bool tle)
{
    if (tle)
        log->warn(head(index), tr("Time Limit Exceeded"));

    switch (TResult(exitCode))
    {
    case _ok:
        if (!err.isEmpty())
            log->message(head(index), err, "green");
        emit checkFinished(index, Widgets::TestCase::AC);
        return;

    case _wa:
    case _pe:
    case _fail:
    case _dirt:
    case _points:
    case _unexpected_eof:
    case _partially:
        if (err.isEmpty())
            log->error(head(index), tr("Checker exited with exit code %1").arg(exitCode));
        else
            log->error(head(index), err);
        emit checkFinished(index, Widgets::TestCase::WA);
        return;

        // use return in each case with no default case and handle other exit codes below
        // so that if there are unhandled enums there's a compilation warning (-Wswitch)
    }

    // This exit code is not one of the normal exit codes of a testlib checker, maybe the checker crashed
    log->error(head(index), tr("Checker exited with unknown exit code %1").arg(exitCode));
    if (!err.isEmpty())
        log->error(head(index), err);
}

void Checker::onFailedToStartRun(int index, const QString &error)
{
    log->error(head(index), error, false);
}

void Checker::onRunOutputLimitExceeded(int index, const QString &type)
{
    log->warn(
        head(index),
        tr("The %1 of the process running on the testcase #%2 contains more than %3 characters, which is longer "
           "than the output length limit, so the process is killed. You can change the output length limit at %4.")
            .arg(type)
            .arg(index + 1)
            .arg(SettingsHelper::getOutputLengthLimit())
            .arg(SettingsHelper::pathOfOutputLengthLimit()),
        false);
}

void Checker::onRunKilled(int index)
{
    log->error(head(index), tr("The checker is killed"));
}

bool Checker::checkIgnoreTrailingSpaces(const QString &output, const QString &expected)
{
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
    auto a = output;
    auto b = expected;
    // replace \r\n and \r with \n, then directly compare them
    return a.replace("\r\n", "\n").replace("\r", "\n") == b.replace("\r\n", "\n").replace("\r", "\n");
}

void Checker::check(int index, const QString &input, const QString &output, const QString &expected)
{
    LOG_INFO(INFO_OF(index));
    switch (checkerType)
    {
    // check directly if it's a built-in checker
    case IgnoreTrailingSpaces:
        emit checkFinished(index,
                           checkIgnoreTrailingSpaces(output, expected) ? Widgets::TestCase::AC : Widgets::TestCase::WA);
        break;
    case Strict:
        emit checkFinished(index, checkStrict(output, expected) ? Widgets::TestCase::AC : Widgets::TestCase::WA);
        break;
    default:
        // if it's a testlib checker, save the input, output and expected files first
        auto inputPath = tmpDir->filePath(QString::number(index) + ".in");
        auto outputPath = tmpDir->filePath(QString::number(index) + ".out");
        auto expectedPath = tmpDir->filePath(QString::number(index) + ".ans");
        if (Util::saveFile(inputPath, input, tr("Checker"), false, log) &&
            Util::saveFile(outputPath, output, tr("Checker"), false, log) &&
            Util::saveFile(expectedPath, expected, tr("Checker"), false, log))
        {
            // if files are successfully saved, run the checker
            auto *tmp = new Runner(index);
            runners.push_back(tmp); // save the checkers in a list, so we can delete them when destructing the checker
            connect(tmp, &Runner::runFinished, this, &Checker::onRunFinished);
            connect(tmp, &Runner::failedToStartRun, this, &Checker::onFailedToStartRun);
            connect(tmp, &Runner::runOutputLimitExceeded, this, &Checker::onRunOutputLimitExceeded);
            connect(tmp, &Runner::runKilled, this, &Checker::onRunKilled);
            tmp->run(checkerTmpPath, "", "C++", "",
                     "\"" + inputPath + "\" \"" + outputPath + "\" \"" + expectedPath + "\"", "",
                     SettingsHelper::getDefaultTimeLimit());
        }
        break;
    }
}

QString Checker::head(int index)
{
    return tr("Checker[%1]").arg(index + 1);
}

bool Checker::isLatest()
{
    if (checkerOriginalPath.isEmpty())
        return true;
    const QString currentCheckerCode = Util::readFile(checkerOriginalPath, "Read Checker", log);
    return currentCheckerCode.isNull() || currentCheckerCode == checkerCode;
}

} // namespace Core

/*
 * Copyright (C) 2019-2020 Ashar Khan <ashar786khan@gmail.com>
 *
 * This file is part of cpeditor.
 *
 * cpeditor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * I will not be responsible if cpeditor behaves in unexpected way and
 * causes your ratings to go down and or lose any important contest.
 *
 * Believe Software is "Software" and it isn't immune to bugs.
 *
 */

#ifndef CHECKER_HPP
#define CHECKER_HPP

#include "Core/Compiler.hpp"
#include "Core/MessageLogger.hpp"
#include "Core/Runner.hpp"
#include <QTemporaryDir>

namespace Core
{

class Checker : public QObject
{
    Q_OBJECT

  public:
    enum CheckerType
    {
        IgnoreTrailingSpaces,
        Strict,
        Ncmp,
        Rcmp4,
        Rcmp6,
        Rcmp9,
        Wcmp,
        Nyesno,
        Custom
    };

    enum Verdict
    {
        AC,
        WA,
        UNKNOWN
    };

    Checker(CheckerType type, MessageLogger *logger, int timeLimit, QObject *parent);
    Checker(const QString &path, MessageLogger *logger, int timeLimit, QObject *parent);
    ~Checker();
    void prepare(const QString &command);
    void reqeustCheck(int index, const QString &input, const QString &output, const QString &expected);

  signals:
    void checkFinished(int index, Verdict verdict);

  private slots:
    void onCompilationFinished();
    void onCompilationErrorOccured(const QString &error);
    void onCompilationKilled();

    void onRunFinished(int index, const QString &out, const QString &err, int exitCode, int timeUsed);
    void onRunErrorOccured(int index, const QString &error);
    void onRunTimeout(int index);
    void onRunKilled(int index);

  private:
    bool checkIgnoreTrailingSpaces(const QString &output, const QString &expected);
    bool checkStrict(const QString &output, const QString &expected);
    void check(int index, const QString &input, const QString &output, const QString &expected);

    struct Task
    {
        int index;
        QString input, output, expected;
    };

    CheckerType checkerType;
    QString checkerPath;
    int timeLimit;
    QTemporaryDir *tmpDir = nullptr;
    MessageLogger *log = nullptr;
    Compiler *compiler = nullptr;
    QVector<Runner *> runner;
    QVector<Task> pendingTasks;
    bool compiled = false;
};

} // namespace Core

#endif // CHECKER_HPP
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

/*
 * The checker is used to check whether an output is accepted.
 * You can send the actual output and the expected output to a checker,
 * and the checker will tell you whether this output is accepted or not.
 * The checker should be setup before required to check outputs, and the
 * response is not always immediate.
 * The official testlib checkers are saved in the Qt Resources, and are
 * compiled during the runtime.
 */

#ifndef CHECKER_HPP
#define CHECKER_HPP

#include "Widgets/TestCase.hpp"

class QTemporaryDir;
class MessageLogger;

namespace Core
{

class Compiler;
class Runner;

class Checker : public QObject
{
    Q_OBJECT

  public:
    // The type of a checker
    enum CheckerType
    {
        /* built-in checkers */
        IgnoreTrailingSpaces, // Ignore blank characters at the end of lines and blank lines at the end.
        Strict,               // White space differences matters, except the differences between \n, \r and \r\n
        /* testlib checkers */
        Ncmp,   // ncmp.cpp in testlib, compare ordered sequences of signed int64 numbers
        Rcmp4,  // rcmp4.cpp in testlib, compare two sequences of doubles, max absolute or relative error = 1e-4
        Rcmp6,  // rcmp6.cpp in testlib, compare two sequences of doubles, max absolute or relative error = 1e-6
        Rcmp9,  // rcmp9.cpp in testlib, compare two sequences of doubles, max absolute or relative error = 1e-9
        Wcmp,   // wcmp.cpp in testlib, compare sequences of tokens
        Nyesno, // nyesno.cpp in testlib, multiple YES/NO (case insensetive)
        /* custom checkers */
        Custom // a local custom checker chose by the user
    };

    /**
     * @brief construct a checker
     * @param type the type of the checker
     * @param logger the message logger that receives the messages
     * @param parent the parent of a QObject
     * @note Don't construct a custom checker by this.
     */
    Checker(CheckerType type, MessageLogger *logger, QObject *parent = nullptr);

    /**
     * @brief construct a custom checker
     * @param path the file path to the custom checker
     * @param logger the message logger that receives the messages
     * @param parent the parent of a QObject
     */
    Checker(const QString &path, MessageLogger *logger, QObject *parent = nullptr);

    /**
     * @brief destruct a checker
     * @note Tt kills the running compiler and checker, removes the temporary directory.
     */
    ~Checker() override;

    /**
     * @brief prepare for checking
     * @note Testlib checkers will be compiled after calling this function. This should be called only once.
     */
    void prepare();

    /**
     * @brief request the checker to check a testcase
     * @param index the index of this testcase, used in messages and the result signals
     * @param input the input of the testcase, not used in the built-in checkers
     * @param output the output to check
     * @param expected the expected output of the testcase
     * @note This function doesn't return anything, it request the checker to check,
     *       and the checker emits a signal when it's done
     */
    void reqeustCheck(int index, const QString &input, const QString &output, const QString &expected);

    /**
     * @brief clear the pending tasks and kill executing tasks
     */
    void clearTasks();

  signals:
    /**
     * @brief return the check result
     * @param index the index of the checked testcase
     * @param verdict the result of this check
     */
    void checkFinished(int index, Widgets::TestCase::Verdict verdict);

  private slots:
    void onCompilationStarted();

    void onCompilationFinished();

    void onCompilationErrorOccurred(const QString &error);

    void onCompilationFailed(const QString &reason);

    void onCompilationKilled();

    void onRunFinished(int index, const QString &, const QString &err, int exitCode, int, bool tle);

    void onFailedToStartRun(int index, const QString &error);

    void onRunOutputLimitExceeded(int index, const QString &type);

    void onRunKilled(int index);

  private:
    /**
     * @brief check the output against the expected output in IgnoreTrailingSpaces mode
     * @param output the output to check
     * @param expected the expected output to check the output against
     * @return whether this output is accepted or not
     */
    static bool checkIgnoreTrailingSpaces(const QString &output, const QString &expected);

    /**
     * @brief check the output against the expected output in Strict mode
     * @param output the output to check
     * @param expected the expected output to check the output against
     * @return whether this output is accepted or not
     */
    static bool checkStrict(const QString &output, const QString &expected);

    /**
     * @brief check a testcase
     * @param input the input of the testcase
     * @param output the output to check
     * @param expected the expected output of the testcase
     * @note this should only be called when the checker is compiled
     */
    void check(int index, const QString &input, const QString &output, const QString &expected);

    /**
     * @param index the index of the testcase
     * @returns "Checker[*index*]"
     */
    static QString head(int index);

    /**
     * @returns if checker is changed, it starts recompilation and returns true; otherwise, returns false
     */
    bool recompileIfChanged();

    // a struct with the info of a testcase, or called a check task, used to save check requests
    struct Task
    {
        int index;
        QString input, output, expected;
    };

    // copied from testlib.h, see #746 for why not include testlib.h
    enum TResult
    {
        _ok = 0,
        _wa = 1,
        _pe = 2,
        _fail = 3,
        _dirt = 4,
        _points = 5,
        _unexpected_eof = 8,
        _partially = 16
    };

    CheckerType checkerType;         // the type of the checker
    QString checkerTmpPath;          // the file path to checker file in the temp dir
    QString checkerOriginalPath;     // the path to the original checker
    QString checkerCode;             // the source code of the checker
    QTemporaryDir *tmpDir = nullptr; // the temp directory to save the I/O files, testlib.h and the compiled checker
                                     // It's not needed by built-in checkers
    MessageLogger *log = nullptr;    // the message logger to show messages to the user
    Compiler *compiler = nullptr;    // the compiler used to compile the checker
    QVector<Runner *> runners;       // the runners used to run the check processes
    QVector<Task> pendingTasks;      // the unsolved check requests
    std::atomic<bool> compiled;      // whether the testlib checker is compiled or not
                                     // It should be true for built-in checkers.
};

} // namespace Core

#endif // CHECKER_HPP

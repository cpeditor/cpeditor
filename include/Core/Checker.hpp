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

    // The result of a check
    enum Verdict
    {
        AC,     // Accepted
        WA,     // Wrong answer
        UNKNOWN // Used when the checker hasn't finished checking, or the checker failed
                // Usually the checker doesn't return UNKNOWN, the verdict is set to UNKNOWN before the check begins
    };

    /*
     * @brief construct a checker
     * @param type the type of the checker
     * @param logger the message logger that receives the messages
     * @param parent the parent of a QObject
     * @note Don't construct a custom checker by this.
     */
    Checker(CheckerType type, MessageLogger *logger, QObject *parent = nullptr);

    /*
     * @brief construct a custom checker
     * @param path the file path to the custom checker
     * @param logger the message logger that receives the messages
     * @param parent the parent of a QObject
     */
    Checker(const QString &path, MessageLogger *logger, QObject *parent = nullptr);

    /*
     * @brief destruct a checker
     * @note Tt kills the running compiler and checker, removes the temporary directory.
     */
    ~Checker();

    /*
     * @brief prepare for checking
     * @param compileCommand the command used to compile the checker
     * @note Testlib checkers will be compiled after calling this function. This should be called only once.
     */
    void prepare(const QString &compileCommand);

    /*
     * @brief request the checker to check a testcase
     * @param index the index of this testcase, used in messages and the result signals
     * @param input the input of the testcase, not used in the built-in checkers
     * @param output the output to check
     * @param expected the expected output of the testcase
     * @note This function doesn't return anything, it request the checker to check,
     *       and the checker emits a signal when it's done
     */
    void reqeustCheck(int index, const QString &input, const QString &output, const QString &expected);

  signals:
    /*
     * @brief return the check result
     * @param index the index of the checked testcase
     * @param verdict the result of this check
     */
    void checkFinished(int index, Verdict verdict);

  private slots:
    /*
     * @brief the checker is compiled successfully
     */
    void onCompilationFinished();

    /*
     * @brief failed to compile the checker
     * @param error the error message provided by Core::Compiler
     */
    void onCompilationErrorOccured(const QString &error);

    /*
     * @brief the compile process is killed
     */
    void onCompilationKilled();

    /*
     * @brief the checker process is finished
     * @param index the index of the testcase
     * @param out the stdout of the checker process
     * @param err the stderr of the checker process
     * @param exitCode the exit code of the checker process
     */
    void onRunFinished(int index, const QString &out, const QString &err, int exitCode);

    /*
     * @brief the checker failed to start
     * @param index the index of the testcase
     * @param error the error message provided by Core::Runner
     */
    void onFailedToStartRun(int index, const QString &error);

    /*
     * @brief the checker hasn't finished in the time limit
     * @param index the index of the testcase
     */
    void onRunTimeout(int index);

    /*
     * @brief the checker process is killed
     * @param index the index of the testcase
     */
    void onRunKilled(int index);

  private:
    /*
     * @brief check the output against the expected output in IgnoreTrailingSpaces mode
     * @param output the output to check
     * @param expected the expected output to check the output against
     * @return whether this output is accepted or not
     */
    bool checkIgnoreTrailingSpaces(const QString &output, const QString &expected);

    /*
     * @brief check the output against the expected output in Strict mode
     * @param output the output to check
     * @param expected the expected output to check the output against
     * @return whether this output is accepted or not
     */
    bool checkStrict(const QString &output, const QString &expected);

    /*
     * @brief check a testcase
     * @param input the input of the testcase
     * @param output the output to check
     * @param expected the expected output of the testcase
     * @note this should only be called when the checker is compiled
     */
    void check(int index, const QString &input, const QString &output, const QString &expected);

    // a struct with the info of a testcase, or called a check task, used to save check requests
    struct Task
    {
        int index;
        QString input, output, expected;
    };

    CheckerType checkerType;         // the type of the checker
    QString checkerPath;             // the file path to the custom checker
    QTemporaryDir *tmpDir = nullptr; // the temp directory to save the I/O files, testlib.h and the compiled checker
                                     // It's not needed by built-in checkers
    MessageLogger *log = nullptr;    // the message logger to show messages to the user
    Compiler *compiler = nullptr;    // the compiler used to compile the checker
    QVector<Runner *> runner;        // the runners used to run the check processes
    QVector<Task> pendingTasks;      // the unsolved check requests
    bool compiled = false;           // whether the testlib checker is compiled or not
                                     // It should be true for built-in checkers.
};

} // namespace Core

#endif // CHECKER_HPP
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
 * The Runner accepts an executable file, run it and return the result.
 * The source file should be compiled by Core::Compiler before the execution.
 * You have to create a new Runner for each execution.
 * The results are returned by signals.
 */

#ifndef RUNNER_HPP
#define RUNNER_HPP

#include <QElapsedTimer>
#include <QProcess>
#include <QTimer>

namespace Core
{

class Runner : public QObject
{
    Q_OBJECT

  public:
    /*
     * @brief construct a runner
     * @param index the index of the testcase
     */
    explicit Runner(int index);

    /*
     * @brief descruct the runner
     * @note the process will be killed if it's still running
     */
    ~Runner();

    /*
     * @brief run a program on a given input
     * @param filePath the path to the source file
     * @param lang the language to run, one of "C++", "Java" and "Python"
     * @param runCommand the command for running a program
     * @param args the command line arguments added at the back to start the program
     * @param input the input to the program
     * @param timeLimit the maximum time for the program to run, in milliseconds
     * @note This should be called only once. Please create multiple Runners for multiple runs.
     */
    void run(const QString &filePath, const QString &lang, const QString &runCommand, const QString &args,
             const QString &input, int timeLimit);

    /*
     * @brief run a program in a pop-up terminal
     * @param filePath the path to the source file
     * @param lang the language to run, one of "C++", "Java" and "Python"
     * @param runCommand the command for running a program
     * @param args the command line arguments added at the back to start the program
     * @note runFinished, runTimeout and runKilled won't be emitted when using runDetached. failedToStartRun will only
     *       be emitted when xterm is not installed on Linux, it's not emitted even the source file is not compiled.
     */
    void runDetached(const QString &filePath, const QString &lang, const QString &runCommand, const QString &args);

  signals:
    /*
     * @brief the execution has just started
     * @param index the index of the testcase
     */
    void runStarted(int index);

    /*
     * @brief the execution has just finished
     * @param index the idnex of the testcase
     * @param out the stdout of the program
     * @param err the stderr of the program
     * @param exitCode the exit code of the program
     * @param timeUsed the time between the execution started and finished
     */
    void runFinished(int index, const QString &out, const QString &err, int exitCode, int timeUsed);

    /*
     * @brief failed to start the execution
     * @param index the index of the testcase
     * @param error a string to describe the error
     */
    void failedToStartRun(int index, const QString &error);

    /*
     * @brief the execution reached the time limit
     * @param index the index of the testcase
     */
    void runTimeout(int index);

    /*
     * @brief the program is killed
     * @param index the index of the testcase
     * @note It's only emitted when the process is killed when destructing the Runner.
     */
    void runKilled(int index);

  private slots:
    /*
     * @brief the process is finished
     * @param exitCode the exit code of the process
     * @param exitStatus the exit status of the process
     */
    void onFinished(int exitCode, QProcess::ExitStatus exitStatus);

    /*
     * @brief the process has just started
     */
    void onStarted();

    /*
     * @brief the time limit is reached
     * @param this will kill the process if it's still running and emit runTimeout
     */
    void onTimeout();

  private:
    /*
     * @brief get the command to run a program
     * @param filePath the path to the source file
     * @param lang the language to run, one of "C++", "Java" and "Python"
     * @param runCommand the command for running a program
     * @param args the command line arguments added at the back to start the program
     */
    QString getCommand(const QString &filePath, const QString &lang, const QString &runCommand, const QString &args);

    const int runnerIndex;             // the index of the testcase
    QProcess *runProcess = nullptr;    // the process to run the program
    QTimer *killTimer = nullptr;       // the timer used to kill the process when the time limit is reached
    QElapsedTimer *runTimer = nullptr; // the timer used to measure how much time did the execution use
};

} // namespace Core

#endif // RUNNER_HPP
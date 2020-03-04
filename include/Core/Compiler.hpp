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
 * The Compiler accepts a file, compile it, and emit signal when finished.
 * It should be used only once in a life time, so if you want to compile multiple files,
 * please use multiple Compilers.
 * The compilation process will be automatically killed when the Compiler is destructed,
 * so it's convenient to use one Compiler for one compilation.
 * When using it to "compile" Python, it will emit compilationFinished("") immediately.
 */

#ifndef COMPILER_HPP
#define COMPILER_HPP

#include <QProcess>

namespace Core
{

class Compiler : public QObject
{
    Q_OBJECT

  public:
    /*
     * @brief construct a compiler
     */
    Compiler();

    /*
     * @brief destruct a compiler
     * @note the compilation process will be killed if it's running
     */
    ~Compiler();

    /*
     * @brief start the compilation
     * @param filePath the file path to the source file
     * @param compileCommand the command for compiling
     * @param lang the language to compile, one of "C++", "Java", "Python"
     * @note this should be called only once per Compiler
     */
    void start(const QString &filePath, const QString &compileCommand, const QString &lang);

    /*
     * @brief check whether a compile command is valid or not
     * @param compilaCommand the command to check
     * @note this only checks whether the program of the command works
     */
    static bool check(const QString &compileCommand);

  signals:
    /*
     * @brief the compilation has just started
     */
    void compilationStarted();

    /*
     * @brief the compilation has just finished
     * @param warning the compile warnings (stderr of the compile process)
     */
    void compilationFinished(const QString &warning);

    /*
     * @brief the compilation has just finished with error
     * @param error the compile errors and warnings (stderr of the compile process)
     */
    void compilationErrorOccured(const QString &error);

    /*
     * @brief the compilation process has just been killed
     */
    void compilationKilled();

  private slots:
    /*
     * @brief the compilation process has just finished
     * @param the exit code of the compilation process
     */
    void onProcessFinished(int exitCode);

  private:
    QProcess *compileProcess = nullptr; // the compilation process
};

} // namespace Core

#endif // COMPILER_HPP
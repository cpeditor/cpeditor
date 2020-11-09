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

#include <QObject>
#include <QProcess>

namespace Core
{

class Compiler : public QObject
{
    Q_OBJECT

  public:
    /**
     * @brief construct a compiler
     */
    Compiler();

    /**
     * @brief destruct a compiler
     * @note the compilation process will be killed if it's running
     */
    ~Compiler();

    /**
     * @brief start the compilation
     * @param tmpFilePath the path to the temporary file which is compiled
     * @param sourceFilePath the path to the original source file
     * @param compileCommand the command for compiling
     * @param lang the language to compile, one of "C++", "Java", "Python"
     * @note this should be called only once per Compiler
     */
    void start(const QString &tmpFilePath, const QString &sourceFilePath, const QString &compileCommand,
               const QString &lang);

    /**
     * @brief get the output path (executable file path for C++, class path for Java, tmp file path for Python)
     * @param tmpFilePath the path to the temporary file which is compiled
     * @param sourceFilePath the path to the original source file, if it's empty, tmpFilePath will be used instead of it
     * @param lang the language being compiled
     * @param create the directory if it doesn't exist
     */
    static QString outputPath(const QString &tmpFilePath, const QString &sourceFilePath, const QString &lang,
                              bool createDirectory = true);

    /**
     * @brief Similar to Compiler::outputPath, but returns the path of the output file.
     * i.e. with .exe on Windows for C++, class file instead of containing directory for Java
     */
    static QString outputFilePath(const QString &tmpFilePath, const QString &sourceFilePath, const QString &lang,
                                  bool createDirectory = true);

  signals:
    /**
     * @brief the compilation has just started
     */
    void compilationStarted();

    /**
     * @brief the compilation has just finished
     * @param warning the compile warnings (stderr of the compile process)
     */
    void compilationFinished(const QString &warning);

    /**
     * @brief the compilation has just finished with error
     * @param error the compile errors and warnings (stderr of the compile process)
     */
    void compilationErrorOccurred(const QString &error);

    /**
     * @brief failed to start compilation
     * @param reason the reason of the failure
     */
    void compilationFailed(const QString &reason);

    /**
     * @brief the compilation process has just been killed
     * @note It's only emitted when the process is killed when destructing the Compiler.
     */
    void compilationKilled();

  private slots:
    /**
     * @brief the compilation process has just finished
     * @param the exit code of the compilation process
     */
    void onProcessFinished(int exitCode);

    void onProcessErrorOccurred(QProcess::ProcessError error);

  private:
    QProcess *compileProcess = nullptr; // the compilation process
    QString lang;
};

} // namespace Core

#endif // COMPILER_HPP

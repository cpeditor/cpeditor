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

#ifndef COMPILER_HPP
#define COMPILER_HPP

#include <QProcess>

namespace Core
{

// Compiler accepts a file, compile it, emit compilationStarted(), and either compilationFinished(const QString
// &warning) or void compilationErrorOccured(const QString &error). Destructing it kills the compile process.

class Compiler : public QObject
{
    Q_OBJECT

  public:
    Compiler();
    ~Compiler();
    void start(const QString &filePath, const QString &compileCommand, const QString &lang);
    static bool check(const QString &compileCommand);

  signals:
    void compilationStarted();
    void compilationFinished(const QString &warning);
    void compilationErrorOccured(const QString &error);
    void compilationKilled();

  private slots:
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);

  private:
    QProcess *compileProcess = nullptr;
};

} // namespace Core

#endif // COMPILER_HPP
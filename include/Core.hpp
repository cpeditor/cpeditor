/*
 * Copyright (C) 2019 Ashar Khan <ashar786khan@gmail.com>
 *
 * This file is part of CPEditor.
 *
 * CPEditor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * I will not be responsible if CPEditor behaves in unexpected way and
 * causes your ratings to go down and or loose any important contest.
 *
 * Believe Software is "Software" and it isn't not immune to bugs.
 *
 */

#ifndef CORE_CP_EDITOR_HPP
#define CORE_CP_EDITOR_HPP

#include <BaseFiles.hpp>
#include <MessageLogger.hpp>
#include <QCodeEditor>
#include <QFile>
#include <QProcess>
namespace Core
{

class Compiler : public QObject, private Core::Base::Files
{
    Q_OBJECT
  public:
    Compiler(QString command, int index, MessageLogger* log);
    void compile(QCodeEditor *editor, QString lang = "Cpp");
    void updateCommand(QString newCommand);
    static bool check(QString command);
    ~Compiler();

  signals:
    void compilationFinished(bool okay);
    void compilationError();
    void compilationStarted();

  private slots:
    void finished(int, QProcess::ExitStatus);
    void errorOccurred(QProcess::ProcessError);
    void started();

  private:
    QString runCommand;
    QProcess *compilationProcess = nullptr;
    QFile *file, *pyFile, *javaFile;
    MessageLogger *log;
};

enum Verdict
{
    ACCEPTED,
    WRONG_ANSWER,
    UNKNOWN,
};
} // namespace Core
#endif // CORE_CP_EDITOR_HPP

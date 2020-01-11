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

#include <Formatter.hpp>
#include <QProcess>

namespace Core
{
Formatter::Formatter(QString runCommand, int index, MessageLogger *log) : Base::Files(index)
{
    this->log = log;
    command = runCommand;
    file = new QFile(getTempFormatFile());
    file->open(QIODevice::ReadWrite | QFile::Text);
    if (!file->isOpen())
    {
        log->warn("Formatter", "Cannot create temporary format file");
    }
    else
    {
        file->resize(0);
    }
}

Formatter::~Formatter()
{
    if (file->isOpen())
        file->close();
    delete file;
}

bool Formatter::check(QString command)
{
    auto results = command.trimmed().split(" ");
    QProcess program;
    QString commandToStart = results[0];
    QStringList environment = program.systemEnvironment();
    program.start(commandToStart + " --version");
    bool started = program.waitForStarted();
    if (started) // 10 Second timeout
        program.kill();

    return started;
}

void Formatter::updateCommand(QString newCommand)
{
    command = newCommand;
}

void Formatter::format(QCodeEditor *editor)
{
    auto old_pos = editor->textCursor();
    auto old = editor->toPlainText().toStdString();
    if (file->isOpen())
    {
        file->resize(0);
        file->write(old.c_str());
        file->close();
        auto result = command.trimmed().split(" ");
        QProcess formatProcess;
        QString programName = result[0];
        result.removeAt(0);
        result.append(getTempFormatFile());

        formatProcess.start(programName, result);
        formatProcess.waitForFinished(2000); // BLOCKS main Thread

        if (formatProcess.state() == QProcess::Running)
        {
            log->warn("Formatter", "It seems the format command took more than 2 seconds to complete. Skipped");
            file->open(QIODevice::ReadWrite | QFile::Text);
            return;
        }

        if (formatProcess.exitCode() != 0)
        {
            log->error("Formatter",
                       "Format command returned non-zero exit code " + std::to_string(formatProcess.exitCode()));
            file->open(QIODevice::ReadWrite | QFile::Text);
            return;
        }

        file->open(QIODevice::ReadWrite | QFile::Text);
        auto doc = editor->document();
        QTextCursor cursor(doc);
        cursor.select(QTextCursor::Document);
        cursor.insertText(formatProcess.readAllStandardOutput());
        editor->setTextCursor(old_pos);
        log->info("Formatter", "Formatting completed");
    }
};
} // namespace Core

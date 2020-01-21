/*
 * Copyright (C) 2019-2020 Ashar Khan <ashar786khan@gmail.com>
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
 * Believe Software is "Software" and it isn't immune to bugs.
 *
 */

#include <Formatter.hpp>
#include <QJsonDocument>
#include <QProcess>
#include <QTemporaryDir>

namespace Core
{
Formatter::Formatter(QString clangFormatBinary, const QString &clangFormatStyle, MessageLogger *log)
{
    this->log = log;
    updateBinary(clangFormatBinary);
    updateStyle(clangFormatStyle);
}

bool Formatter::check(QString checkBinary, const QString &checkStyle)
{
    QTemporaryDir tmpDir;
    if (!tmpDir.isValid())
        return false;
    QFile tmpFile(tmpDir.filePath("tmp.cpp"));
    tmpFile.open(QIODevice::WriteOnly | QIODevice::Text);
    tmpFile.write("int main(){}");
    tmpFile.close();
    QFile styleFile(tmpDir.filePath(".clang-format"));
    styleFile.open(QIODevice::WriteOnly | QIODevice::Text);
    styleFile.write(checkStyle.toStdString().c_str());
    styleFile.close();
    QProcess program;
    program.start(checkBinary, {"--cursor=0", "--offset=0", "--length=0", "--style=file", tmpFile.fileName()});
    if (!program.waitForFinished(2000))
        program.kill();
    return program.exitCode() == 0;
}

void Formatter::updateBinary(QString newBinary)
{
    binary = newBinary;
}

void Formatter::updateStyle(const QString &newStyle)
{
    style = newStyle;
}

void Formatter::format(QCodeEditor *editor, const QString &filePath, const QString &lang, bool selectionOnly)
{
    auto cursor = editor->textCursor();
    int start = cursor.selectionStart();
    int end = cursor.selectionEnd();
    int pos = cursor.position();

    QStringList args = {"--cursor=" + QString::number(pos), "--style=file"};

    if (selectionOnly && cursor.hasSelection())
    {
        args.append("--offset=" + QString::number(start));
        args.append("--length=" + QString::number(end - start));
    }

    QString tmpName = "tmp.cpp";
    if (filePath.isEmpty())
    {
        if (lang == "Java")
            tmpName = "tmp.java";
        else if (lang == "Python")
            tmpName = "tmp.py";
    }
    else
    {
        tmpName = QFileInfo(filePath).fileName();
    }

    QTemporaryDir tmpDir;
    if (!tmpDir.isValid())
    {
        log->error("Formatter", "Failed to create temporary directory");
        return;
    }
    QFile tmpFile(tmpDir.filePath(tmpName));
    tmpFile.open(QIODevice::WriteOnly | QIODevice::Text);
    tmpFile.write(editor->toPlainText().toStdString().c_str());
    tmpFile.close();
    QFile styleFile(tmpDir.filePath(".clang-format"));
    styleFile.open(QIODevice::WriteOnly | QIODevice::Text);
    styleFile.write(style.toStdString().c_str());
    styleFile.close();

    args.append(tmpFile.fileName());

    auto res = getFormatResult(args);
    if (res.first == -1)
        return;

    cursor.select(QTextCursor::Document);
    cursor.insertText(res.second);
    cursor.setPosition(res.first);

    if (start != end)
    {
        if (pos == end)
            args[0] = "--cursor=" + QString::number(start);
        else
            args[0] = "--cursor=" + QString::number(end);
        auto res2 = getFormatResult(args);
        if (res2.first == -1)
            return;
        cursor.setPosition(res2.first, QTextCursor::MoveAnchor);
        cursor.setPosition(res.first, QTextCursor::KeepAnchor);
    }

    editor->setTextCursor(cursor);

    log->info("Formatter", "Formatting completed");
}

QPair<int, QString> Formatter::getFormatResult(const QStringList &args)
{
    QProcess formatProcess;
    formatProcess.start(binary, args);
    formatProcess.waitForFinished(2000); // BLOCKS main Thread

    if (formatProcess.state() == QProcess::Running)
    {
        formatProcess.kill();
        log->warn("Formatter", "The format command is: " + binary + " " + args.join(' '));
        log->warn("Formatter", "It seems the formatting took more than 2 seconds to complete. Skipped");
        return QPair<int, QString>(-1, QString());
    }

    if (formatProcess.exitCode() != 0)
    {
        log->warn("Formatter", "The format command is: " + binary + " " + args.join(' '));
        auto stdOut = formatProcess.readAllStandardOutput();
        if (!stdOut.isEmpty())
            log->warn("Formatter[stdout]", stdOut);
        auto stdError = formatProcess.readAllStandardError();
        if (!stdError.isEmpty())
            log->error("Formatter[stderr]", stdError);
        return QPair<int, QString>(-1, QString());
    }

    auto result = formatProcess.readAllStandardOutput();
    int firstNewLine = result.indexOf('\n');
    QString jsonLine = result.left(firstNewLine);
    auto formattedCodes = result.mid(firstNewLine + 1);
    auto json = QJsonDocument::fromJson(jsonLine.toLocal8Bit());
    int newCursorPos = json["Cursor"].toInt();

    return QPair<int, QString>(newCursorPos, formattedCodes);
}
} // namespace Core

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

#include "Core/Formatter.hpp"
#include "Core/EventLogger.hpp"
#include "Util.hpp"
#include <QJsonDocument>
#include <QProcess>
#include <QTemporaryDir>

namespace Core
{
Formatter::Formatter(const QString &clangFormatBinary, const QString &clangFormatStyle, MessageLogger *log)
{
    Core::Log::i("formatter/created") << "clangformatBinary : " << clangFormatBinary
                                      << "clangformatStyle : " << clangFormatStyle
                                      << "log is null? : " << (log == nullptr) << endl;
    this->log = log;
    updateBinary(clangFormatBinary);
    updateStyle(clangFormatStyle);
}

bool Formatter::check(const QString &checkBinary, const QString &checkStyle)
{
    Core::Log::i("formatter/check") << "checkBinary : " << checkBinary << "checkStyle : " << checkStyle << endl;
    QTemporaryDir tmpDir;
    if (!tmpDir.isValid())
    {
        Core::Log::w("formatter/check", "tmpDir is not valid");
        return false;
    }

    if (!Util::saveFile(tmpDir.filePath("tmp.cpp"), "int main(){}", "Formatter/check"))
        return false;

    if (!Util::saveFile(tmpDir.filePath(".clang-format"), checkStyle, "Formatter/check"))
        return false;

    QProcess program;
    program.start(checkBinary, {"--cursor=0", "--offset=0", "--length=0", "--style=file", tmpDir.filePath("tmp.cpp")});
    Core::Log::i("formatter/check", "started the format process");

    if (!program.waitForFinished(2000))
    {
        Core::Log::w("formatter/check", "Format process did not finished in 2 sec, it is being killed");
        program.kill();
        return false;
    }
    Core::Log::i("formatter/check") << "Process returned exitcode " << program.exitCode() << endl;
    return program.exitCode() == 0;
}

void Formatter::updateBinary(const QString &newBinary)
{
    Core::Log::i("formatter/updateBinary") << "New binary is : " << newBinary << endl;
    binary = newBinary;
}

void Formatter::updateStyle(const QString &newStyle)
{
    Core::Log::i("formatter/updateStyle") << "New style is : " << newStyle << endl;
    style = newStyle;
}

void Formatter::format(QCodeEditor *editor, const QString &filePath, const QString &lang, bool selectionOnly)
{
    Core::Log::i("formatter/format") << "Editor is null ? : " << (editor == nullptr) << " Filepath is : " << filePath
                                     << " Language is : " << lang << " selectionOnly ? : " << selectionOnly << endl;

    auto cursor = editor->textCursor();
    int start = cursor.selectionStart();
    int end = cursor.selectionEnd();
    int pos = cursor.position();

    Core::Log::i("formatter/format") << "Selection start : " << start << "Selection end : " << endl;
    Core::Log::i("formatter/format") << "Cursor position is : " << pos << endl;

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
        Core::Log::w("formatter/format", "filepath is empty");
    }
    else
    {
        Core::Log::i("formatter/format", "filepath is not empty");
        tmpName = QFileInfo(filePath).fileName();
    }

    QTemporaryDir tmpDir;
    if (!tmpDir.isValid())
    {
        log->error("Formatter", "Failed to create temporary directory");
        return;
    }

    if (!Util::saveFile(tmpDir.filePath(tmpName), editor->toPlainText(), "Formatter", true, log))
        return;

    Core::Log::i("formatter/format") << "Editor content is : \n" << editor->toPlainText() << endl;

    if (!Util::saveFile(tmpDir.filePath(".clang-format"), style, "Formatter", true, log))
        return;

    args.append(tmpDir.filePath(tmpName));

    auto res = getFormatResult(args);
    if (res.first == -1)
    {
        Core::Log::w("formatter/format", "getFormatResult.first is -1, bailed out");
        return;
    }

    cursor.select(QTextCursor::Document);
    cursor.insertText(res.second);
    cursor.setPosition(res.first);

    Core::Log::i("formatter/format") << "Restored the cursor and formatted text is \n" << res.second << endl;

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
        Core::Log::i("formatter/format", "Restoring the selection");
    }

    editor->setTextCursor(cursor);

    log->info("Formatter", "Formatting completed");
}

QPair<int, QString> Formatter::getFormatResult(const QStringList &args)
{
    QProcess formatProcess;
    formatProcess.start(binary, args);
    Core::Log::i("formatter/getFormatResult") << "Starting format with args : " << args.join(",") << endl;

    bool finished = formatProcess.waitForFinished(2000); // BLOCKS main Thread
    Core::Log::i("formatter/getFormatResult", "Finished wait of 2 sec for format process is completed");
    if (!finished)
    {
        Core::Log::w("formatter/getFormatResult", "formatProcess didn't finish in 2 seconds, kill it.");
        formatProcess.kill();
        log->warn("Formatter",
                  "The format process didn't finish in 2 seconds. This is probably because the clang-format binary is "
                  "not found by CP Editor. You can set the path to clang-format in Preferences->Formatting.");
        return QPair<int, QString>(-1, QString());
    }

    if (formatProcess.exitCode() != 0)
    {
        Core::Log::w("formatter/getFormatResult")
            << "formatProcess did returned exit code " << formatProcess.exitCode();
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

    Core::Log::i("formatter/getFormatResult") << "Completed returning the newCursor as : " << newCursorPos << endl;

    return QPair<int, QString>(newCursorPos, formattedCodes);
}
} // namespace Core

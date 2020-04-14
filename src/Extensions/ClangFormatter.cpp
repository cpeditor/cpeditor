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

#include "Extensions/ClangFormatter.hpp"
#include "Core/EventLogger.hpp"
#include "Util.hpp"
#include <QJsonDocument>
#include <QProcess>
#include <QTemporaryDir>

namespace Extensions
{
ClangFormatter::ClangFormatter(const QString &clangFormatBinary, const QString &clangFormatStyle, MessageLogger *logger)
{
    Core::Log::i("clangformatter/created")
        << "clangformatBinary : " << clangFormatBinary << "clangformatStyle : " << clangFormatStyle
        << "log is null? : " << (log == nullptr) << endl;
    log = logger;
    updateBinary(clangFormatBinary);
    updateStyle(clangFormatStyle);
}

bool ClangFormatter::check(const QString &checkBinary, const QString &checkStyle)
{
    Core::Log::i("clangformatter/check") << "checkBinary : " << checkBinary << "checkStyle : " << checkStyle << endl;

    // create a temporary directory for formatting
    QTemporaryDir tmpDir;
    if (!tmpDir.isValid())
    {
        Core::Log::w("clangformatter/check", "tmpDir is not valid");
        return false;
    }

    // save a simple file for testing
    if (!Util::saveFile(tmpDir.filePath("tmp.cpp"), "int main(){}", "Formatter/check"))
        return false;

    // save the style to .clang-format
    if (!Util::saveFile(tmpDir.filePath(".clang-format"), checkStyle, "Formatter/check"))
        return false;

    // run Clang Format
    QProcess program;
    program.start(checkBinary, {"--cursor=0", "--offset=0", "--length=0", "--style=file", tmpDir.filePath("tmp.cpp")});
    Core::Log::i("clangformatter/check", "started the format process");

    // check whether the process finished in 2 seconds with exit code 0
    if (!program.waitForFinished(2000))
    {
        Core::Log::w("clangformatter/check", "Format process did not finished in 2 sec, it is being killed");
        program.kill();
        return false;
    }
    Core::Log::i("clangformatter/check") << "Process returned exitcode " << program.exitCode() << endl;
    return program.exitCode() == 0;
}

void ClangFormatter::updateBinary(const QString &newBinary)
{
    Core::Log::i("clangformatter/updateBinary") << "New binary is : " << newBinary << endl;
    binary = newBinary;
}

void ClangFormatter::updateStyle(const QString &newStyle)
{
    Core::Log::i("clangformatter/updateStyle") << "New style is : " << newStyle << endl;
    style = newStyle;
}

void ClangFormatter::format(QCodeEditor *editor, const QString &filePath, const QString &lang, bool selectionOnly)
{
    Core::Log::i("clangformatter/format")
        << "Editor is null ? : " << (editor == nullptr) << " Filepath is : " << filePath << " Language is : " << lang
        << " selectionOnly ? : " << selectionOnly << endl;

    // get the cursor positions
    auto cursor = editor->textCursor();
    int start = cursor.selectionStart();
    int end = cursor.selectionEnd();
    int pos = cursor.position();
    Core::Log::i("clangformatter/format") << "Selection start : " << start << "Selection end : " << endl;
    Core::Log::i("clangformatter/format") << "Cursor position is : " << pos << endl;

    // get command line arguments related to cursor position
    QStringList args = {"--cursor=" + QString::number(pos), "--style=file"};
    if (selectionOnly && cursor.hasSelection())
    {
        args.append("--offset=" + QString::number(start));
        args.append("--length=" + QString::number(end - start));
    }

    // get the file name
    QString tmpName = "tmp.cpp";
    if (filePath.isEmpty())
    {
        // if the file path is empty, generate one depends on the language
        if (lang == "Java")
            tmpName = "tmp.java";
        else if (lang == "Python")
            tmpName = "tmp.py";
        Core::Log::w("clangformatter/format", "filepath is empty");
    }
    else
    {
        // otherwise, use the actual file name
        Core::Log::i("clangformatter/format", "filepath is not empty");
        tmpName = QFileInfo(filePath).fileName();
    }

    // create a temporary directory
    QTemporaryDir tmpDir;
    if (!tmpDir.isValid())
    {
        log->error("Formatter", "Failed to create temporary directory");
        return;
    }

    // save the text to a file
    if (!Util::saveFile(tmpDir.filePath(tmpName), editor->toPlainText(), "Formatter", true, log))
        return;
    Core::Log::i("clangformatter/format") << "Editor content is : \n" << editor->toPlainText() << endl;

    // save the style to .clang-format
    if (!Util::saveFile(tmpDir.filePath(".clang-format"), style, "Formatter", true, log))
        return;

    // add the file to be formatted in the command line arguments
    args.append(tmpDir.filePath(tmpName));

    // get format result with the current cursor
    auto res = getFormatResult(args);
    if (res.first == -1)
    {
        Core::Log::w("clangformatter/format", "getFormatResult.first is -1, bailed out");
        return;
    }

    // set the formatted text and cursor position
    cursor.select(QTextCursor::Document);
    cursor.insertText(res.second);
    cursor.setPosition(res.first);
    Core::Log::i("clangformatter/format") << "Restored the cursor and formatted text is \n" << res.second << endl;

    if (start != end)
    {
        // if there's a selection, we have to restore not only the cursor, but also the anchor

        // get the new position for the anchor
        if (pos == end)
            args[0] = "--cursor=" + QString::number(start);
        else
            args[0] = "--cursor=" + QString::number(end);
        auto res2 = getFormatResult(args);
        if (res2.first == -1)
            return;

        // restore the selection
        cursor.setPosition(res2.first, QTextCursor::MoveAnchor);
        cursor.setPosition(res.first, QTextCursor::KeepAnchor);
        Core::Log::i("clangformatter/format", "The selection is restored");
    }

    // apply the cursor changes to the editor
    editor->setTextCursor(cursor);

    log->info("Formatter", "Formatting completed");
}

QPair<int, QString> ClangFormatter::getFormatResult(const QStringList &args)
{
    // run the format porcess

    QProcess formatProcess;
    formatProcess.start(binary, args);
    Core::Log::i("clangformatter/getFormatResult") << "Starting format with args : " << args.join(",") << endl;

    bool finished = formatProcess.waitForFinished(2000); // BLOCKS main Thread
    Core::Log::i("clangformatter/getFormatResult", "Finished wait of 2 sec for format process is completed");
    if (!finished)
    {
        Core::Log::w("clangformatter/getFormatResult", "formatProcess didn't finish in 2 seconds, kill it.");
        formatProcess.kill();
        log->warn("Formatter",
                  "The format process didn't finish in 2 seconds. This is probably because the clang-format binary is "
                  "not found by CP Editor. You can set the path to clang-format in Preferences->Formatting.");
        return QPair<int, QString>(-1, QString());
    }

    if (formatProcess.exitCode() != 0)
    {
        // the format process failed, show the error messages and return {-1, QString()}
        Core::Log::w("clangformatter/getFormatResult")
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

    // get the formatted codes and the new cursor position
    auto result = formatProcess.readAllStandardOutput();
    int firstNewLine = result.indexOf('\n');
    QString jsonLine = result.left(firstNewLine);
    auto formattedCodes = result.mid(firstNewLine + 1);
    auto json = QJsonDocument::fromJson(jsonLine.toLocal8Bit());
    int newCursorPos = json["Cursor"].toInt();

    Core::Log::i("clangformatter/getFormatResult") << "Completed returning the newCursor as : " << newCursorPos << endl;

    return QPair<int, QString>(newCursorPos, formattedCodes);
}
} // namespace Extensions

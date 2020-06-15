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
#include "Core/MessageLogger.hpp"
#include "Util/FileUtil.hpp"
#include <QCodeEditor>
#include <QJsonDocument>
#include <QProcess>
#include <QTemporaryDir>

namespace Extensions
{
ClangFormatter::ClangFormatter(const QString &clangFormatBinary, const QString &clangFormatStyle, MessageLogger *logger,
                               QObject *parent)
    : QObject(parent)
{
    log = logger;
    updateBinary(clangFormatBinary);
    updateStyle(clangFormatStyle);
}

bool ClangFormatter::check(const QString &checkBinary, const QString &checkStyle)
{
    LOG_INFO(INFO_OF(checkBinary));

    // create a temporary directory for formatting
    QTemporaryDir tmpDir;
    if (!tmpDir.isValid())
    {
        LOG_WARN("tmpDir is not valid");
        return false;
    }

    // save a simple file for testing
    if (!Util::saveFile(tmpDir.filePath("tmp.cpp"), "int main(){}", tr("Formatter/check")))
        return false;

    // save the style to .clang-format
    if (!Util::saveFile(tmpDir.filePath(".clang-format"), checkStyle, tr("Formatter/check")))
        return false;

    // run Clang Format
    QProcess program;
    program.start(checkBinary, {"--cursor=0", "--offset=0", "--length=0", "--style=file", tmpDir.filePath("tmp.cpp")});
    LOG_INFO(INFO_OF(program.arguments().join(" ")));

    // check whether the process finished in 2 seconds with exit code 0
    if (!program.waitForFinished(2000))
    {
        LOG_WARN("Format process did not finished in 2 sec, it is being killed forcefully");
        program.kill();
        return false;
    }
    LOG_INFO(INFO_OF(program.exitCode()));
    return program.exitCode() == 0;
}

void ClangFormatter::updateBinary(const QString &newBinary)
{
    LOG_INFO("Updated clangformat binary to " << newBinary);
    binary = newBinary;
}

void ClangFormatter::updateStyle(const QString &newStyle)
{
    style = newStyle;
}

void ClangFormatter::format(QCodeEditor *editor, const QString &filePath, const QString &lang, bool selectionOnly)
{
    LOG_INFO(BOOL_INFO_OF(editor == nullptr) << INFO_OF(filePath) << BOOL_INFO_OF(selectionOnly));

    // get the cursor positions
    auto cursor = editor->textCursor();
    int start = cursor.selectionStart();
    int end = cursor.selectionEnd();
    int pos = cursor.position();

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
    }
    else
    {
        // otherwise, use the actual file name
        tmpName = QFileInfo(filePath).fileName();
    }

    // create a temporary directory
    QTemporaryDir tmpDir;
    if (!tmpDir.isValid())
    {
        log->error(tr("Formatter"), tr("Failed to create temporary directory"));
        return;
    }

    // save the text to a file
    if (!Util::saveFile(tmpDir.filePath(tmpName), editor->toPlainText(), tr("Formatter"), true, log))
        return;

    // save the style to .clang-format
    if (!Util::saveFile(tmpDir.filePath(".clang-format"), style, tr("Formatter"), true, log))
        return;

    // add the file to be formatted in the command line arguments
    args.append(tmpDir.filePath(tmpName));

    // get format result with the current cursor
    auto res = getFormatResult(args);
    if (res.first == -1)
    {
        return;
    }

    // set the formatted text and cursor position
    cursor.select(QTextCursor::Document);
    cursor.insertText(res.second);
    cursor.setPosition(res.first);

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
    }

    // apply the cursor changes to the editor
    editor->setTextCursor(cursor);

    log->info(tr("Formatter"), tr("Formatting completed"));
}

QPair<int, QString> ClangFormatter::getFormatResult(const QStringList &args)
{
    // run the format porcess

    QProcess formatProcess;
    formatProcess.start(binary, args);
    LOG_INFO("Starting format with args : " << args.join(","));

    bool finished = formatProcess.waitForFinished(2000); // BLOCKS main Thread
    if (!finished)
    {
        formatProcess.kill();
        log->warn(
            tr("Formatter"),
            tr("The format process didn't finish in 2 seconds. This is probably because the clang-format binary is not "
               "found by CP Editor. You can set the path to clang-format in Preferences->Extensions->Clang Format."));
        return QPair<int, QString>(-1, QString());
    }

    if (formatProcess.exitCode() != 0)
    {
        // the format process failed, show the error messages and return {-1, QString()}
        LOG_WARN("Format process returned exit code " << formatProcess.exitCode());

        log->warn(tr("Formatter"), tr("The format command is: %1 %2").arg(binary, args.join(' ')));
        auto stdOut = formatProcess.readAllStandardOutput();
        if (!stdOut.isEmpty())
            log->warn(tr("Formatter[stdout]"), stdOut);
        auto stdError = formatProcess.readAllStandardError();
        if (!stdError.isEmpty())
            log->error(tr("Formatter[stderr]"), stdError);
        return QPair<int, QString>(-1, QString());
    }

    // get the formatted codes and the new cursor position
    auto result = formatProcess.readAllStandardOutput();
    int firstNewLine = result.indexOf('\n');
    QString jsonLine = result.left(firstNewLine);
    auto formattedCodes = result.mid(firstNewLine + 1);
    auto json = QJsonDocument::fromJson(jsonLine.toLocal8Bit());
    int newCursorPos = json["Cursor"].toInt();

    return QPair<int, QString>(newCursorPos, formattedCodes);
}
} // namespace Extensions
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

#include "Extensions/YAPFormatter.hpp"
#include "Core/EventLogger.hpp"
#include "Core/MessageLogger.hpp"
#include "Util/FileUtil.hpp"
#include "generated/SettingsHelper.hpp"
#include <QCodeEditor>
#include <QProcess>
#include <QTemporaryDir>
#include <QTextBlock>
#include <QTextDocumentFragment>

namespace Extensions
{
YAPFormatter::YAPFormatter(const QString &bin, const QString &args, const QString &style, MessageLogger *logger,
                           QObject *parent)
    : QObject(parent)
{
    log = logger;
    updateBinary(bin);
    updateArguments(args);
    updateStyle(style);
}

void YAPFormatter::updateBinary(const QString &newBinary)
{
    LOG_INFO("Updated yapf format binary to " << newBinary);
    binary = newBinary;
}

void YAPFormatter::updateStyle(const QString &newStyle)
{
    style = newStyle;
}

void YAPFormatter::updateArguments(const QString &newArgs)
{
    args = newArgs;
}

void YAPFormatter::format(QCodeEditor *editor, const QString &filePath, bool selectionOnly, bool verbose)
{
    LOG_INFO(BOOL_INFO_OF(editor == nullptr) << INFO_OF(filePath) << BOOL_INFO_OF(selectionOnly));

    // get the cursor positions
    auto cursor = editor->textCursor();
    int start = cursor.selectionStart();
    int end = cursor.selectionEnd();

    cursor.setPosition(start);
    int firstLine = cursor.blockNumber();
    cursor.setPosition(end, QTextCursor::KeepAnchor);
    int lastLine = cursor.blockNumber();

    // get command line arguments related to cursor position
    auto argList = QProcess::splitCommand(args);

    if (selectionOnly && cursor.hasSelection())
    {
        argList.append("-l");
        argList.append(QString::number(firstLine + 1) + "-" + QString::number(lastLine + 1));
    }

    // get the file name
    QString tmpName = "tmp.py";

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
    if (!Util::saveFile(tmpDir.filePath(".style.yapf"), style, tr("Formatter"), true, log))
        return;

    // add the file to be formatted in the command line arguments
    argList.append(tmpDir.filePath(tmpName));

    // get format result with the current cursor
    auto res = getFormatResult(argList);
    if (res.isEmpty())
    {
        return;
    }

    if (res == editor->toPlainText())
    {
        LOG_INFO("Already formatted");
        if (verbose)
            log->info(tr("Formatter"), tr("Formatting completed"));
        return;
    }

    // set the formatted text and cursor position
    cursor.select(QTextCursor::Document);
    cursor.insertText(res);

    if (start != end)
    {
        // if there's a selection, we have to restore the selection.
        // Since YAPF does not provides --cursor argument, restoring cursor is just selecting the lines [start, end]

        cursor.movePosition(QTextCursor::MoveOperation::Start);
        cursor.movePosition(QTextCursor::MoveOperation::Down, QTextCursor::MoveMode::MoveAnchor, firstLine);
        cursor.movePosition(QTextCursor::MoveOperation::Down, QTextCursor::MoveMode::KeepAnchor,
                            lastLine - firstLine + 1);
    }

    // apply the cursor changes to the editor
    editor->setTextCursor(cursor);

    log->info(tr("Formatter"), tr("Formatting completed"));
}

QString YAPFormatter::getFormatResult(const QStringList &args)
{
    // run the format porcess

    QProcess formatProcess;
    formatProcess.start(binary, args);
    LOG_INFO("Starting format with args : " << args.join(","));

    bool finished = formatProcess.waitForFinished(2000); // BLOCKS main Thread
    if (!finished)
    {
        formatProcess.kill();
        log->warn(tr("Formatter"),
                  tr("The format process didn't finish in 2 seconds. This is probably because the python binary is not "
                     "found by CP Editor. You can set the path to python at %1.")
                      .arg(SettingsHelper::pathOfYAPFPath()));
        return QString();
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
        return QString();
    }

    // get the formatted codes
    return formatProcess.readAllStandardOutput();
}
} // namespace Extensions

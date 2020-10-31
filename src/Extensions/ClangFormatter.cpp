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
#include "generated/SettingsHelper.hpp"
#include <QCodeEditor>
#include <QJsonDocument>
#include <QProcess>
#include <QTemporaryDir>

namespace Extensions
{
ClangFormatter::ClangFormatter(QString bin, QString style, QString args, MessageLogger *log, QObject *parent)
    : CodeFormatter(bin, style, args, log, parent)
{
}

QStringList ClangFormatter::supportedLanguages()
{
    return {"C++", "Java"};
}

QString ClangFormatter::formatterName()
{
    return "Clang Formatter";
}

QStringList ClangFormatter::prepareFormatArguments(QCodeEditor *editor, const QString &filePath, QString language,
                                                   bool formatCompleteSource)
{
    wasSelection = false;

    LOG_INFO(BOOL_INFO_OF(editor == nullptr) << INFO_OF(filePath));

    // get the cursor positions
    auto cursor = editor->textCursor();
    unformatterStart = cursor.selectionStart();
    unformatterEnd = cursor.selectionEnd();
    unformatterPos = cursor.position();

    // get command line arguments related to cursor position
    QStringList args = {"--cursor=" + QString::number(unformatterPos), "--style=file"};
    if (cursor.hasSelection() && !formatCompleteSource)
    {
        wasSelection = true;
        args.append("--offset=" + QString::number(unformatterStart));
        args.append("--length=" + QString::number(unformatterEnd - unformatterStart));
    }

    // get the file name
    QString tmpName = language == "C++" ? "tmp.cpp" : "tmp.java";
    if (!filePath.isEmpty())
        tmpName = QFileInfo(filePath).fileName();

    // create a temporary directory
    if (tempDir)
        delete tempDir;
    tempDir = new QTemporaryDir();

    if (!tempDir->isValid())
    {
        logErrorMessage(tr("Failed to create temporary directory"));
        return QStringList();
    }

    // save the text to a file
    if (!Util::saveFile(tempDir->filePath(tmpName), editor->toPlainText(), formatterName(), true, logger()))
        return QStringList();

    // save the style to .clang-format
    if (!Util::saveFile(tempDir->filePath(".clang-format"), style(), tr("Formatter"), true, logger()))
        return QStringList();

    // add the file to be formatted in the command line arguments
    args.append(tempDir->filePath(tmpName));

    formatArgs = args; // save for future use
    return args;
}

void ClangFormatter::applyFormatting(QCodeEditor *editor, QString formatStdout)
{

    if (formatStdout.isEmpty())
        return;

    auto parseStdout = [](QString formatStdout) -> QPair<int, QString> {
        int firstNewLine = formatStdout.indexOf('\n');
        QString jsonLine = formatStdout.left(firstNewLine);
        auto formattedCodes = formatStdout.mid(firstNewLine + 1);
        auto json = QJsonDocument::fromJson(jsonLine.toLocal8Bit());
        int newCursorPos = json["Cursor"].toInt();

        return {newCursorPos, formattedCodes};
    };

    auto formatResult = parseStdout(formatStdout);

    // get format result with the current cursor
    if (formatResult.first == -1)
        return;

    if (formatResult.second == editor->toPlainText())
    {
        LOG_INFO("Already formatted");
        logMessage(tr("Formatting completed"));
        return;
    }

    // set the formatted text and cursor position
    QTextCursor cursor = editor->textCursor();
    cursor.select(QTextCursor::Document);
    cursor.insertText(formatResult.second);
    qDebug() << "Setting new cursor position at " << formatResult.first;
    cursor.setPosition(formatResult.first);

    if (wasSelection)
    {
        qDebug() << "There was a selection";
        // if there's a selection, we have to restore not only the cursor, but also the anchor

        // get the new position for the anchor
        if (unformatterPos == unformatterEnd)
            formatArgs[0] = "--cursor=" + QString::number(unformatterStart);
        else
            formatArgs[0] = "--cursor=" + QString::number(unformatterEnd);
        auto res2 = parseStdout(runFormatProcess(formatArgs).second);
        qDebug() << "Returned " << res2;
        if (res2.first == -1)
            return;

        // restore the selection
        cursor.setPosition(res2.first, QTextCursor::MoveAnchor);
        cursor.setPosition(formatResult.first, QTextCursor::KeepAnchor);
    }

    // apply the cursor changes to the editor
    editor->setTextCursor(cursor);
    logMessage("Formatting completed");
}
} // namespace Extensions

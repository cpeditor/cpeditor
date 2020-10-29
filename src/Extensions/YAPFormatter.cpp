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
#include <QJsonDocument>
#include <QProcess>
#include <QTemporaryDir>

namespace Extensions
{
YAPFormatter::YAPFormatter(QString bin, QString style, QString args, MessageLogger *log, QObject *parent)
    : CodeFormatter(bin, style, args, log, parent)
{
}

QStringList YAPFormatter::supportedLanguages()
{
    return {"Python"};
}

QString YAPFormatter::formatterName()
{
    return "YAPF Formatter";
}

QStringList YAPFormatter::prepareFormatArguments(QCodeEditor *editor, const QString &filePath, QString language)
{

    // get the file name
    QString tmpName = "tmp.py";
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
    if (!Util::saveFile(tempDir->filePath(".style.yapf"), style(), tr("Formatter"), true, logger()))
        return QStringList();

    QStringList arguments = {QProcess::splitCommand(binaryArgs())};

    // get the cursor positions
    auto cursor = editor->textCursor();
    int start = cursor.selectionStart();
    int end = cursor.selectionEnd();

    if (cursor.hasSelection())
    {
        cursor.setPosition(start);
        firstLine = cursor.blockNumber();
        cursor.setPosition(end, QTextCursor::KeepAnchor);
        lastLine = cursor.blockNumber();

        arguments.append("-l");
        arguments.append(QString::number(firstLine) + "-" + QString::number(lastLine - firstLine + 1));
    }

    arguments.append(tempDir->filePath(tmpName));
    return arguments;
}

void YAPFormatter::applyFormatting(QCodeEditor *editor, QString formatStdout)
{
    if (formatStdout.isEmpty())
        return;

    if (formatStdout == editor->toPlainText())
    {
        LOG_INFO("Already formatted");
        logMessage(tr("Formatting completed"));
        return;
    }
    QTextCursor cursor = editor->textCursor();
    cursor.select(QTextCursor::Document);
    cursor.insertText(formatStdout);

    if (cursor.hasSelection())
    {
        cursor.movePosition(QTextCursor::MoveOperation::Start);
        cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, firstLine - 1);
        cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::MoveMode::KeepAnchor);
        cursor.movePosition(QTextCursor::Down, QTextCursor::KeepAnchor, lastLine - 1);
    }

    editor->setTextCursor(cursor);
    delete tempDir;
    tempDir = nullptr;
    logMessage("Formatting completed");
}
} // namespace Extensions

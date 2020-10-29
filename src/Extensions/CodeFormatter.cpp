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

/*
 * The Formatter is used to format codes.
 * It runs synchronously, so the event loop is blocked while formatting.
 * The time limit for formatting is 2 seconds.
 */

#include "Extensions/CodeFormatter.hpp"
#include "Core/EventLogger.hpp"
#include "Core/MessageLogger.hpp"
#include <QCodeEditor>
#include <QProcess>

namespace Extensions
{
CodeFormatter::CodeFormatter(QString binary, QString style, QString binArgs, MessageLogger *logger, QObject *parent)

    : QObject(parent), m_binary(binary), m_style(style), m_binaryArgs(binArgs), m_logger(logger)
{
}

void CodeFormatter::format(QCodeEditor *editor, const QString &filePath, QString language)
{
    // Nothing in code editor, bail out.
    if (editor->toPlainText().isEmpty())
        return;

    // Sanity check if formatter supports this language
    if (!supportedLanguages().contains(language))
    {
        logErrorMessage(tr("Cannot format %1 code with this formatter").arg(language));
        return;
    }

    // get the format arguments, if empty is returned. It means some error occurred in preparing and bail out.
    // Error will be logged and reported by the function iteself.
    auto formatArgs = prepareFormatArguments(editor, filePath, language);
    if (formatArgs.isEmpty())
        return;

    // run the format process with the argument.
    // error will be reported by the function iteself.
    auto formatResults = runFormatProcess(formatArgs);

    // exit code 0 but empty output on stdout means that user added -i (inplace) formatting command.
    // warn him of this disaster.
    if (formatResults.first == 0 && formatResults.second.isEmpty())
    {
        logErrorMessage(tr("Got empty stdout from format process. Did you used -i or inplace formatting argument?"));
        return;
    }

    applyFormatting(editor, formatResults.second);
}
void CodeFormatter::setBinary(QString const &newBinary)
{
    m_binary = newBinary;
}
void CodeFormatter::setStyle(QString const &newStyle)
{
    m_style = newStyle;
}
void CodeFormatter::setArguments(QString const &newArgs)
{
    m_binaryArgs = newArgs;
}

QString CodeFormatter::binary() const
{
    return m_binary;
}
QString CodeFormatter::style() const
{
    return m_style;
}
QString CodeFormatter::binaryArgs() const
{
    return m_binaryArgs;
}
MessageLogger *CodeFormatter::logger() const
{
    return m_logger;
}
void CodeFormatter::logMessage(QString const &message)
{
    if (m_logger)
        m_logger->info(formatterName(), message);
}
void CodeFormatter::logErrorMessage(QString const &message)
{
    if (m_logger)
        m_logger->error(formatterName(), message);
}
void CodeFormatter::logWarningMessage(QString const &message)
{
    if (m_logger)
        m_logger->warn(formatterName(), message);
}

QPair<int, QString> CodeFormatter::runFormatProcess(QStringList const &args)
{
    QProcess formatProcess;
    formatProcess.start(binary(), args);
    LOG_INFO("Starting format with args : " << args.join(","));

    bool finished = formatProcess.waitForFinished(2000); // BLOCKS main Thread
    if (!finished)
    {
        formatProcess.kill();
        logWarningMessage(tr("The format process didn't finished in 2 seconds. This is probably because %1 binary is "
                             "not found by CP Editor. You can set its path in Extensions -> %1 -> Path.")
                              .arg(formatterName()));
        return {-1, QString()};
    }

    if (formatProcess.exitCode() != 0)
    {
        // the format process failed, show the error messages and return {-1, QString()}
        LOG_WARN("Format process returned exit code " << formatProcess.exitCode());

        logWarningMessage(tr("The format command is: %1 %2").arg(binary(), args.join(' ')));
        auto stdOut = formatProcess.readAllStandardOutput();
        if (!stdOut.isEmpty() && logger())
            logger()->warn(tr("Formatter[stdout]"), stdOut);
        auto stdError = formatProcess.readAllStandardError();
        if (!stdError.isEmpty() && logger())
            logger()->error(tr("Formatter[stderr]"), stdError);
        return {formatProcess.exitCode(), QString()};
    }
    return {formatProcess.exitCode(), formatProcess.readAllStandardOutput()};
}
} // namespace Extensions

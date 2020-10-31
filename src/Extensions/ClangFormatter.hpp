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

#ifndef CLANG_FORMATTER_HPP
#define CLANG_FORMATTER_HPP

#include "Extensions/CodeFormatter.hpp"

class MessageLogger;
class QCodeEditor;

namespace Extensions
{
class ClangFormatter : public CodeFormatter
{
    Q_OBJECT
  public:
    ClangFormatter(QString binary, QString style, QString binaryArgs, MessageLogger *logger, QObject *parent = nullptr);

    virtual QStringList supportedLanguages() override;
    virtual QString formatterName() override;
    virtual QStringList prepareFormatArguments(QCodeEditor *editor, const QString &filePath, QString language,
                                               bool formatCompleteSource) override;
    virtual void applyFormatting(QCodeEditor *editor, QString formatStdout) override;

  private:
    QStringList formatArgs;
    int unformatterStart;
    int unformatterEnd;
    int unformatterPos;
    
	bool wasSelection;
};
} // namespace Extensions
#endif

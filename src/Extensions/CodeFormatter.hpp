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

#ifndef CODE_FORMATTER_HPP
#define CODE_FORMATTER_HPP

#include <QObject>

class QString;
template <class A, class B> class QPair;
class QStringList;
class QCodeEditor;
class MessageLogger;
class QTemporaryFile;
class QTemporaryDir;

namespace Extensions
{
class CodeFormatter : public QObject
{
    Q_OBJECT
  public:
    CodeFormatter(QString binary, QString style, QString binaryArgs, MessageLogger *logger, QObject *parent = nullptr);

    void format(QCodeEditor *editor, const QString &filePath, QString language, bool formatCompleteSource = false);
    void setBinary(QString const &newBinary);
    void setStyle(QString const &newStyle);
    void setArguments(QString const &newArgs);

    virtual QStringList supportedLanguages() = 0;
    virtual QString formatterName() = 0;
    virtual QStringList prepareFormatArguments(QCodeEditor *editor, const QString &filePath, QString language, bool formatCompleteSource = false) = 0;
    virtual void applyFormatting(QCodeEditor *editor, QString formatStdout) = 0;

    void logMessage(QString const &message);
    void logErrorMessage(QString const &message);
    void logWarningMessage(QString const &message);

    QString binary() const;
    QString style() const;
    QString binaryArgs() const;
    MessageLogger *logger() const;

  protected:
    virtual QPair<int, QString> runFormatProcess(const QStringList &args);

    QTemporaryDir *tempDir = nullptr;

  private:
    QString m_binary;
    QString m_style;
    QString m_binaryArgs;

    MessageLogger *m_logger;
};
} // namespace Extensions
#endif

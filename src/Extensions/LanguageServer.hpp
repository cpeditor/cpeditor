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

#ifndef LANGUAGE_SERVER_H
#define LANGUAGE_SERVER_H

#include "Widgets/CodeEditor.hpp"
#include <QJsonObject>
#include <QProcess>

class MessageLogger;
class LSPClient;

namespace Extensions
{
class LanguageServer : public QObject
{
    Q_OBJECT

  public:
    explicit LanguageServer(QString language);
    ~LanguageServer();

    void openDocument(QString path, CodeEditor *editor, MessageLogger *logger);
    void closeDocument();
    void requestLinting();

    bool isDocumentOpen() const;

    void updateSettings();
    void updatePath(QString);

  private slots:
    void onLSPServerNotificationArrived(QString method, QJsonObject param);
    void onLSPServerResponseArrived(QJsonObject method, QJsonObject param);
    void onLSPServerRequestArrived(QString method, QJsonObject param, QJsonObject id);
    void onLSPServerErrorArrived(QJsonObject id, QJsonObject error);
    void onLSPServerProcessError(QProcess::ProcessError error);
    void onLSPServerProcessFinished(int exitCode, QProcess::ExitStatus status);
    void onLSPServerNewStderr(const QString &content);

  private:
    void performConnection();
    void createClient();
    bool shouldCreateClient();

    QCodeEditor::SeverityLevel lspSeverity(int a);
    void initializeLSP(QString url);

    CodeEditor *m_editor = nullptr;
    MessageLogger *logger = nullptr;
    LSPClient *lsp = nullptr;
    bool isInitialized = false;
    QString language;
    QString openFile;
};
} // namespace Extensions

#endif // !LANGUAGE_SERVER_H

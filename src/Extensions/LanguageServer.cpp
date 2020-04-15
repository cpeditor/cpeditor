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
#include "LanguageServer.hpp"
#include "Core/EventLogger.hpp"
#include <QJsonObject>
#include <QProcess>
#include <QString>

namespace Extensions
{

LanguageServer::LanguageServer(QString language)
{
    if (language == "Python")
        this->language = "python";
    else if (language == "Java")
        this->language = "java";
    else if (language == "C++")
        this->language = "cpp";
    else
    {
        Core::Log::w("LanguageServer/constructed") << "Invalid language " << language << "Fallback to c++" << endl;
        this->language = "cpp";
    }

    lsp = new LSPClient(SettingsHelper::getLSPPath(), {/* No arguments to pass for now*/});
    performConnection();
    lsp->initialize();
}

LanguageServer::~LanguageServer()
{
    if (lsp != nullptr)
    {
        lsp->shutdown();
        lsp->exit();
        delete lsp;
    }
    if (m_editor != nullptr)
        m_editor->clearSquiggle();
}

void LanguageServer::openDocument(QString path, QCodeEditor *editor)
{
    DocumentUri uri = "file://" + path.toStdString();
    m_editor = editor;
    lsp->didOpen(uri, m_editor->toPlainText().toStdString(), language.toStdString());
}
void LanguageServer::closeDocument(QString path)
{
}
void LanguageServer::changeDocument(QString path, QCodeEditor *editor)
{
}
void LanguageServer::updateSettings()
{
}

// Private methods

void LanguageServer::performConnection()
{
    if (lsp == nullptr)
    {
        Core::Log::e("LanguageServer/performConnection", "Cannot perform connect if lsp is nullptr");
        return;
    }
    connect(lsp, &LSPClient::onError, this, &LanguageServer::onLSPServerErrorArrived);
    connect(lsp, &LSPClient::onRequest, this, &LanguageServer::onLSPServerRequestArrived);
    connect(lsp, &LSPClient::onServerError, this, &LanguageServer::onLSPServerProcessError);
    connect(lsp, &LSPClient::onResponse, this, &LanguageServer::onLSPServerResponseArrived);
    connect(lsp, &LSPClient::onNotify, this, &LanguageServer::onLSPServerNotificationArrived);
    connect(lsp, &LSPClient::onServerFinished, this, &LanguageServer::onLSPServerProcessFinished);

    Core::Log::i("LanguageServer/performConnections", "All connections have been established");
}

// ---------------------------- LSP SLOTS ------------------------

void LanguageServer::onLSPServerNotificationArrived(QString method, QJsonObject param)
{
}

void LanguageServer::onLSPServerResponseArrived(QJsonObject method, QJsonObject param)
{
}

void LanguageServer::onLSPServerRequestArrived(QString method, QJsonObject param, QJsonObject id)
{
}

void LanguageServer::onLSPServerErrorArrived(QJsonObject id, QJsonObject error)
{
}

void LanguageServer::onLSPServerProcessError(QProcess::ProcessError error)
{
}

void LanguageServer::onLSPServerProcessFinished(int exitCode, QProcess::ExitStatus status)
{
}
} // namespace Extensions
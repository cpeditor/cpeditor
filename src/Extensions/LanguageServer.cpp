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
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QString>
#include <Util.hpp>

namespace Extensions
{

LanguageServer::LanguageServer(QString lang)
{
    LOG_INFO(INFO_OF(lang));
    this->language = lang;
    if (shouldCreateClient())
    {
        createClient();
        performConnection();
    }
}

LanguageServer::~LanguageServer()
{
    if (lsp != nullptr)
    {
        lsp->shutdown();
        lsp->exit();
        delete lsp;
    }
}

void LanguageServer::openDocument(QString path, QCodeEditor *editor, MessageLogger *log)
{
    m_editor = editor;
    openFile = path;
    logger = log;

    if (lsp == nullptr)
        return;

    if (!isInitialized)
    {
        initializeLSP(path);
        isInitialized = true;
    }

    std::string uri = "file://" + path.toStdString();
    std::string code = m_editor->toPlainText().toStdString();
    std::string lang = language.toStdString();

    lsp->didOpen(uri, code, lang);
}

void LanguageServer::closeDocument()
{
    openFile = "";
    logger = nullptr;
    m_editor = nullptr;

    if (lsp == nullptr)
        return;

    std::string uri = "file://" + openFile.toStdString();
    lsp->didClose(uri);
}

void LanguageServer::requestLinting()
{
    if (m_editor == nullptr || lsp == nullptr)
        return;

    std::vector<TextDocumentContentChangeEvent> changes;
    TextDocumentContentChangeEvent e;
    e.text = m_editor->toPlainText().toStdString();
    changes.push_back(e);

    std::string uri = "file://" + openFile.toStdString();
    lsp->didChange(uri, changes, true);
}

bool LanguageServer::isDocumentOpen() const
{
    return !openFile.isEmpty() && lsp != nullptr;
}

void LanguageServer::updateSettings()
{
    if (lsp != nullptr)
    {

        lsp->shutdown();
        lsp->exit();
        delete lsp;
        lsp = nullptr;
    }

    if (m_editor != nullptr)
        m_editor->clearSquiggle();

    if (shouldCreateClient())
    {
        createClient();

        performConnection();
        initializeLSP(openFile);

        if (m_editor != nullptr)
        {
            auto tmp_editor = m_editor;
            auto tmp_path = openFile;
            auto tmp_log = logger;
            if (isDocumentOpen())
                closeDocument();
            openDocument(tmp_path, tmp_editor, tmp_log);
        }
    }
}

void LanguageServer::updatePath(QString newPath)
{
    if (lsp == nullptr || (openFile == newPath))
        return;
    auto tmp_logger = logger;
    auto tmp_editor = m_editor;
    closeDocument();
    openDocument(newPath, tmp_editor, tmp_logger);
}

// Private methods
bool LanguageServer::shouldCreateClient()
{
    if (language == "python")
        return SettingsHelper::isLSPUseAutocompletePython() || SettingsHelper::isLSPUseLintingPython();
    else if (language == "java")
        return SettingsHelper::isLSPUseAutocompleteJava() || SettingsHelper::isLSPUseLintingJava();
    else
    {
        LOG_WARN_IF(language != "cpp", "Invalid language " << language << "falling back to C++");
        language = "cpp";
        return SettingsHelper::isLSPUseAutocompleteCpp() || SettingsHelper::isLSPUseLintingCpp();
    }
}

void LanguageServer::createClient()
{

    QString lspArgCpp = SettingsHelper::getLSPArgsCpp().trimmed();
    QString lspArgPython = SettingsHelper::getLSPArgsPython().trimmed();
    QString lspArgJava = SettingsHelper::getLSPArgsJava().trimmed();

    QStringList argListCpp, argListPython, argListJava;

    if (!lspArgCpp.isEmpty())
        argListCpp = Util::splitArgument(lspArgCpp);
    if (!lspArgPython.isEmpty())
        argListPython = Util::splitArgument(lspArgPython);
    if (!lspArgJava.isEmpty())
        argListJava = Util::splitArgument(lspArgJava);

    if (language == "python")
        lsp = new LSPClient(SettingsHelper::getLSPPathPython(), argListPython);
    else if (language == "java")
        lsp = new LSPClient(SettingsHelper::getLSPPathJava(), argListJava);
    else
        lsp = new LSPClient(SettingsHelper::getLSPPathCpp(), argListCpp);
}

void LanguageServer::performConnection()
{
    if (lsp == nullptr)
    {
        LOG_WARN("Skipping establishement of connections as lsp client is nullptr");
        return;
    }
    connect(lsp, &LSPClient::onError, this, &LanguageServer::onLSPServerErrorArrived);
    connect(lsp, &LSPClient::onRequest, this, &LanguageServer::onLSPServerRequestArrived);
    connect(lsp, &LSPClient::onServerError, this, &LanguageServer::onLSPServerProcessError);
    connect(lsp, &LSPClient::onResponse, this, &LanguageServer::onLSPServerResponseArrived);
    connect(lsp, &LSPClient::onNotify, this, &LanguageServer::onLSPServerNotificationArrived);
    connect(lsp, &LSPClient::onServerFinished, this, &LanguageServer::onLSPServerProcessFinished);
    connect(lsp, &LSPClient::newStderr, this, &LanguageServer::onLSPServerNewStderr);

    LOG_INFO("All language server connections have been established");
}

QCodeEditor::SeverityLevel LanguageServer::lspSeverity(int in)
{
    switch (in)
    {
    case 1:
        return QCodeEditor ::SeverityLevel::Error;
    case 2:
        return QCodeEditor::SeverityLevel::Warning;
    case 3:
        return QCodeEditor::SeverityLevel::Information;
    case 4:
        return QCodeEditor::SeverityLevel::Hint;
    }
    // Nothing matched
    return QCodeEditor::SeverityLevel::Error;
}

void LanguageServer::initializeLSP(QString filePath)
{
    QFileInfo info(filePath);
    std::string uri = "file://" + info.absoluteDir().absolutePath().toStdString();
    option<DocumentUri> rootUri(uri);
    lsp->initialize(rootUri);
}
// ---------------------------- LSP SLOTS ------------------------

void LanguageServer::onLSPServerNotificationArrived(QString method, QJsonObject param)
{
    if (method == "textDocument/publishDiagnostics" && m_editor != nullptr) // Linting
    {
        m_editor->clearSquiggle();
        QJsonArray doc = QJsonDocument::fromVariant(param.toVariantMap()).object()["diagnostics"].toArray();
        for (auto e : doc)
        {
            QString tooltip = e.toObject()["message"].toString();
            QCodeEditor::SeverityLevel level = lspSeverity(e.toObject()["severity"].toInt());

            auto beg = e.toObject()["range"].toObject()["start"].toObject();
            auto end = e.toObject()["range"].toObject()["end"].toObject();

            QPair<int, int> start, stop;

            start.first = beg["line"].toInt() + 1;
            start.second = beg["character"].toInt();

            stop.first = end["line"].toInt() + 1;
            stop.second = end["character"].toInt();

            m_editor->squiggle(level, start, stop,
                               tooltip.remove(" (fix available)")); // We do not provide quick fix so remove this text.
        }
    }
}

void LanguageServer::onLSPServerResponseArrived(QJsonObject method, QJsonObject param)
{
    LOG_INFO("Response from Server has arrived");
}

void LanguageServer::onLSPServerRequestArrived(QString method, QJsonObject param, QJsonObject id)
{
    LOG_INFO("Request from Sever has arrived. " << INFO_OF(method));
}

void LanguageServer::onLSPServerErrorArrived(QJsonObject id, QJsonObject error)
{
    QString ID, ERR;
    ID = QJsonDocument::fromVariant(id.toVariantMap()).toJson();
    ERR = QJsonDocument::fromVariant(error.toVariantMap()).toJson();

    LOG_ERR("ID is \n" << ID);
    LOG_ERR("ERR is \n" << ERR);

    if (logger != nullptr)
        logger->error("Langauge Server [" + language + "]",
                      "Language server sent an error. Please check log for details.");
}

void LanguageServer::onLSPServerProcessError(QProcess::ProcessError error)
{
    LOG_ERR("LSP Process errored out " << INFO_OF(error));
    if (logger == nullptr)
        return;
    switch (error)
    {
    case QProcess::FailedToStart:
        logger->error("Language Server  [" + language + "]",
                      "Failed to start LSP Process. Have you set the path to the Language Server program in "
                      "Preferences->Extensions->Language Server?");
        break;
    case QProcess::Crashed:
        logger->warn("Language Server [" + language + "]", "Process was abruptly killed");
        break;
    case QProcess::Timedout:
        logger->error("Language Server [" + language + "]", "LSP Process timed out");
        break;
    case QProcess::ReadError:
        logger->error("Language Server [" + language + "]", "LSP Process Read Error");
        break;
    case QProcess::WriteError:
        logger->error("Language Server [" + language + "]", "LSP Process Write Error");
        break;
    case QProcess::UnknownError:
        logger->error("Language Server [" + language + "]", "An unknown error has occured in LSP Process");
        break;
    }
}

void LanguageServer::onLSPServerProcessFinished(int exitCode, QProcess::ExitStatus status)
{
    LOG_INFO("LSP Finished with exit code " << exitCode << INFO_OF(language) << INFO_OF(status));
}

void LanguageServer::onLSPServerNewStderr(const QString &content)
{
    LOG_INFO(content);
}
} // namespace Extensions

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
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>
#include <QString>

namespace Extensions
{

LanguageServer::LanguageServer(QString lang)
{
    Core::Log::i("LanguageServer/constructed") << INFO_OF(lang) << endl;
    if (lang == "Python")
        language = "python";
    else if (lang == "Java")
        language = "java";
    else if (lang == "C++")
        language = "cpp";
    else
    {
        Core::Log::w("LanguageServer/constructed") << "Invalid language " << language << "Fallback to c++" << endl;
        this->language = "cpp";
    }
    // When we have written the QString -> QStringList argument converter this split(" ") should be removed

    createClient();
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
    std::string uri = "file://" + path.toStdString();
    m_editor = editor;
    openFile = path;
    std::string code = m_editor->toPlainText().toStdString();
    std::string lang = language.toStdString();

    lsp->didOpen(uri, code, lang);
}
void LanguageServer::closeDocument()
{
    std::string uri = "file://" + openFile.toStdString();
    lsp->didClose(uri);
    openFile = "";
    m_editor = nullptr;
}

void LanguageServer::requestLinting()
{
    std::vector<TextDocumentContentChangeEvent> changes;
    TextDocumentContentChangeEvent e;
    e.text = m_editor->toPlainText().toStdString();
    changes.push_back(e);

    std::string uri = "file://" + openFile.toStdString();
    lsp->didChange(uri, changes, true);
}

bool LanguageServer::isDocumentOpen() const
{
    return !openFile.isEmpty();
}

void LanguageServer::updateSettings()
{
    // When settings are changed. Do destruction then reconstruct the client.

    lsp->shutdown();
    lsp->exit();
    delete lsp;

    if (m_editor != nullptr)
        m_editor->clearSquiggle();

    createClient();

    performConnection();
    lsp->initialize();
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

void LanguageServer::createClient()
{
    QString lspArgCpp = SettingsHelper::getLSPArgsCpp().trimmed();
    QString lspArgPython = SettingsHelper::getLSPArgsPython().trimmed();
    QString lspArgJava = SettingsHelper::getLSPArgsJava().trimmed();

    QStringList argListCpp, argListPython, argListJava;

    if (!lspArgCpp.isEmpty())
        argListCpp = lspArgCpp.split(" ");
    if (!lspArgPython.isEmpty())
        argListPython = lspArgPython.split(" ");
    if (!lspArgJava.isEmpty())
        argListJava = lspArgJava.split(" ");

    if (language == "python")
        lsp = new LSPClient(SettingsHelper::getLSPPathPython(), argListPython);
    else if (language == "java")
        lsp = new LSPClient(SettingsHelper::getLSPPathJava(), argListJava);
    else
        lsp = new LSPClient(SettingsHelper::getLSPPathCpp(), argListCpp);
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
        return QCodeEditor::SeverityLevel::Information; // Hint will be added in future.
    }
    // Nothing matched
    return QCodeEditor::SeverityLevel::Error;
}

void LanguageServer::lintInEditor(QPair<int, int> start, QPair<int, int> stop, QCodeEditor::SeverityLevel level,
                                  QString tooltip)
{
    if (start.first == stop.first) // single line error
    {
        Core::Log::i("lintInEditor") << "Linting at " << start.first << " " << start.second << " " << stop.first << " "
                                     << stop.second << endl;

        m_editor->squiggle(level, start.first, start.second, stop.second, tooltip);
    }
    else // multiline error
    {
		// We select the error range text and for each new line we skip it.
		// This requires some other API that is yet to be implemented in QCodeEditor hence skipped.
        Core::Log::w("lintInEditor", "Multiline linting is not yet supported");
	}
}
// ---------------------------- LSP SLOTS ------------------------

void LanguageServer::onLSPServerNotificationArrived(QString method, QJsonObject param)
{
    if (method == "textDocument/publishDiagnostics") // Linting
    {
        m_editor->clearSquiggle();
        Core::Log::i("onLSPServerNotificationArrived","Linting started");
        QJsonArray doc = QJsonDocument::fromVariant(param.toVariantMap()).object()["diagnostics"].toArray();
        for (auto e : doc)
        {
            Core::Log::i("inside", "inside");
            QString tooltip = e.toObject()["message"].toString();
            QCodeEditor::SeverityLevel level = lspSeverity(e.toObject()["severity"].toInt());
            
			auto beg = e.toObject()["range"].toObject()["start"].toObject();
            auto end = e.toObject()["range"].toObject()["end"].toObject();

			QPair<int, int> start, stop;

            start.first = beg["line"].toInt();
            start.second = beg["character"].toInt();
			
			stop.first = end["line"].toInt();
            stop.second = end["character"].toInt();

            lintInEditor(start, stop, level, tooltip);
        }
    }
}

void LanguageServer::onLSPServerResponseArrived(QJsonObject method, QJsonObject param)
{
    Core::Log::i("Response arrived", "Response");
}

void LanguageServer::onLSPServerRequestArrived(QString method, QJsonObject param, QJsonObject id)
{
    Core::Log::i("Request arrived", "Something");
}

void LanguageServer::onLSPServerErrorArrived(QJsonObject id, QJsonObject error)
{
    Core::Log::i("Error arrived", "error");
}

void LanguageServer::onLSPServerProcessError(QProcess::ProcessError error)
{
    Core::Log::e("LanguageServer/onLSPServerProcessError", "LSP Process errored out");
    switch (error)
    {
    case QProcess::FailedToStart:
        Core::Log::e("LanguageServer/onLSPServerProcessError", "Failed to start");
        break;
    case QProcess::Crashed:
        Core::Log::e("LanguageServer/onLSPServerProcessError", "Process Crashed");
        break;
    case QProcess::Timedout:
        Core::Log::e("LanguageServer/onLSPServerProcessError", "Process Timedout");
        break;
    case QProcess::ReadError:
        Core::Log::e("LanguageServer/onLSPServerProcessError", "Process ReadError");
        break;
    case QProcess::WriteError:
        Core::Log::e("LanguageServer/onLSPServerProcessError", "Process WriteError");
        break;
    case QProcess::UnknownError:
        Core::Log::e("LanguageServer/onLSPServerProcessError", "Unknown Error");
        break;
    }
}

void LanguageServer::onLSPServerProcessFinished(int exitCode, QProcess::ExitStatus status)
{
    Core::Log::i("LanguageServer/onLSPServerProcessFinished")
        << "LSP Finished with exit code " << exitCode << " " << INFO_OF(language) << " " << INFO_OF(status) << endl;
}
} // namespace Extensions

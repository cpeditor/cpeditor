/*
 * Copyright (C) 2019-2020 Ashar Khan <ashar786khan@gmail.com>
 *
 * This file is part of CPEditor.
 *
 * CPEditor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * I will not be responsible if CPEditor behaves in unexpected way and
 * causes your ratings to go down and or lose any important contest.
 *
 * Believe Software is "Software" and it isn't immune to bugs.
 *
 */

#include "Extensions/CFTool.hpp"
#include "Core/EventLogger.hpp"
#include <QFileInfo>
#include <QUrl>

namespace Network
{

CFTool::CFTool(const QString &path, MessageLogger *logger) : CFToolPath(path)
{
    Core::Log::i("cftool/constructed") << "path is : " << path << " is logger null " << (logger == nullptr) << endl;
    log = logger;
    icon = new QSystemTrayIcon();
}

CFTool::~CFTool()
{
    Core::Log::i("cftool/destructed", "Invoking destructor");
    if (CFToolProcess != nullptr)
    {
        Core::Log::i("cftool/destructed", "deleting process pointer");
        delete CFToolProcess;
    }
    if (icon != nullptr)
    {
        Core::Log::i("cftool/destructed", "deleting icon pointer");
        delete icon;
    }
}

void CFTool::submit(const QString &filePath, const QString &url, const QString &lang)
{
    if (CFToolProcess != nullptr)
    {
        if (CFToolProcess->state() == QProcess::Running)
        {
            Core::Log::w("cftool/submit", "There is already a CF Tool running, kill it now");
            CFToolProcess->kill();
            delete CFToolProcess;
            log->error("CF Tool", "CF Tool was killed");
        }
        else
            delete CFToolProcess;
        CFToolProcess = nullptr;
    }

    Core::Log::i("cftool/submit") << INFO_OF(filePath) << ", " << INFO_OF(url) << ", " INFO_OF(lang) << endl;

    if (parseCfUrl(url, problemContestId, problemCode))
    {
        lastStatus = "Unknown";
        CFToolProcess = new QProcess();
        CFToolProcess->setProgram(CFToolPath);
        CFToolProcess->setArguments({"submit", problemContestId, problemCode, filePath});
        connect(CFToolProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(onReadReady()));
        connect(CFToolProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onFinished(int)));
        CFToolProcess->start();
        log->info("CF Tool", "CF Tool has started");
    }
    else
    {
        log->error("CF Tool", "Failed to parse the URL [" + url + "]");
    }
}

bool CFTool::check(const QString &path)
{
    Core::Log::i("cftool/check") << "checking for path " << path << endl;
    QProcess checkProcess;
    checkProcess.start(path, {"--version"});
    Core::Log::i("cftool/check", "process started to fetch version");
    bool finished = checkProcess.waitForFinished(2000);
    Core::Log::i("cftool/check") << "finished ? " << finished << " exitcode " << checkProcess.exitCode() << endl;
    return finished && checkProcess.exitCode() == 0;
}

void CFTool::updatePath(const QString &p)
{
    Core::Log::i("cftool/updatePath") << "new path is : " << p << endl;
    CFToolPath = p;
}

bool CFTool::parseCfUrl(const QString &url, QString &contestId, QString &problemCode)
{
    Core::Log::i("CF Tool/parseCfUrl") << INFO_OF(url) << endl;
    auto match = QRegularExpression(".*://codeforces.com/contest/([1-9][0-9]*)/problem/(0|[A-Z][1-9]?)").match(url);
    if (match.hasMatch())
    {
        contestId = match.captured(1);
        problemCode = match.captured(2);
        if (problemCode == "0")
            problemCode = "A";
        return true;
    }
    match = QRegularExpression(".*://codeforces.com/problemset/problem/([1-9][0-9]*)/([A-Z][1-9]?)").match(url);
    if (match.hasMatch())
    {
        contestId = match.captured(1);
        problemCode = match.captured(2);
        return true;
    }
    return false;
}

void CFTool::onReadReady()
{
    QString response = CFToolProcess->readAll();
    Core::Log::i("cftool/onReadReady") << "\n" << INFO_OF(response) << endl;
    response.remove(QRegularExpression("\x1b\\[.. "));
    if (!response.trimmed().isEmpty())
    {
        if (response.contains("status: "))
        {
            auto shortStatus = response.mid(response.indexOf("status: ") + 8);
            lastStatus = shortStatus.contains('\n') ? shortStatus.left(shortStatus.indexOf('\n')) : shortStatus;
            Core::Log::i("cftool/showResponse") << INFO_OF(shortStatus) << endl;
            if (response.contains("status: Happy New Year") || response.contains("status: Accepted") ||
                response.contains("status: Pretests Passed"))
                log->message("CF Tool", shortStatus, "green");
            else if (response.contains("status: Running"))
                log->error("CF Tool", shortStatus);
            else
                log->info("CF Tool", shortStatus);
        }
    }
    else if (!response.trimmed().isEmpty())
    {
        Core::Log::i("CF Tool/showResponse") << "no status, " << INFO_OF(response) << endl;
        log->info("CF Tool", response);
    }
    else
    {
        Core::Log::i("CF Tool/showResponse", "Response is empty");
    }
}

void CFTool::onFinished(int exitCode)
{
    if (exitCode == 0)
    {
        showToastMessage(lastStatus);
    }
    else
    {
        showToastMessage("CF Tool failed");
        log->error("CF Tool", "CF Tool finished with non-zero exit code " + QString::number(exitCode));
        QString err = CFToolProcess->readAllStandardError();
        if (!err.trimmed().isEmpty())
            log->error("CF Tool", err);
    }
}

void CFTool::showToastMessage(const QString &message)
{
    icon->show();
    icon->showMessage("Contest " + problemContestId + " Problem " + problemCode, message);
    icon->hide();
}

} // namespace Network

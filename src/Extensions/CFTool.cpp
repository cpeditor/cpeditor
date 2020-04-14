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

#include "Extensions/CFTool.hpp"
#include "Core/EventLogger.hpp"
#include <QFileInfo>
#include <QUrl>

namespace Extensions
{

CFTool::CFTool(const QString &path, MessageLogger *logger) : CFToolPath(path)
{
    Core::Log::i("cftool/constructed") << "path is : " << path << " is logger null " << (logger == nullptr) << endl;
    log = logger;
}

CFTool::~CFTool()
{
    Core::Log::i("cftool/destructed", "Invoking destructor");
    if (CFToolProcess != nullptr)
    {
        Core::Log::i("cftool/destructed", "deleting process pointer");
        delete CFToolProcess;
    }
}

void CFTool::submit(const QString &filePath, const QString &url)
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

    Core::Log::i("cftool/submit") << INFO_OF(filePath) << ", " << INFO_OF(url) << endl;

    if (parseCfUrl(url, problemContestId, problemCode))
    {
        if (problemCode == "0")
        {
            problemCode = "A";
            log->warn("CF Tool", "The problem code is 0, now use A automatically. If the actual problem code is not A, "
                                 "please set the problem code manually in the right-click menu of the current tab.");
        }
        lastStatus = "Unknown";
        CFToolProcess = new QProcess();
        CFToolProcess->setProgram(CFToolPath);
        auto version = getCFToolVersion();
        if (version.isEmpty())
        {
            log->error(
                "CF Tool",
                "Failed to get the version of CF Tool. Have you set the correct path to CF Tool in Preferences?");
            return;
        }
        if (version.split('.')[0] == "0")
            CFToolProcess->setArguments({"submit", problemContestId, problemCode, filePath});
        else
            CFToolProcess->setArguments({"submit", "-f", filePath, url});
        Core::Log::i("CFTool/submit") << INFO_OF(CFToolProcess->arguments().join(' ')) << endl;
        connect(CFToolProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(onReadReady()));
        connect(CFToolProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onFinished(int)));
        CFToolProcess->start();
        bool started = CFToolProcess->waitForStarted(2000);
        if (started)
        {
            log->info("CF Tool", "CF Tool has started");
        }
        else
        {
            CFToolProcess->kill();
            log->error(
                "CF Tool",
                "Failed to start CF Tool in 2 seconds. Have you set the correct path to CF Tool in Preferences?");
        }
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
    if (response.contains("status: "))
    {
        auto shortStatus = response.mid(response.indexOf("status: ") + 8);
        lastStatus = shortStatus.contains('\n') ? shortStatus.left(shortStatus.indexOf('\n')) : shortStatus;
        Core::Log::i("cftool/showResponse") << INFO_OF(shortStatus) << endl;
        if (response.contains("status: Happy New Year") || response.contains("status: Accepted") ||
            response.contains("status: Pretests passed"))
            log->message("CF Tool", shortStatus, "green");
        else if (response.contains("status: Running"))
            log->info("CF Tool", shortStatus);
        else
            log->error("CF Tool", shortStatus);
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
    }
    QString err = CFToolProcess->readAllStandardError();
    if (!err.trimmed().isEmpty())
        log->error("CF Tool", err);
}

void CFTool::showToastMessage(const QString &message)
{
    emit requestToastMessage("Contest " + problemContestId + " Problem " + problemCode, message);
}

QString CFTool::getCFToolVersion() const
{
    Core::Log::i("CFTool/getCFToolVersion", "Invoked");
    QProcess process;
    process.start(CFToolPath, {"--version"});
    if (!process.waitForFinished(2000))
    {
        Core::Log::w("CFTool/getCFToolVersion", "CF Tool didn't finish");
        return "";
    }
    QString version = QRegularExpression(R"((?<=v)\d+\.\d+\.\d+)").match(process.readAll()).captured();
    Core::Log::i("CFTool/getCFToolVersion") << INFO_OF(version) << endl;
    return version;
}

} // namespace Extensions

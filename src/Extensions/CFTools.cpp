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
 * causes your ratings to go down and or loose any important contest.
 *
 * Believe Software is "Software" and it isn't immune to bugs.
 *
 */

#include "Extensions/CFTools.hpp"
#include "Core/EventLogger.hpp"
#include <QFileInfo>
#include <QUrl>

namespace Network
{

CFTools::CFTools(const QString &path, MessageLogger *logger) : path(path)
{
    Core::Log::i("cftools/constructed") << "path is : " << path << " is logger null " << (logger == nullptr) << endl;
    log = logger;
    icon = new QSystemTrayIcon();
    CFToolProcess = new QProcess();
}

CFTools::~CFTools()
{
    Core::Log::i("cftools/destructed", "Invoking destructor");
    if (icon != nullptr)
    {
        Core::Log::i("cftools/destructed", "deleting icon pointer");
        delete icon;
    }
    if (CFToolProcess != nullptr)
    {
        Core::Log::i("cftools/destructed", "deleting process pointer");
        delete CFToolProcess;
    }
}

void CFTools::submit(const QString &filePath, const QString &url, const QString &lang)
{
    if (CFToolProcess->state() == QProcess::Running)
    {
        Core::Log::w("cftools/submit", "Already cftool was running, now killing it");
        CFToolProcess->kill();
    }

    Core::Log::i("cftools/submit") << "filepath " << filePath << " url " << url << "lang " << lang << endl;
    QString problemContestId, problemCode;
    QRegularExpression regexContest(".*://codeforces.com/contest/([1-9][0-9]*)/problem/(0|[A-Z][1-9]?)");
    auto matchContest = regexContest.match(url);
    if (matchContest.hasMatch())
    {
        problemContestId = matchContest.captured(1);
        problemCode = matchContest.captured(2);
        if (problemCode == "0")
            problemCode = "A";
        Core::Log::i("cftools/submit") << "contest regex matched for id " << problemContestId << " code " << problemCode
                                       << endl;
    }
    else
    {
        Core::Log::i("cftools/submit", "regex match fallbackt to practice");
        QRegularExpression regexProblemSet(".*://codeforces.com/problemset/problem/([1-9][0-9]*)/([A-Z][1-9]?)");
        auto matchProblemSet = regexProblemSet.match(url);
        if (matchProblemSet.hasMatch())
        {
            problemContestId = matchProblemSet.captured(1);
            problemCode = matchProblemSet.captured(2);
            Core::Log::i("cftools/submit")
                << "practice regex matched for id " << problemContestId << " code " << problemCode << endl;
        }
        else
        {
            log->error("CF Tool", "Failed to parse the problem URL " + url);
            return;
        }
    }
    CFToolProcess->setProgram(path);
    CFToolProcess->setArguments({"submit", problemContestId, problemCode, filePath});
    connect(CFToolProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(onReadReady()));
    CFToolProcess->start();
    Core::Log::i("cftools/submit", "cftools has started");
}

bool CFTools::check(const QString &path)
{
    Core::Log::i("cftools/check") << "checking for path " << path << endl;
    QProcess checkProcess;

    checkProcess.start(path, {"--version"});
    Core::Log::i("cftools/check", "process started to fetch version");
    bool finished = checkProcess.waitForFinished(2000);
    Core::Log::i("cftools/check") << "finished ? " << finished << " exitcode " << checkProcess.exitCode() << endl;
    return finished && checkProcess.exitCode() == 0;
}

void CFTools::updatePath(const QString &p)
{
    Core::Log::i("cftools/updatePath") << "new path is : " << p << endl;
    path = p;
}

void CFTools::onReadReady()
{
    QString newResponse = CFToolProcess->readAll();
    Core::Log::i("cftools/onReadReady") << "\n" << newResponse << endl;
    auto shortStatus = newResponse.right(newResponse.size() - newResponse.indexOf("status:") - 8);
    if (newResponse.contains("status: Happy New Year") || newResponse.contains("status: Accepted"))
    {
        Core::Log::i("cftools/onReadReady") << "solution was accepted " << shortStatus << endl;
        log->message("CF Tool", shortStatus, "green");
        icon->show();
        icon->showMessage("CF Tool", shortStatus);
        icon->hide();
    }
    else if (newResponse.contains("status: Running on"))
    {
        Core::Log::i("cftools/onReadReady") << " solution is running " << shortStatus << endl;
        log->info("CFTool", shortStatus);
    }
    else
    {
        Core::Log::i("cftools/onReadReady") << " solution was rejected " << shortStatus << endl;
        log->error("CF Tool", newResponse);
        icon->show();
        icon->showMessage("CF Tool", newResponse, QSystemTrayIcon::Warning);
        icon->hide();
    }
}

} // namespace Network

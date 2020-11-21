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
#include "Core/MessageLogger.hpp"
#include "generated/SettingsHelper.hpp"
#include <QFileInfo>
#include <QProcess>
#include <QRegularExpression>
#include <QUrl>

namespace Extensions
{

CFTool::CFTool(const QString &path, MessageLogger *logger) : CFToolPath(path)
{
    LOG_INFO(INFO_OF(path))
    log = logger;
}

CFTool::~CFTool()
{
    if (CFToolProcess != nullptr)
        delete CFToolProcess;
}

void CFTool::submit(const QString &filePath, const QString &url)
{
    if (CFToolProcess != nullptr)
    {
        if (CFToolProcess->state() == QProcess::Running)
        {
            LOG_WARN("CF Tool was already running, forcefully killing it now");
            CFToolProcess->kill();
            delete CFToolProcess;
            log->error(tr("CF Tool"), tr("CF Tool was killed"));
        }
        else
            delete CFToolProcess;
        CFToolProcess = nullptr;
    }

    LOG_INFO(INFO_OF(filePath) << INFO_OF(url));

    auto version = getCFToolVersion();
    if (version.isEmpty())
    {

        log->error(
            tr("CF Tool"),
            tr("Failed to get the version of CF Tool. Have you set the correct path to CF Tool in Preferences?"));
        return;
    }

    lastStatus = "Unknown"; // No tr here. We don't know what we'll get from network. Maybe a array for mapping.
    CFToolProcess = new QProcess();
    CFToolProcess->setProgram(CFToolPath);

    bool wasProblemCodeChanged = false;
    QString urlCopy = url;

    if (version.split('.')[0] == "0")
    {
        log->warn(tr("CF Tool"),
                  tr("You are using CF Tool %1. Please update to CF tools 1.0 or above.").append(version));

        if (parseCfUrl(url, problemContestId, problemCode))
        {

            if (problemCode == "0")
            {
                problemCode = "A";
                wasProblemCodeChanged = true;
            }

            CFToolProcess->setArguments({"submit", problemContestId, problemCode, filePath});
        }
        else
        {
            log->error(tr("CF Tool"),
                       tr("Failed to parse URL %1, updating CF tool to 1.0 or above might fix.").arg(url));
            return;
        }
    }
    else
    {
        if (url.endsWith("0"))
        {
            wasProblemCodeChanged = true;
            urlCopy = url.mid(0, url.size() - 1) + "A";
        }
        CFToolProcess->setArguments({"submit", "-f", filePath, urlCopy});
    }

    if (wasProblemCodeChanged)
    {
        log->warn(tr("CF Tool"),
                  tr("The problem code was 0, It has been changed to %1. If the actual problem code is not this, "
                     "please set the problem code manually in the right-click menu of the current tab.").arg(urlCopy));
    }

    LOG_INFO(INFO_OF(CFToolProcess->arguments().join(' ')));

    connect(CFToolProcess, &QProcess::readyReadStandardOutput, this, &CFTool::onReadReady);
    connect(CFToolProcess, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, &CFTool::onFinished);
    CFToolProcess->start();
    bool started = CFToolProcess->waitForStarted(2000);
    if (started)
    {
        log->info(tr("CF Tool"), tr("CF Tool has started"));
    }
    else
    {
        CFToolProcess->kill();
        log->error(
            tr("CF Tool"),
            tr("Failed to start CF Tool in 2 seconds. Have you set the correct path to CF Tool in Preferences?"));
    }
}

bool CFTool::check(const QString &path)
{
    LOG_INFO(INFO_OF(path));
    QProcess checkProcess;
    checkProcess.start(path, {"--version"});
    bool finished = checkProcess.waitForFinished(2000);
    LOG_INFO(BOOL_INFO_OF(finished) << INFO_OF(checkProcess.exitCode()) << INFO_OF(checkProcess.exitStatus()));
    return finished && checkProcess.exitCode() == 0;
}

void CFTool::updatePath(const QString &p)
{
    LOG_INFO(INFO_OF(p));
    CFToolPath = p;
}

bool CFTool::parseCfUrl(const QString &url, QString &contestId, QString &problemCode)
{
    LOG_INFO(INFO_OF(url));
    auto match =
        QRegularExpression(".*://codeforces.com/(?:gym|contest)/([1-9][0-9]*)/problem/(0|[A-Z][1-9]?)").match(url);
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
    response.remove(QRegularExpression("\x1b\\[.. "));
    if (response.contains("status: "))
    {
        auto shortStatus = response.mid(response.indexOf("status: ") + 8); // Maybe need some tricks to translate
        lastStatus = shortStatus.contains('\n') ? shortStatus.left(shortStatus.indexOf('\n')) : shortStatus;

        if (response.contains("status: Happy New Year") || response.contains("status: Accepted") ||
            response.contains("status: Pretests passed"))
            log->message(tr("CF Tool"), shortStatus, "green");
        else if (response.contains("status: Running"))
            log->info(tr("CF Tool"), shortStatus);
        else
            log->error(tr("CF Tool"), shortStatus);
    }
    else if (!response.trimmed().isEmpty())
        log->info(tr("CF Tool"), response);
    else
        LOG_INFO("Response is empty");
}

void CFTool::onFinished(int exitCode, QProcess::ExitStatus e)
{
    if (exitCode == 0)
    {
        showToastMessage(lastStatus);
    }
    else
    {
        showToastMessage(tr("CF Tool failed"));
        log->error(tr("CF Tool"), tr("CF Tool finished with non-zero exit code %1").arg(exitCode));
    }
    QString err = CFToolProcess->readAllStandardError();
    if (!err.trimmed().isEmpty())
        log->error(tr("CF Tool"), err);
}

void CFTool::showToastMessage(const QString &message)
{
    if (SettingsHelper::isCFShowToastMessages())
        emit requestToastMessage(tr("Contest %1 Problem %2").arg(problemContestId).arg(problemCode), message);
}

QString CFTool::getCFToolVersion() const
{
    QProcess process;
    process.start(CFToolPath, {"--version"});
    if (!process.waitForFinished(2000))
    {
        LOG_WARN("CF Tool didn't finish after 2 second");
        return "";
    }
    QString version = QRegularExpression(R"((?<=v)\d+\.\d+\.\d+)").match(process.readAll()).captured();
    LOG_INFO(INFO_OF(version));
    return version;
}

} // namespace Extensions

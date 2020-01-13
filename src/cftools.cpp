/*
* Copyright (C) 2019 Ashar Khan <ashar786khan@gmail.com> 
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


#include "cftools.hpp"
#include <QDebug>
#include <QUrl>
namespace Network
{

CFTools::CFTools(int index, MessageLogger *logger) : Core::Base::Files(index)
{
    log = logger;
}

CFTools::~CFTools()
{
    if (cftool != nullptr)
    {
        delete cftool;
    }
}

void CFTools::killProcess()
{
}

void CFTools::submit(QString url, QString language)
{
    if (cftool != nullptr)
    {
        delete cftool;
        cftool = nullptr;
    }

    QString filePath;
    if (language == "Cpp")
    {
        filePath = getProgramFile();
    }
    else if (language == "Java")
    {
        filePath = getProgramFile(".java");
    }
    else if (language == "Python")
    {
        filePath = getProgramFile(".py");
    }
    else
    {
        return;
    }

    auto problemCode = url.right(url.size() - 1 - url.lastIndexOf('/'));
    auto splitRes = url.split('/');
    QString problemContestId;
    for (auto e : splitRes)
    {
        if (!e.isEmpty() && e.toInt())
        {
            problemContestId = e;
            break;
        }
    }

    cftool = new QProcess();
    cftool->setProgram("cf");
    cftool->setArguments({"submit", problemContestId, problemCode, filePath});
    connect(cftool, SIGNAL(readyReadStandardOutput()), this, SLOT(onReadReady()));
    cftool->start();

    // Todo : Invoke cf-tools here.
}

bool CFTools::check()
{
    QProcess program;
    QString commandToStart = "cf";
    QStringList environment = program.systemEnvironment();
    program.start(commandToStart);
    bool started = program.waitForStarted();
    if (started) // 10 Second timeout
        program.kill();
    return started;
}

void CFTools::onReadReady()
{
    QString newResponse = cftool->readAll();
    auto shortStatus = newResponse.right(newResponse.size() - newResponse.indexOf("status:") - 8).toStdString();
    if (newResponse.contains("status: Happy New Year") || newResponse.contains("status: Accepted"))
    {
        // Warnings are green so its a hack to look green
        log->warn("CFTool", shortStatus);
    }
    else if (newResponse.contains("status: Running on"))
    {
        log->info("CFTool", shortStatus);
    }
    else
    {
        log->error("CFTool", newResponse.toStdString(), newResponse.contains("status:"));
    }
}

} // namespace Network

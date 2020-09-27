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
#include "Core/FakeVimCommand.hpp"
#include "Util/FileUtil.hpp"
#include "appwindow.hpp"
#include "fakevimhandler.h"
#include <QApplication>
#include <QDebug>
#include <QFileInfo>
#include <QPair>
#include <QStatusBar>
#include <generated/SettingsHelper.hpp>

namespace Core
{
FakeVimCommand::FakeVimCommand(AppWindow *aw) : appwin(aw)
{
}

bool FakeVimCommand::handle(FakeVim::Internal::ExCommand const &cmd)
{
    if (wantNewFile(cmd).first)
    {
        appwin->openTab("", wantNewFile(cmd).second);
        return true;
    }
    if (wantOpenFile(cmd).first)
    {
        auto path = wantOpenFile(cmd).second;
        if (!path.isEmpty())
        {
            appwin->openTab(path);
        }
        return true;
    }
    if (wantCompile(cmd))
    {
        appwin->on_actionCompile_triggered();
        return true;
    }
    if (wantCompileRun(cmd) != -1)
    {
    }
    if (wantDetachedRun(cmd) != -1)
    {
    }
    if (wantRun(cmd) != -1)
    {
    }
    if (wantKillAll(cmd))
    {
        appwin->on_actionKillProcesses_triggered();
        return true;
    }
    if (wantSnippets(cmd))
    {
        appwin->on_actionUseSnippets_triggered();
        return true;
    }
    if (wantFormat(cmd))
    {
        appwin->on_actionFormatCode_triggered();
        return true;
    }
    if (!wantVModeChange(cmd).isEmpty())
    {
    }
    if (wantPreferences(cmd))
    {
        appwin->on_actionSettings_triggered();
        return true;
    }
    if (QStringList({"cpp", "java", "python", "py"}).contains(wantLanguageChange(cmd)))
    {
    }
    if (wantMessageLoggerClear(cmd) && appwin->currentWindow())
    {
        appwin->currentWindow()->on_clearMessagesButton_clicked();
        return true;
    }
    if (wantExit(cmd))
    {
        appwin->on_actionQuit_triggered();
        return true;
    }
    return false;
}

QPair<bool, QString> FakeVimCommand::wantNewFile(FakeVim::Internal::ExCommand const &ex)
{
    if (ex.cmd == "new")
    {
        if (ex.args == "c++" || ex.args == "cpp")
            return {true, "C++"};
        else if (ex.args == "py" || ex.args == "python")
            return {true, "Python"};
        else if (ex.args == "java")
            return {true, "Java"};
        else if (ex.args.isEmpty())
            return {true, SettingsHelper::getDefaultLanguage()};
        else
        {
            if (appwin->currentWindow())
                appwin->currentWindow()->statusBar()->showMessage(
                    QString(tr("Error: %1 requires an empty or one of \"cpp\", \"java\" and \"python\" argument"))
                        .arg(ex.cmd),
                    STATUS_RESPONSE_TIMEOUT);
        }
    }

    return {false, ""};
}

QPair<bool, QString> FakeVimCommand::wantOpenFile(FakeVim::Internal::ExCommand const &ex)
{
    if (ex.cmd == "open" || ex.cmd == "opn")
    {
        if (ex.args.isEmpty())
        {
            if (appwin->currentWindow())
                appwin->currentWindow()->statusBar()->showMessage(
                    QString(QApplication::tr("Error: %1 requires a file path")).arg(ex.cmd), STATUS_RESPONSE_TIMEOUT);
            return {true, ""};
        }

        QFileInfo file(ex.args);
        if (!file.exists())
        {
            if (appwin->currentWindow())
                appwin->currentWindow()->statusBar()->showMessage(
                    QString(tr("Error in Opening: %1 does not exists")).arg(ex.args), STATUS_RESPONSE_TIMEOUT);
            return {true, ""};
        }
        else if (!file.isFile())
        {

            if (appwin->currentWindow())
                appwin->currentWindow()->statusBar()->showMessage(
                    QString(tr("Error in Opening: %1 is not a file")).arg(ex.args), STATUS_RESPONSE_TIMEOUT);
            return {true, ""};
        }

        else if (!Util::cppSuffix.contains(file.suffix()) && !Util::javaSuffix.contains(file.suffix()) &&
                 !Util::javaSuffix.contains(file.suffix()))
        {
            if (appwin->currentWindow())
                appwin->currentWindow()->statusBar()->showMessage(
                    QString(tr("Error in Opening: %1 is not a source file of C/C++, Java or Python")).arg(ex.args),
                    STATUS_RESPONSE_TIMEOUT);

            return {true, ""};
        }
        else
        {
            return {true, ex.args};
        }
    }
    return {false, ""};
}

bool FakeVimCommand::wantCompile(FakeVim::Internal::ExCommand const &ex)
{
    return ex.cmd == "compile" || ex.cmd == "cmp";
}

int FakeVimCommand::wantCompileRun(FakeVim::Internal::ExCommand const &ex)
{
    return -1;
}

int FakeVimCommand::wantDetachedRun(FakeVim::Internal::ExCommand const &ex)
{
    return -1;
}

int FakeVimCommand::wantRun(FakeVim::Internal::ExCommand const &ex)
{
    return -1;
}
bool FakeVimCommand::wantKillAll(FakeVim::Internal::ExCommand const &ex)
{
    return ex.cmd == "killall" || ex.cmd == "kap"; // kill all process
}
bool FakeVimCommand::wantFormat(FakeVim::Internal::ExCommand const &ex)
{
    return ex.cmd == "format" || ex.cmd == "fmt";
}
bool FakeVimCommand::wantSnippets(FakeVim::Internal::ExCommand const &ex)
{
    return ex.cmd == "snippet" || ex.cmd == "snp";
}
QString FakeVimCommand::wantVModeChange(FakeVim::Internal::ExCommand const &ex)
{
    return "";
}
bool FakeVimCommand::wantPreferences(FakeVim::Internal::ExCommand const &ex)
{
    return ex.cmd == "preference" || ex.cmd == "prf";
}
QString FakeVimCommand::wantLanguageChange(FakeVim::Internal::ExCommand const &ex)
{
    return "";
}
bool FakeVimCommand::wantExit(FakeVim::Internal::ExCommand const &ex)
{
    return ex.cmd == "exit" || ex.cmd == "ext";
}

bool FakeVimCommand::wantMessageLoggerClear(FakeVim::Internal::ExCommand const &ex)
{
    return ex.cmd == "clear" || ex.cmd == "clr";
}
} // namespace Core

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
#include "fakevimhandler.h"
#include <Core/FakeVimCommands.hpp>
#include <QMainWindow>
#include <appwindow.hpp>

namespace Core
{
FakeVimCommand::FakeVimCommand(QMainWindow *aw) : appwindow(aw)
{
}

bool FakeVimCommand::handle(FakeVim::Internal::ExCommand const &cmd)
{
    auto appwin = qobject_cast<AppWindow *>(appwindow);
    if (!appwin)
        return false;

    if (!wantNewFile(cmd).isEmpty())
    {
    }
    if (!wantOpenFile(cmd).isEmpty())
    {
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

QString FakeVimCommand::wantNewFile(FakeVim::Internal::ExCommand const &ex)
{
    return "";
}
QString FakeVimCommand::wantOpenFile(FakeVim::Internal::ExCommand const &ex)
{
    return "";
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

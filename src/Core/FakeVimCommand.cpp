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
#include "Widgets/TestCases.hpp"
#include "appwindow.hpp"
#include "fakevimhandler.h"
#include "generated/SettingsHelper.hpp"
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QStatusBar>

namespace Core
{

static int const STATUS_MESSAGE_TIMEOUT = 3000;

FakeVimCommand::FakeVimCommand(AppWindow *aw) : QObject(aw), appwin(aw)
{
}

FakeVimCommand::CommandTypes FakeVimCommand::customCommandType(FakeVim::Internal::ExCommand const &ex)
{
    if (ex.cmd == "new")
        return CommandTypes::NEW;

    if (ex.cmd == "open" || ex.cmd == "opn")
        return CommandTypes::OPEN;

    if (ex.cmd == "compile" || ex.cmd == "cmp")
        return CommandTypes::COMPILE;

    if (ex.cmd == "crun" || ex.cmd == "crn")
        return CommandTypes::COMPILE_RUN;

    if (ex.cmd == "run")
        return CommandTypes::RUN;

    if (ex.cmd == "drun" || ex.cmd == "drn")
        return CommandTypes::DETACHED_RUN;

    if (ex.cmd == "killall" || ex.cmd == "kap")
        return CommandTypes::KILL_PROCESS;

    if (ex.cmd == "format" || ex.cmd == "fmt")
        return CommandTypes::FORMAT_CODE;

    if (ex.cmd == "snippet" || ex.cmd == "snp")
        return CommandTypes::SNIPPETS;

    if (ex.cmd == "vmode" || ex.cmd == "vmd")
        return CommandTypes::VMODE;

    if (ex.cmd == "preference" || ex.cmd == "prf")
        return CommandTypes::PREFERENCE;

    if (ex.cmd == "lang" || ex.cmd == "lng")
        return CommandTypes::CHLANG;

    if (ex.cmd == "clear" || ex.cmd == "clr")
        return CommandTypes::CLEAR;

    if (ex.cmd == "exit" || ex.cmd == "ext")
        return CommandTypes::EXIT;

    return CommandTypes::UNKNOWN;
}

bool FakeVimCommand::handleCustomCommand(CommandTypes type, QString const &args, bool hasbang)
{
    if (type == CommandTypes::UNKNOWN)
        return false;

    switch (type)
    {
    case CommandTypes::NEW: {
        QString lang;

        if (args == "c++" || args == "cpp")
            lang = "C++";
        else if (args == "py" || args == "python")
            lang = "Python";
        else if (args == "java")
            lang = "Java";
        else if (args.isEmpty())
            lang = SettingsHelper::getDefaultLanguage();

        if (!lang.isEmpty())
            appwin->openTab("", lang);
        else
            showError(tr("new requires empty or one of 'cpp', 'java' and 'python' argument, got %1").arg(args));
        break;
    }

    case CommandTypes::OPEN: {
        QString path = args;
        if (path.isEmpty())
        {
            showError(tr("open requires a file path"));
            break;
        }

        if (path.startsWith('~'))
            path = QDir::home().absolutePath() + "/" + args.mid(1);

        QFileInfo file(path);
        if (file.isRelative())
            file.makeAbsolute();

        if (!Util::cppSuffix.contains(file.suffix()) && !Util::javaSuffix.contains(file.suffix()) &&
            !Util::javaSuffix.contains(file.suffix()))
        {
            showError(tr("%1 is not c++, python or java source file").arg(file.absoluteFilePath()));
            break;
        }

        if (!file.exists() && !hasbang)
        {
            showError(
                tr("%1 does not exists. To open non-existing file use open! instead").arg(file.absoluteFilePath()));
            break;
        }

        appwin->openTab(file.absoluteFilePath());
        break;
    }
    case CommandTypes::COMPILE: {
        appwin->on_actionCompile_triggered();
        break;
    }
    case CommandTypes::COMPILE_RUN: {
        appwin->on_actionCompileRun_triggered();
        break;
    }
    case CommandTypes::RUN: {
        int ans = -1;
        if (args.isEmpty())
            ans = 0;
        bool ok = false;
        int caseNum = args.toInt(&ok);
        if (ok)
            ans = caseNum;
        else if (!args.isEmpty())
        {
            showError(tr("%1 is not a number").arg(caseNum));
            break;
        }

        // Parsed index correctly
        if (ans == 0)
            appwin->on_actionRun_triggered();
        else if (ans > 0 && appwin->currentWindow() && appwin->currentWindow()->testcases &&
                 appwin->currentWindow()->testcases->count() >= ans)
            appwin->currentWindow()->runTestCase(ans - 1);
        else
            showError(tr("cannot run testcase %1").arg(args));
        break;
    }
    case CommandTypes::DETACHED_RUN: {
        appwin->on_actionRunDetached_triggered();
    }
    break;
    case CommandTypes::KILL_PROCESS: {
        appwin->on_actionKillProcesses_triggered();
        break;
    }
    case CommandTypes::FORMAT_CODE: {
        appwin->on_actionFormatCode_triggered();
        break;
    }
    case CommandTypes::SNIPPETS: {
        appwin->on_actionUseSnippets_triggered();
        break;
    }
    case CommandTypes::VMODE: {
        if (args == "edit")
            appwin->on_actionEditorMode_triggered();
        else if (args == "split")
            appwin->on_actionSplitMode_triggered();
        else
            showError(tr("%1 is not a valid view mode. It should be one of 'split' or 'edit'").arg(args));
        break;
    }
    case CommandTypes::PREFERENCE: {
        appwin->on_actionSettings_triggered();
        break;
    }
    case CommandTypes::CHLANG: {
        QString lang;

        if (args == "cpp" || args == "c++")
            lang = "C++";
        else if (args == "java")
            lang = "Java";
        else if (args == "py" || args == "python")
            lang = "Python";

        if (!lang.isEmpty() && appwin->currentWindow())
            appwin->currentWindow()->setLanguage(lang);
        else if (appwin->currentWindow())
            showError(tr("%1 is not a valid language name. It should be one of 'cpp', 'java' or 'python'").arg(args));
        break;
    }
    case CommandTypes::CLEAR: {
        appwin->currentWindow()->on_clearMessagesButton_clicked();
        break;
    }
    case CommandTypes::EXIT: {
        appwin->on_actionQuit_triggered();
        break;
    }
    case CommandTypes::UNKNOWN: {
        Q_UNREACHABLE();
    }
    };
    return true;
}

void FakeVimCommand::showError(QString const &message)
{
    if (appwin->currentWindow())
        appwin->currentWindow()->statusBar()->showMessage(message, STATUS_MESSAGE_TIMEOUT);
}
} // namespace Core

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

#ifndef FAKE_VIM_COMMAND
#define FAKE_VIM_COMMAND

#define STATUS_RESPONSE_TIMEOUT 3000

#include <QObject>
namespace FakeVim
{
namespace Internal
{
class ExCommand;
}
} // namespace FakeVim

class QString;
class AppWindow;
template <class A, class B> class QPair;

namespace Core
{
class FakeVimCommand : public QObject
{
    Q_OBJECT
  public:
    explicit FakeVimCommand(AppWindow *window);

    bool handle(FakeVim::Internal::ExCommand const &ex);

  private:
    QPair<bool, QString> wantNewFile(FakeVim::Internal::ExCommand const &ex);
    QPair<bool, QString> wantOpenFile(FakeVim::Internal::ExCommand const &ex);
    bool wantCompile(FakeVim::Internal::ExCommand const &ex);
    int wantCompileRun(FakeVim::Internal::ExCommand const &ex);
    int wantDetachedRun(FakeVim::Internal::ExCommand const &ex);
    int wantRun(FakeVim::Internal::ExCommand const &ex);
    bool wantKillAll(FakeVim::Internal::ExCommand const &ex);
    bool wantFormat(FakeVim::Internal::ExCommand const &ex);
    bool wantSnippets(FakeVim::Internal::ExCommand const &ex);
    QString wantVModeChange(FakeVim::Internal::ExCommand const &ex);
    bool wantPreferences(FakeVim::Internal::ExCommand const &ex);
    QString wantLanguageChange(FakeVim::Internal::ExCommand const &ex);
    bool wantExit(FakeVim::Internal::ExCommand const &ex);
    bool wantMessageLoggerClear(FakeVim::Internal::ExCommand const &ex);

    AppWindow *appwin;
};
} // namespace Core

#endif

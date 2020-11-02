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
    /**
     * @brief Constructs a new handler which performs actions using/on the provided appwindow
     */
    explicit FakeVimCommand(AppWindow *window);

    /**
     * @brief Tries to handle passed command.
     * @returns true if the command was handled otherwise false
     * @note Custom commands are handled before Vim Commands
     */
    bool handle(FakeVim::Internal::ExCommand const &ex);

  private:
    /**
     * @brief Helper to check if command is to create a new file
     * @returns {true, language} representing that command does want to open a new file with given language.
     * @note if empty language is returned, it means user command was invalid and no action should be performed.
     */
    QPair<bool, QString> wantNewFile(FakeVim::Internal::ExCommand const &ex);

    /**
     * @brief Same as wantNewFile except that
     * @returns {true, filepath} representing the filepath to open
     */
    QPair<bool, QString> wantOpenFile(FakeVim::Internal::ExCommand const &ex);

    /**
     * @brief Checks if command is to run the given testcase
     * @returns {true, natural_number} means run only the nth testcase. (Including inactive/hidden testcases)
     * @note {true, 0} means run all testcase and {true, -1} means invalid index of testcase was provided
     * @subnote If argument is missing to the command, running all testcase is assumed.
     */
    QPair<bool, int> wantRun(FakeVim::Internal::ExCommand const &ex);

    /**
     * @note Same as others except that empty second value of pair means invalid mode
     * @subnote IO mode is not supported because in that mode, the editor is hidden and
     * vim command statusBar becomes inactive
     */
    QPair<bool, QString> wantVModeChange(FakeVim::Internal::ExCommand const &ex);
    /**
     * @note Same as others, except an empty second value of pair means incorrect language name
     * @subnote The returned language name is one of {C++, Java, Python}
     */
    QPair<bool, QString> wantLanguageChange(FakeVim::Internal::ExCommand const &ex);

    /**
     * @brief Checks if command is to perform the action as mentioned by function name
     * @returns true if command is matched with function name action otherwise false
     */
    bool wantCompile(FakeVim::Internal::ExCommand const &ex);
    bool wantCompileRun(FakeVim::Internal::ExCommand const &ex);
    bool wantDetachedRun(FakeVim::Internal::ExCommand const &ex);
    bool wantKillAll(FakeVim::Internal::ExCommand const &ex);
    bool wantFormat(FakeVim::Internal::ExCommand const &ex);
    bool wantSnippets(FakeVim::Internal::ExCommand const &ex);
    bool wantPreferences(FakeVim::Internal::ExCommand const &ex);
    bool wantExit(FakeVim::Internal::ExCommand const &ex);
    bool wantMessageLoggerClear(FakeVim::Internal::ExCommand const &ex);

    AppWindow *appwin;

    void showError(QString const &message);
};
} // namespace Core

#endif

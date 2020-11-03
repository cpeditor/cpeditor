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
    enum class CommandTypes
    {
        NEW,
		NEXT_TAB,
		LAST_TAB,
        OPEN,
        COMPILE,
        COMPILE_RUN,
        RUN,
        DETACHED_RUN,
        KILL_PROCESS,
        FORMAT_CODE,
        SNIPPETS,
        VMODE,
        PREFERENCE,
        CHLANG,
        CLEAR,
        EXIT,

        UNKNOWN
    };

    /**
     * @brief Constructs a new handler which performs actions using/on the provided appwindow
     */
    explicit FakeVimCommand(AppWindow *window);

    /**
     * @brief retrieve the type of custom command.
     * @returns enum CommandTypes that represents the command type.
     * @note if not a custom command or invalid command, the CommandTypes::UNKNOWN is returned
     */
    CommandTypes customCommandType(FakeVim::Internal::ExCommand const &ex);

    /**
     * @brief Tries to handle passed command type with commandArgs and bang.
     * @returns true if the command was handled otherwise false
     * @note Custom commands are handled before Vim Commands
     */
    bool handleCustomCommand(CommandTypes type, QString const &commandArgs, bool hasBang = false);

  private:
    AppWindow *appwin;

    void showError(QString const &message);
};
} // namespace Core

#endif

/*
 * Copyright (C) 2019-2021 Ashar Khan <ashar786khan@gmail.com>
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

#ifndef PROCESSUTIL_HPP
#define PROCESSUTIL_HPP

#include <QPair>
#include <QString>
#include <QStringList>

class QProcess;

namespace Util
{

/**
 * @brief Checks whether CP Editor is running inside a Flatpak sandbox.
 */
bool isFlatpak();

/**
 * @brief Gets the program and the arguments to use to run a command.
 *
 * Inside a Flatpak sandbox, programs installed on the host are not available. In that case the
 * command is wrapped with `flatpak-spawn --host --watch-bus` so that it runs on the host, which
 * makes arbitrary user-configured programs (compilers, LSPs, formatters, ...) work without
 * needing a wrapper for every single one of them.
 *
 * Outside Flatpak, the command is returned unchanged.
 */
QPair<QString, QStringList> hostCommand(const QString &program, const QStringList &args);

/**
 * @brief Starts a QProcess with @p program and @p args, transparently running it on the host
 * inside a Flatpak sandbox.
 */
void startProcess(QProcess *process, const QString &program, const QStringList &args);

} // namespace Util

#endif // PROCESSUTIL_HPP

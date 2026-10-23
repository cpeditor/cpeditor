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

#include "Util/ProcessUtil.hpp"
#include "Core/EventLogger.hpp"
#include <QFileInfo>
#include <QProcess>
#include <QStandardPaths>

namespace Util
{

bool isFlatpak()
{
    // FLATPAK_ID is set by Flatpak for every sandboxed application. We also make sure that
    // flatpak-spawn is available, otherwise wrapping commands would only make things worse.
    static const bool isFlatpakSandbox = qEnvironmentVariableIsSet("FLATPAK_ID") &&
                                         !QStandardPaths::findExecutable(QStringLiteral("flatpak-spawn")).isEmpty();
    return isFlatpakSandbox;
}

QPair<QString, QStringList> hostCommand(const QString &program, const QStringList &args)
{
    static const QString flatpakSpawn = QStringLiteral("flatpak-spawn");

    // Don't wrap:
    //  - commands that are already run through flatpak-spawn, since they are handled explicitly;
    //  - programs bundled inside `/app` with the application itself, which only exist in the sandbox.
    if (program.isEmpty() || !isFlatpak() || QFileInfo(program).fileName() == flatpakSpawn ||
        program.startsWith(QStringLiteral("/app/")))
        return {program, args};

    LOG_INFO("Running " << program << " on the host via flatpak-spawn");

    // --watch-bus makes the host process exit when flatpak-spawn exits, e.g. when the QProcess is
    // killed due to a time limit, so that no orphan processes are left running on the host.
    return {flatpakSpawn, QStringList{"--host", "--watch-bus", program} + args};
}

void startProcess(QProcess *process, const QString &program, const QStringList &args)
{
    const auto command = hostCommand(program, args);
    process->start(command.first, command.second);
}

} // namespace Util

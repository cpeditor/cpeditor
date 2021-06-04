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

#include "application.hpp"
#include "Core/EventLogger.hpp"
#include "appwindow.hpp"
#include <QFileOpenEvent>

Application::Application(int argc, char **argv) : SingleApplication(argc, argv, true)
{
}

bool Application::event(QEvent *event)
{
#ifdef Q_OS_MAC
    if (event->type() == QEvent::FileOpen)
    {
        const auto *openEvent = qobject_cast<QFileOpenEvent *>(event);
        const auto file = openEvent->file();
        LOG_INFO("Opening file : " << file);
        emit requestOpenFile(file);
    }
#endif
    return QApplication::event(event);
}

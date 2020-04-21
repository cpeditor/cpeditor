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

#include "signal.hpp"

#ifdef Q_OS_WIN32

#include <Windows.h>

#else

#include <signal.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#endif

#ifdef Q_OS_WIN32

static Daemon *daemon;

BOOL WINAPI HandlerRoutine(DWORD type)
{
    switch (type)
    {
    case CTRL_C_EVENT:
    case CTRL_CLOSE_EVENT:
        if (daemon)
            daemon->handleSignal();
        return TRUE;
    default:
        return FALSE;
    }
}

#else
int sigFd[2];

void signalHandler(int)
{
    char a = 1;
    ::write(sigFd[0], &a, sizeof(a));
}
#endif

Daemon::Daemon(QObject *parent) : QObject(parent)
{
#ifdef Q_OS_WIN32
    daemon = this;
#else
    if (::socketpair(AF_UNIX, SOCK_STREAM, 0, sigFd))
        qFatal("Couldn't create socketpair");

    notifier = new QSocketNotifier(sigFd[1], QSocketNotifier::Read, this);
    connect(notifier, SIGNAL(activated(int)), this, SLOT(handleSignal()));
#endif
}

Daemon::~Daemon()
{
    delete notifier;
}

void Daemon::setup()
{
#ifdef Q_OS_WIN32
    SetConsoleCtrlHandler(HandlerRoutine, TRUE);
#else
    struct sigaction act;

    act.sa_handler = signalHandler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    act.sa_flags |= SA_RESTART;

    sigaction(SIGINT, &act, 0);
    sigaction(SIGTERM, &act, 0);
#endif
}

void Daemon::handleSignal()
{
#ifndef Q_OS_WIN32
    notifier->setEnabled(false);
    char tmp;
    ::read(sigFd[1], &tmp, sizeof(tmp));
#endif
    emit signalActivated();
#ifndef Q_OS_WIN32
    notifier->setEnabled(true);
#endif
}
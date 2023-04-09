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

/**
 * based on https://stackoverflow.com/a/7582555/12601364 & https://doc.qt.io/qt-5/unix-signals.html
 */

#include "SignalHandler.hpp"
#include <cassert>

#ifndef _WIN32

#include <QSocketNotifier>
#include <csignal>
#include <sys/socket.h>
#include <unistd.h>

#else

#include <set>
#include <windows.h>

#endif //!_WIN32

// There can be only ONE SignalHandler per process
SignalHandler *g_handler = nullptr;

#ifdef _WIN32

BOOL WINAPI WIN32_handleFunc(DWORD /*signal*/);
int WIN32_physicalToLogical(DWORD /*signal*/);
DWORD WIN32_logicalToPhysical(int /*signal*/);
std::set<int> g_registry;

#else //_WIN32

static int socketFd[32][2];
void POSIX_handleFunc(int signal);
int POSIX_physicalToLogical(int signal);
int POSIX_logicalToPhysical(int signal);

#endif //_WIN32

SignalHandler::SignalHandler(int mask) : _mask(mask)
{
    assert(g_handler == nullptr);
    g_handler = this;

#ifdef _WIN32
    SetConsoleCtrlHandler(WIN32_handleFunc, TRUE);
#endif //_WIN32

    for (int i = 0; i < numSignals; i++)
    {
        int logical = 0x1 << i;
        if (_mask & logical)
        {
#ifdef _WIN32
            g_registry.insert(logical);
#else
            assert(!::socketpair(AF_UNIX, SOCK_STREAM, 0, socketFd[logical]));
            auto *sn = new QSocketNotifier(socketFd[logical][1], QSocketNotifier::Read, this);
            connect(sn, &QSocketNotifier::activated, this, [=] {
                sn->setEnabled(false);
                char tmp = 0;
                ::read(socketFd[logical][1], &tmp, sizeof(tmp));
                handleSignal(logical);
                sn->setEnabled(true);
            });
            const int sig = POSIX_logicalToPhysical(logical);
            struct sigaction sa;              // NOLINT
            sa.sa_handler = POSIX_handleFunc; // NOLINT
            sigemptyset(&sa.sa_mask);
            sa.sa_flags = SA_RESTART;
            assert(!sigaction(sig, &sa, nullptr));
#endif //_WIN32
        }
    }
}

SignalHandler::~SignalHandler()
{
#ifdef _WIN32
    SetConsoleCtrlHandler(WIN32_handleFunc, FALSE);
#else
    for (int i = 0; i < numSignals; i++)
    {
        int logical = 0x1 << i;
        if (_mask & logical)
        {
            signal(POSIX_logicalToPhysical(logical), SIG_DFL);
        }
    }
#endif //_WIN32
}

#ifdef _WIN32
DWORD WIN32_logicalToPhysical(int signal)
{
    switch (signal)
    {
    case SignalHandler::SIG_INT:
        return CTRL_C_EVENT;
    case SignalHandler::SIG_TERM:
        return CTRL_BREAK_EVENT;
    case SignalHandler::SIG_CLOSE:
        return CTRL_CLOSE_EVENT;
    default:
        return ~(unsigned int)0; // SIG_ERR = -1
    }
}
#else
int POSIX_logicalToPhysical(int signal)
{
    switch (signal)
    {
    case SignalHandler::SIG_INT:
        return SIGINT;
    case SignalHandler::SIG_TERM: // NOLINT: switch has 2 consecutive identical branches [bugprone-branch-clone]
        return SIGTERM;
    // In case the client asks for a SIG_CLOSE handler, accept and
    // bind it to a SIGTERM. Anyway the signal will never be raised
    case SignalHandler::SIG_CLOSE:
        return SIGTERM;
    case SignalHandler::SIG_RELOAD:
        return SIGHUP;
    default:
        return -1; // SIG_ERR = -1
    }
}
#endif //_WIN32

#ifdef _WIN32
int WIN32_physicalToLogical(DWORD signal)
{
    switch (signal)
    {
    case CTRL_C_EVENT:
        return SignalHandler::SIG_INT;
    case CTRL_BREAK_EVENT:
        return SignalHandler::SIG_TERM;
    case CTRL_CLOSE_EVENT:
        return SignalHandler::SIG_CLOSE;
    default:
        return SignalHandler::SIG_UNHANDLED;
    }
}
#else
int POSIX_physicalToLogical(int signal)
{
    switch (signal)
    {
    case SIGINT:
        return SignalHandler::SIG_INT;
    case SIGTERM:
        return SignalHandler::SIG_TERM;
    case SIGHUP:
        return SignalHandler::SIG_RELOAD;
    default:
        return SignalHandler::SIG_UNHANDLED;
    }
}
#endif //_WIN32

#ifdef _WIN32
BOOL WINAPI WIN32_handleFunc(DWORD signal)
{
    if (g_handler)
    {
        int signo = WIN32_physicalToLogical(signal);
        // The std::set is thread-safe in const reading access and we never
        // write to it after the program has started so we don't need to
        // protect this search by a mutex
        auto found = g_registry.find(signo);
        if (signo != -1 && found != g_registry.end())
            return g_handler->handleSignal(signo) ? TRUE : FALSE;
        return FALSE;
    }
    return FALSE;
}
#else
void POSIX_handleFunc(int signal)
{
    if (g_handler)
    {
        int signo = POSIX_physicalToLogical(signal);
        char a = 1;
        ::write(socketFd[signo][0], &a, sizeof(a));
    }
}
#endif //_WIN32

bool SignalHandler::handleSignal(int signal)
{
    emit signalReceived(signal);
    return false;
}

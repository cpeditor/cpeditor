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

/**
 * This class is based on https://stackoverflow.com/a/7582555/12601364
 */

#include "SignalHandler.hpp"
#include <cassert>

#ifndef _WIN32

#include <csignal>

#else

#include <set>
#include <windows.h>

#endif //!_WIN32

// There can be only ONE SignalHandler per process
SignalHandler *g_handler = nullptr; // NOLINT: variable 'g_handler' provides global access to a non-const object;
                                    // consider making the pointed-to data 'const'

#ifdef _WIN32

BOOL WINAPI WIN32_handleFunc(DWORD);
int WIN32_physicalToLogical(DWORD);
DWORD WIN32_logicalToPhysical(int);
std::set<int> g_registry;

#else //_WIN32

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
            int sig = POSIX_logicalToPhysical(logical);
            bool failed = signal(sig, POSIX_handleFunc) ==
                          SIG_ERR; // NOLINT: do not use C-style cast to convert between unrelated types
            assert(!failed);
            (void)failed; // Silence the warning in non _DEBUG; TODO: something better

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
        std::set<int>::const_iterator found = g_registry.find(signo);
        if (signo != -1 && found != g_registry.end())
        {
            return g_handler->handleSignal(signo) ? TRUE : FALSE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }
}
#else
void POSIX_handleFunc(int signal)
{
    if (g_handler)
    {
        int signo = POSIX_physicalToLogical(signal);
        g_handler->handleSignal(signo);
    }
}
#endif //_WIN32

bool SignalHandler::handleSignal(int signal)
{
    emit signalReceived(signal);
    return false;
}

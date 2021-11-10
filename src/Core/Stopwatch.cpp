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

#include "Core/Stopwatch.hpp"
#include "generated/SettingsHelper.hpp"

#include <QCoreApplication>
#include <QTimerEvent>

namespace Core
{

Stopwatch::Stopwatch(QObject *parent) : QObject(parent)
{
}

void Stopwatch::setGranularity(Granularity g)
{
    granularity = g;

    // whenever granularity is changed, also apply that granularity to the timer event
    if (timerId != INACTIVE_TIMER_ID)
    {
        killTimer(timerId);
        timerId = startTimer(getGranularity());
    }
}

bool Stopwatch::isRunning() const
{
    return currentState == State::Running;
}

bool Stopwatch::isPaused() const
{
    return currentState == State::Paused;
}

bool Stopwatch::isInactive() const
{
    return currentState == State::Inactive;
}

void Stopwatch::start()
{
    if (isInactive())
    {
        accumulator = 0;
        elapsedTimer.start();

        if (timerId == INACTIVE_TIMER_ID)
        {
            timerId = startTimer(getGranularity());
        }
    }
    else if (isPaused())
    {
        elapsedTimer.restart();
        timerId = startTimer(getGranularity());
    }

    currentState = State::Running;
    emit time(accumulator);
    emit stateChanged(State::Running);
}

void Stopwatch::pause()
{
    if (elapsedTimer.isValid())
    {
        accumulator += elapsedTimer.elapsed();
    }

    elapsedTimer.invalidate();
    currentState = State::Paused;
    emit time(accumulator);
    emit stateChanged(State::Paused);
}

void Stopwatch::reset()
{
    elapsedTimer.invalidate(); // if state is running, it will emit a zero time at next timerEvent() call
    QCoreApplication::processEvents();
    emit time(0);
    currentState = State::Inactive;
    emit stateChanged(State::Inactive);
}

void Stopwatch::timerEvent(QTimerEvent *event)
{
    if (event->timerId() != timerId)
    { // forward undesired events
        QObject::timerEvent(event);
        return;
    }

    if (elapsedTimer.isValid())
    {
        emit time(accumulator + elapsedTimer.elapsed());
    }
    else
    {
        killTimer(timerId);
        timerId = INACTIVE_TIMER_ID;
    }
}

int Stopwatch::getGranularity() const
{
    return static_cast<int>(granularity);
}

} // namespace Core

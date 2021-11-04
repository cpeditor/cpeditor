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

#ifndef STOPWATCH_HPP
#define STOPWATCH_HPP

#include <QElapsedTimer>
#include <QObject>

namespace Core
{
class Stopwatch : public QObject
{
    Q_OBJECT

  public:
    enum class Granularity : int
    {
        Milliseconds = 1, // Stopwatch refreshed every msec.
        Hundredths = 10,  // Stopwatch refreshed every 10 msec.
        Tenths = 100,     // Stopwatch refreshed every 100 msec.
        Seconds = 1000    // Stopwatch refreshed every sec.
    };

    explicit Stopwatch(QObject *parent = nullptr);

    /**
     * @brief set the stopwatch refresh granularity
     * @param g The granularity to be set.
     */
    void setGranularity(Granularity g);

    /**
     * @brief check if the stopwatch is running
     * @return true if running, false otherwise
     */
    bool isRunning() const;

    /**
     * @brief check if the stopwatch is paused
     * @return true if paused, false otherwise
     */
    bool isPaused() const;

    /**
     * @brief check if the stopwatch is inactive
     * @return true if inactive, false otherwise
     */
    bool isInactive() const;

  public slots:
    /**
     * @brief start the stopwatch, if inactive or paused.
     */
    void start();

    /**
     * @brief pause the stopwatch, is running.
     */
    void pause();

    /**
     * @brief reset to stopwatch to inactive state.
     */
    void reset();

  signals:
    /**
     * Emits a signal with the current stopwatch time.
     * @param t Current stopwatch time.
     */
    void time(int t);

  protected:
    void timerEvent(QTimerEvent *event) override;

  private:
    enum class State
    {
        Inactive,
        Paused,
        Running
    };

    int getGranularity() const;

    static constexpr int INACTIVE_TIMER_ID = -1;

    int timerId = INACTIVE_TIMER_ID;
    int accumulator = 0; // milisecond internal counter
    State currentState = State::Inactive;
    Granularity granularity = Granularity::Seconds;

    QElapsedTimer elapsedTimer; // Stopwatch core class
};

} // namespace Core

#endif // STOPWATCH_HPP
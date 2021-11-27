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
#include <QWidget>

class QLabel;
class QPushButton;

namespace Widgets
{
class Stopwatch : public QWidget
{
    Q_OBJECT

  public:
    /**
     * @brief construct a Stopwatch
     */
    explicit Stopwatch(QWidget *parent = nullptr);

    /**
     * @brief check if the stopwatch is running.
     * @return true if running, false otherwise
     */
    bool isRunning() const;

    /**
     * @brief check if the stopwatch is paused.
     * @return true if paused, false otherwise
     */
    bool isPaused() const;

    /**
     * @brief check if the stopwatch is inactive.
     * @return true if inactive, false otherwise
     */
    bool isInactive() const;

    /**
     * @brief update according to the "Hide Stopwatch Result" setting
     */
    void updateHideResult();

  public slots:
    /**
     * @brief start the stopwatch, if inactive or paused.
     */
    void start();

    /**
     * @brief pause the stopwatch, if running.
     */
    void pause();

    /**
     * @brief reset to stopwatch to inactive state.
     */
    void reset();

    /**
     * @brief start the stopwatch if inactive or paused, pause the stopwatch if running
     */
    void startOrPause();

  private:
    void update();

    void setupSingleShot();

    qint64 totalMilliseconds() const;

  private slots:
    void updateUi();

  private:
    enum class State : int
    {
        Inactive,
        Paused,
        Running
    };

    /**
     * The UI structure:
     *
     * mainLayout:
     *  - timeLabel
     *  - showButton
     *  - startPauseButton
     *  - resetButton
     */
    QLabel *timeLabel = nullptr;
    QPushButton *showButton = nullptr;
    QPushButton *startPauseButton = nullptr;
    QPushButton *resetButton = nullptr;

    qint64 accumulator = 0; // milisecond internal counter
    State currentState = State::Inactive;
    static const int GRANULARITY = 1000; // stopwatch refreshes every second

    QElapsedTimer elapsedTimer;
    QTimer *timer;
};

} // namespace Widgets

#endif // STOPWATCH_HPP

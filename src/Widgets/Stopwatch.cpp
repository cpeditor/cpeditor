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

#include "Widgets/Stopwatch.hpp"
#include "generated/SettingsHelper.hpp"

#include <QCoreApplication>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QString>
#include <QTime>
#include <QTimer>

namespace Widgets
{

Stopwatch::Stopwatch(QWidget *parent) : QWidget(parent)
{
    // construct widgets
    QLayout *mainLayout = new QHBoxLayout(this);
    timeLabel = new QLabel("00:00:00");
    timeLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    startPauseButton = new QPushButton("Start");
    resetButton = new QPushButton("Reset");

    // set up UI
    mainLayout->addWidget(timeLabel);
    mainLayout->addWidget(startPauseButton);
    mainLayout->addWidget(resetButton);

    // set up timer
    timer = new QTimer(this);
    timer->callOnTimeout(this, &Stopwatch::update);

    connect(startPauseButton, &QPushButton::clicked, this, &Stopwatch::startOrPauseStopwatch);
    connect(resetButton, &QPushButton::clicked, this, &Stopwatch::resetStopwatch);
}

void Stopwatch::startOrPauseStopwatch()
{
    isRunning() ? pause() : start();
    update();
}

void Stopwatch::resetStopwatch()
{
    reset();
    update();
}

void Stopwatch::start()
{
    if (isInactive())
    {
        accumulator = 0;
        elapsedTimer.start();
    }
    else if (isPaused())
    {
        elapsedTimer.restart();
    }
    setupSingleShot();

    currentState = State::Running;
}

void Stopwatch::pause()
{
    if (elapsedTimer.isValid())
    {
        accumulator += (int)elapsedTimer.elapsed();
    }

    elapsedTimer.invalidate();
    timer->stop();
    currentState = State::Paused;
}

void Stopwatch::reset()
{
    elapsedTimer.invalidate();
    accumulator = 0;
    timer->stop();
    QCoreApplication::processEvents();
    currentState = State::Inactive;
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

void Stopwatch::update()
{
    if (elapsedTimer.isValid())
        updateUi(accumulator + (int)elapsedTimer.elapsed());
    else
        updateUi(accumulator);
    setupSingleShot();
}

void Stopwatch::updateUi(int ms)
{
    timeLabel->setText(QTime(0, 0).addMSecs(ms).toString("hh:mm:ss"));

    resetButton->setDisabled(isInactive());

    QString buttonText;
    switch (currentState)
    {
    case State::Inactive:
        buttonText = tr("Start");
        break;
    case State::Running:
        buttonText = tr("Pause");
        break;
    case State::Paused:
        buttonText = tr("Resume");
        break;
    }
    startPauseButton->setText(buttonText);
}

void Stopwatch::setupSingleShot()
{
    if (!timer)
        return;

    timer->setInterval(granularity + 10 - accumulator % granularity);
    timer->setSingleShot(true);
    if (isRunning())
        timer->start();
}

} // namespace Widgets

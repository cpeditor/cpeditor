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

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTime>
#include <QTimer>

namespace Widgets
{

Stopwatch::Stopwatch(QWidget *parent) : QWidget(parent)
{
    // construct widgets
    QLayout *mainLayout = new QHBoxLayout(this);
    auto *stopwatchLabel = new QLabel(tr("Stopwatch"));
    timeLabel = new QLabel("00:00:00");
    timeLabel->setAlignment(Qt::AlignVCenter);
    showButton = new QPushButton(tr("Show"));
    startPauseButton = new QPushButton(tr("Start"));
    resetButton = new QPushButton(tr("Reset"));

    // set up UI
    mainLayout->addWidget(stopwatchLabel);
    mainLayout->addWidget(timeLabel);
    mainLayout->addWidget(showButton);
    mainLayout->addWidget(startPauseButton);
    mainLayout->addWidget(resetButton);

    // set up timer
    timer = new QTimer(this);
    timer->callOnTimeout(this, &Stopwatch::update);

    connect(showButton, &QPushButton::pressed, this, &Stopwatch::updateUi);
    connect(showButton, &QPushButton::released, this, &Stopwatch::updateUi);
    connect(startPauseButton, &QPushButton::clicked, this, &Stopwatch::startOrPause);
    connect(resetButton, &QPushButton::clicked, this, &Stopwatch::reset);

    updateHideResult();
}

void Stopwatch::startOrPause()
{
    isRunning() ? pause() : start();
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

    update();
}

void Stopwatch::pause()
{
    if (elapsedTimer.isValid())
    {
        accumulator += elapsedTimer.elapsed();
    }

    elapsedTimer.invalidate();
    timer->stop();
    currentState = State::Paused;

    update();
}

void Stopwatch::reset()
{
    elapsedTimer.invalidate();
    accumulator = 0;
    timer->stop();
    currentState = State::Inactive;

    update();
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

void Stopwatch::updateHideResult()
{
    showButton->setVisible(SettingsHelper::isHideStopwatchResult());
    updateUi();
}

void Stopwatch::update()
{
    updateUi();
    setupSingleShot();
}

void Stopwatch::updateUi()
{
    const qint64 ms = totalMilliseconds();

    static const int SECSPERHOUR = 60 * 60 * 1000;

    if (!showButton->isDown() && SettingsHelper::isHideStopwatchResult())
    {
        timeLabel->setText("**:**:**");
    }
    else
    {
        if (ms < 24LL * SECSPERHOUR)
            timeLabel->setText(QTime(0, 0).addMSecs(int(ms)).toString("hh:mm:ss"));
        else
            timeLabel->setText(QString::number(ms / SECSPERHOUR) +
                               QTime(0, 0).addMSecs(int(ms % SECSPERHOUR)).toString(":mm:ss"));
    }

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

    timer->setInterval(GRANULARITY + 10 - int(totalMilliseconds() % GRANULARITY));
    timer->setSingleShot(true);
    if (isRunning())
        timer->start();
}

qint64 Stopwatch::totalMilliseconds() const
{
    if (elapsedTimer.isValid())
        return accumulator + elapsedTimer.elapsed();
    return accumulator;
}

} // namespace Widgets

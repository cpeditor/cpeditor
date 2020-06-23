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

#ifndef SESSION_MANAGER_HPP
#define SESSION_MANAGER_HPP

#include <QObject>

class AppWindow;
class QTimer;

namespace Core
{
class SessionManager : public QObject
{
    Q_OBJECT

  public:
    explicit SessionManager(AppWindow *appwindow);

    void restoreSession(const QString &path);

    void setAutoUpdateSession(bool shouldAutoUpdate);

    void setAutoUpdateDuration(unsigned int duration);

    QString currentSessionText();

    static QString lastSessionPath();

    static void saveSession(const QString &sessionText);

  public slots:
    void updateSession();

  private:
    QTimer *timer = nullptr;
    AppWindow *app = nullptr;
};
} // namespace Core

#endif

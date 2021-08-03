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

#ifndef WAKATIME_HPP
#define WAKATIME_HPP
#include <QObject>
#include <QTime>

namespace Extensions
{
class WakaTime : public QObject
{
    Q_OBJECT

  public:
    WakaTime(const QString &path, const QString &key, QObject *parent = nullptr);
    void sendHeartBeat(const QString &filePath, bool isWrite);
    void update(const QString &path, const QString &key);

  private:
    QString wakaTimePath;
    QString lastFilePath;
    QString apiKey;
    QTime lastHeartBeat;
};
} // namespace Extensions
#endif // WAKATIME_HPP

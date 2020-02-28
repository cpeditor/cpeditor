/*
 * Copyright (C) 2019-2020 Ashar Khan <ashar786khan@gmail.com>
 *
 * This file is part of CPEditor.
 *
 * CPEditor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * I will not be responsible if CPEditor behaves in unexpected way and
 * causes your ratings to go down and or lose any important contest.
 *
 * Believe Software is "Software" and it isn't immune to bugs.
 *
 */

#ifndef MESSAGELOGGER_HPP
#define MESSAGELOGGER_HPP
#include <QTextBrowser>

class MessageLogger
{
  public:
    MessageLogger() = default;

    void message(const QString &head, const QString &body, const QString &color);
    void warn(const QString &head, const QString &body);
    void info(const QString &head, const QString &body);
    void error(const QString &head, const QString &body);
    void clear();
    void setContainer(QTextBrowser *value);

  private:
    QTextBrowser *box;
};

#endif // MESSAGELOGGER_HPP

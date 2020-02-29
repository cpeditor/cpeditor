/*
 * Copyright (C) 2019-2020 Ashar Khan <ashar786khan@gmail.com>
 *
 * This file is part of cpeditor.
 *
 * cpeditor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * I will not be responsible if cpeditor behaves in unexpected way and
 * causes your ratings to go down and or lose any important contest.
 *
 * Believe Software is "Software" and it isn't immune to bugs.
 *
 */

#ifndef EVENTLOGGER_HPP
#define EVENTLOGGER_HPP

#include <QFile>
#include <QTextStream>

namespace Core
{

#define INFO_OF(x) #x ": " << x

class Log
{
  public:
    static void i(const QString &head, const QString &body);
    static void w(const QString &head, const QString &body);
    static void e(const QString &head, const QString &body);
    static void wtf(const QString &head, const QString &body);
    static QTextStream &i(const QString &head);
    static QTextStream &w(const QString &head);
    static QTextStream &e(const QString &head);
    static QTextStream &wtf(const QString &head);

    static void init(int instanceCount, bool dumpToStderr = false);
    static void clearOldLogs();
    static void revealInFileManager();

  private:
    static QString dateTimeStamp();
    static QString platformInformation();
    static void log(const QString &priority, const QString &head, const QString &body);
    static QTextStream &log(const QString &priority, const QString &head);

    static QTextStream logStream;
    static QFile logFile;
};

} // namespace Core

#endif // EVENTLOGGER_HPP

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

/*
 * The event logger is used for logging events of the editor.
 * The logs can helps the maintainers find the bug.
 */

#ifndef EVENTLOGGER_HPP
#define EVENTLOGGER_HPP

#include <QFile>
#include <QTextStream>

namespace Core
{

#define INFO_OF(x) #x ": " << x << " "                 // a useful macro to show a variable in the log
#define BOOLEAN(x) #x ": " << (x ? "true " : "false ") // a usefule macro to convert boolean type to string equivalent

class Log
{
  public:
    /*
     * There are four log levels:
     * 1. INFO(i): information, used when everything is normal
     * 2. WARN(w): warning, used when something strange happened, but it is not necessarily an error
     * 3. ERR(e): error, used when something bad happened
     * 4. WTF(wtf): what a terrible failure, used when it's considered impossible to happen
     *
     * You can use one of the two forms for logging:
     * 1. Core::Log::i/w/e/wtf(head, body);
     * 2. Core::Log::i/w/e/wtf(head) << body1 << body2 << ... << endl;
     *
     * Please remember to add endl when using the second form.
     */

    static void i(const QString &head, const QString &body);
    static void w(const QString &head, const QString &body);
    static void e(const QString &head, const QString &body);
    static void wtf(const QString &head, const QString &body);
    static QTextStream &i(const QString &head);
    static QTextStream &w(const QString &head);
    static QTextStream &e(const QString &head);
    static QTextStream &wtf(const QString &head);

    /*
     * @brief initialize the event logger
     * @param instanceCount the instance ID provided by SingleApplication, to distinct processes from each other
     * @param dumpToStderr whether to print the logs into stderr or not
     * @note this should be called only once before logging anything
     */
    static void init(int instanceCount, bool dumpToStderr = false);

    /*
     * @brief clear old logs
     * @note this clears all logs except the current one
     */
    static void clearOldLogs();

    /*
     * @brief reveal the current log file in the file manager
     */
    static void revealInFileManager();

  private:
    /*
     * @brief get the current datetime
     * @return the current datetime
     */
    static QString dateTimeStamp();

    /*
     * @brief get the platform info
     * @return the platform information
     */
    static QString platformInformation();

    /*
     * @brief log with a certain priority, head and body
     * @param priority one of "INFO", "WARN", "ERR", "WTF"
     * @param head ususally <class name>/<function name>, indicates where this message is from
     * @param body the main part of the message
     */
    static void log(const QString &priority, const QString &head, const QString &body);

    /*
     * @brief the text stream form of log
     * @param priority one of "INFO", "WARN", "ERR", "WTF"
     * @param head ususally <class name>/<function name>, indicates where this message is from
     * @note Please add endl manually when using the text stream form.
     */
    static QTextStream &log(const QString &priority, const QString &head);

    static QTextStream logStream; // the text stream for logging, writes to logFile
    static QFile logFile;         // the device for logging, a file or stderr
};

} // namespace Core

#endif // EVENTLOGGER_HPP

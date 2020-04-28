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

/**
 * There are four log levels:
 * 1. LOG_INFO(i): information, used when everything is normal
 * 2. LOG_WARN(w): warning, used when something strange happened, but it is not necessarily an error
 * 3. LOG_WARN_IF(cond, w): something strange will happen if condition is true
 * 4. LOG_ERR(e): error, used when something bad happened
 * 5. LOG_ERR_IF(cond, e): something bad will happen if condition is true
 * 6. LOG_WTF(wtf): what a terrible failure, used when it's considered impossible to happen
 */

// Log levels are defined as:
// WTF : Level 4
// ERR : Level 3
// WARN: Level 2
// INFO: Level 1

#define LOG_WTF(stream) Core::Log::log(" WTF ", __func__, __LINE__, __FILE__) << stream << endl;
#define LOG_ERR(stream) Core::Log::log("ERROR", __func__, __LINE__, __FILE__) << stream << endl;
#define LOG_WARN(stream) Core::Log::log("WARN ", __func__, __LINE__, __FILE__) << stream << endl;
#define LOG_INFO(stream) Core::Log::log("INFO ", __func__, __LINE__, __FILE__) << stream << endl;

#define LOG_INFO_IF(cond, stream)                                                                                      \
    if (cond)                                                                                                          \
    {                                                                                                                  \
        Core::Log::log("INFO ", __func__, __LINE__, __FILE__) << stream << endl;                                       \
    }

#define LOG_WARN_IF(cond, stream)                                                                                      \
    if (cond)                                                                                                          \
    {                                                                                                                  \
        Core::Log::log("WARN ", __func__, __LINE__, __FILE__) << stream << endl;                                       \
    }

#define LOG_ERR_IF(cond, stream)                                                                                       \
    if (cond)                                                                                                          \
    {                                                                                                                  \
        Core::Log::log("ERROR", __func__, __LINE__, __FILE__) << stream << endl;                                       \
    }

#define INFO_OF(variable) "<" #variable ">: [" << (variable) << "], "
#define BOOL_INFO_OF(variable) "<" #variable ">: [" << ((variable) ? "true" : "false") << "], "

namespace Core
{
class Log
{
  public:
    /**
     * @brief initialize the event logger
     * @param instanceCount the instance ID provided by SingleApplication, to distinct processes from each other
     * @param dumpToStderr whether to print the logs into stderr or not
     * @note this should be called only once before logging anything
     */
    static void init(int instanceCount, bool dumpToStderr = false);

    /**
     * @brief clear old logs
     * @note this clears all logs except the current one
     */
    static void clearOldLogs();

    /**
     * @brief reveal the current log file in the file manager
     */
    static void revealInFileManager();

    static QTextStream &log(const QString &priority, QString funcName, int lineNumber, QString fileName);

  private:
    static QString dateTimeStamp();
    static void platformInformation();

    static QTextStream logStream; // the text stream for logging, writes to logFile
    static QFile logFile;         // the device for logging, a file or stderr

    static int NUMBER_OF_LOGS_TO_KEEP;     // Number of log files to keep in Temporary directory
    static QString LOG_FILE_NAME;          // Base Name of the log file
    static QString LOG_DIR_NAME;           // Directory inside Temp where log files will be stored
    static int MAXIMUM_FUNCTION_NAME_SIZE; // Maximum size of function name, it is used to determine spacing in log file
    static int MAXIMUM_FILE_NAME_SIZE;     // Maximum size of file name, it is used to determine spacing in log file
};

} // namespace Core

#endif // EVENTLOGGER_HPP

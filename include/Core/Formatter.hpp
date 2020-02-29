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

#ifndef FORMATTER_HPP
#define FORMATTER_HPP
#include "Core/MessageLogger.hpp"
#include <QCodeEditor>
#include <QPair>
#include <QString>

namespace Core
{
class Formatter
{
  public:
    Formatter(const QString &clangFormatBinary, const QString &clangFormatStyle, MessageLogger *log);
    void format(QCodeEditor *editor, const QString &filePath, const QString &lang, bool selectionOnly);
    static bool check(const QString &checkBinary, const QString &checkStyle);
    void updateBinary(const QString &newBinary);
    void updateStyle(const QString &newStyle);

  private:
    QString binary;
    QString style;
    MessageLogger *log;

    QPair<int, QString> getFormatResult(const QStringList &args);
};

} // namespace Core
#endif // FORMATTER_HPP

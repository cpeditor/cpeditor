/*
 * Copyright (C) 2019 Ashar Khan <ashar786khan@gmail.com>
 *
 * This file is part of CPEditor.
 *
 * CPEditor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * I will not be responsible if CPEditor behaves in unexpected way and
 * causes your ratings to go down and or loose any important contest.
 *
 * Believe Software is "Software" and it isn't not immune to bugs.
 *
 */

#ifndef BASEFILES_HPP
#define BASEFILES_HPP

#include <QStandardPaths>
#include <QString>

namespace Core
{
namespace Base
{
class Files
{
  public:
    Files()
    {
        base_ = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    }
    QString getTempFormatFile()
    {
        return base_ + "/cp_editor_temp_format_file";
    }
    QString getProgramFile(QString ext = "")
    {
        return base_ + "/sol" + (ext.isEmpty() ? ".cpp" : ext);
    }
    QString getInput(int id)
    {
        return base_ + "/cp_editor_temp_in" + QString::number(id + 1) + "_file";
    }
    QString getBaseDirectory()
    {
        return base_;
    }
#ifndef _WIN32
    QString getBinaryOutput(QString ext = "")
    {
        return base_ + "/a" + (ext.isEmpty() ? ".out" : ext);
    }
#else
    QString getBinaryOutput(QString ext = "")
    {
        return base_ + "/a" + (ext.isEmpty() ? ".exe" : ext);
    }
#endif
  private:
    QString base_;
};
} // namespace Base

} // namespace Core
#endif // BASEFILES_HPP

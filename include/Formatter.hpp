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
* causes your ratings to go down and or loose any important contest.
* 
* Believe Software is "Software" and it isn't immune to bugs.
* 
*/


#ifndef FORMATTER_HPP
#define FORMATTER_HPP
#include <BaseFiles.hpp>
#include <MessageLogger.hpp>
#include <QCodeEditor>
#include <QFile>
#include <QString>

namespace Core
{
class Formatter : private Base::Files
{
  public:
    Formatter(QString runCommand, int index, MessageLogger *log);
    ~Formatter();
    void format(QCodeEditor *editor);
    static bool check(QString command);
    void updateCommand(QString newCommand);

  private:
    QFile *file;
    QString command;
    MessageLogger *log;
};

} // namespace Core
#endif // FORMATTER_HPP

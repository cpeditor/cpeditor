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
* Believe Software is "Software" and it isn't immune to bugs.
* 
*/


#ifndef MESSAGELOGGER_HPP
#define MESSAGELOGGER_HPP
#include <QTextBrowser>
#include <string>

class MessageLogger
{
  public:
    MessageLogger() = default;

    void warn(std::string head, std::string body, bool multiline = false);
    void info(std::string head, std::string body);
    void error(std::string head, std::string body, bool multiline = false);
    void clear();
    void setContainer(QTextBrowser *value);

  private:
    QTextBrowser *box;
};

#endif // MESSAGELOGGER_HPP

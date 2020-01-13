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


#ifndef IO_HPP
#define IO_HPP

#include <BaseFiles.hpp>
#include <QPlainTextEdit>
namespace Core
{
namespace IO
{
class InputReader : private Core::Base::Files
{
  public:
    InputReader(QVector<QPlainTextEdit *> _a, int index);
    void readToFile();
    ~InputReader();

  private:
    QVector<QPlainTextEdit *> a = QVector<QPlainTextEdit *>(3, nullptr);
    QVector<QFile *> f = QVector<QFile *>(3, nullptr);
};
} // namespace IO
} // namespace Core
#endif // IO_HPP

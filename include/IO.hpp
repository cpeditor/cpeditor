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


#ifndef IO_HPP
#define IO_HPP

#include <BaseFiles.hpp>
#include <QPlainTextEdit>
namespace Core {
namespace IO {
class InputReader : private Core::Base::Files {
 public:
  InputReader(QPlainTextEdit* a, QPlainTextEdit* b, QPlainTextEdit* c);
  void readToFile();
  ~InputReader();

 private:
  QPlainTextEdit *a_, *b_, *c_;
  QFile *f_a, *f_b, *f_c;
};
class OutputReader : private Core::Base::Files {
 public:
  OutputReader(QPlainTextEdit* a, QPlainTextEdit* b, QPlainTextEdit* c);
  void readToFile();
  ~OutputReader();

 private:
  QPlainTextEdit *a_, *b_, *c_;
  QFile *f_a, *f_b, *f_c;
};
class OutputWriter : private Core::Base::Files {
 public:
  OutputWriter(QPlainTextEdit* a, QPlainTextEdit* b, QPlainTextEdit* c);
  void writeFromFile();
  void writeFromFile(int);
  ~OutputWriter();

 private:
  QPlainTextEdit *a_, *b_, *c_;
  QFile *f_a, *f_b, *f_c;
};
}  // namespace IO
}  // namespace Core
#endif  // IO_HPP

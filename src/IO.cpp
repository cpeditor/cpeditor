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


#include <IO.hpp>

namespace Core {
namespace IO {

InputReader::InputReader(QPlainTextEdit* a,
                         QPlainTextEdit* b,
                         QPlainTextEdit* c) {
  a_ = a;
  b_ = b;
  c_ = c;

  f_a = new QFile(getInputFirst());
  f_b = new QFile(getInputSecond());
  f_c = new QFile(getInputThird());
}

InputReader::~InputReader() {
  if (f_a->isOpen())
    f_a->close();
  if (f_b->isOpen())
    f_b->close();
  if (f_c->isOpen())
    f_c->close();

  delete f_a;
  delete f_b;
  delete f_c;
}

void InputReader::readToFile() {
  if (!f_a->isOpen())
    f_a->open(QIODevice::ReadWrite | QFile::Text);
  if (!f_b->isOpen())
    f_b->open(QIODevice::ReadWrite | QFile::Text);
  if (!f_c->isOpen())
    f_c->open(QIODevice::ReadWrite | QFile::Text);

  f_a->resize(0);
  f_b->resize(0);
  f_c->resize(0);

  f_a->write(a_->toPlainText().toStdString().c_str());
  f_b->write(b_->toPlainText().toStdString().c_str());
  f_c->write(c_->toPlainText().toStdString().c_str());

  f_a->close();
  f_b->close();
  f_c->close();
}

OutputReader::OutputReader(QPlainTextEdit* a,
                           QPlainTextEdit* b,
                           QPlainTextEdit* c) {
  a_ = a;
  b_ = b;
  c_ = c;

  f_a = new QFile(getOutputFirst());
  f_b = new QFile(getOutputSecond());
  f_c = new QFile(getOutputThird());
}

OutputReader::~OutputReader() {
  if (f_a->isOpen())
    f_a->close();
  if (f_b->isOpen())
    f_b->close();
  if (f_c->isOpen())
    f_c->close();

  delete f_a;
  delete f_b;
  delete f_c;
}

void OutputReader::readToFile() {
  if (!f_a->isOpen())
    f_a->open(QIODevice::ReadWrite | QFile::Text);
  if (!f_b->isOpen())
    f_b->open(QIODevice::ReadWrite | QFile::Text);
  if (!f_c->isOpen())
    f_c->open(QIODevice::ReadWrite | QFile::Text);

  f_a->resize(0);
  f_b->resize(0);
  f_c->resize(0);

  f_a->write(a_->toPlainText().toStdString().c_str());
  f_b->write(b_->toPlainText().toStdString().c_str());
  f_c->write(c_->toPlainText().toStdString().c_str());

  f_a->close();
  f_b->close();
  f_c->close();
}

OutputWriter::OutputWriter(QPlainTextEdit* a,
                           QPlainTextEdit* b,
                           QPlainTextEdit* c) {
  a_ = a;
  b_ = b;
  c_ = c;

  f_a = new QFile(getOutputFirst());
  f_b = new QFile(getOutputSecond());
  f_c = new QFile(getOutputThird());
}

OutputWriter::~OutputWriter() {
  if (f_a->isOpen())
    f_a->close();
  if (f_b->isOpen())
    f_b->close();
  if (f_c->isOpen())
    f_c->close();

  delete f_a;
  delete f_b;
  delete f_c;
}

void OutputWriter::writeFromFile() {
  if (!f_a->isOpen())
    f_a->open(QIODevice::ReadWrite | QFile::Text);
  if (!f_b->isOpen())
    f_b->open(QIODevice::ReadWrite | QFile::Text);
  if (!f_c->isOpen())
    f_c->open(QIODevice::ReadWrite | QFile::Text);

  a_->setPlainText(f_a->readAll());
  b_->setPlainText(f_b->readAll());
  c_->setPlainText(f_c->readAll());

  f_a->resize(0);
  f_b->resize(0);
  f_c->resize(0);

  f_a->close();
  f_b->close();
  f_c->close();
}

void OutputWriter::writeFromFile(int n) {
  if (n == 1) {
    if (!f_a->isOpen())
      f_a->open(QIODevice::ReadWrite | QFile::Text);
  } else if (n == 2) {
    if (!f_b->isOpen())
      f_b->open(QIODevice::ReadWrite | QFile::Text);
  } else if (!f_c->isOpen())
    f_c->open(QIODevice::ReadWrite | QFile::Text);

  if (n == 1)
    a_->setPlainText(f_a->readAll());
  if (n == 2)
    b_->setPlainText(f_b->readAll());
  if (n == 3)
    c_->setPlainText(f_c->readAll());

  if (f_a->isOpen())
    f_a->close();
  if (f_b->isOpen())
    f_b->close();
  if (f_c->isOpen())
    f_c->close();
}

}  // namespace IO
}  // namespace Core

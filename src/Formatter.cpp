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


#include <Formatter.hpp>
#include <MessageLogger.hpp>

namespace Core {
Formatter::Formatter(QString runCommand) {
  command = runCommand;
  file = new QFile(getTempFormatFile());
  file->open(QIODevice::ReadWrite | QFile::Text);
  if (!file->isOpen()) {
    Log::MessageLogger::warn("Formatter",
                             "Cannot create temporary format file");
  } else {
    file->resize(0);
  }
}

Formatter::~Formatter() {
  if (file->isOpen())
    file->close();
  delete file;
}

bool Formatter::check(QString command) {
  auto results = command.trimmed().split(" ");
  auto runs = results[0] + " --version";
  return std::system(runs.toStdString().c_str()) == 0;
}

void Formatter::updateCommand(QString newCommand) {
  command = newCommand;
}

void Formatter::format(QCodeEditor* editor) {
  auto old_pos = editor->textCursor();
  auto old = editor->toPlainText().toStdString();
  if (file->isOpen()) {
    file->resize(0);
    file->write(old.c_str());
    file->close();
    std::string comm = (command + " " + getTempFormatFile()).toStdString();
    int status = std::system(comm.c_str());
    if (status != 0) {
      Log::MessageLogger::error(
          "Formatter", "Failed to format document at Formatter::format()");
      file->open(QIODevice::ReadWrite | QFile::Text);
      return;
    }
    file->open(QIODevice::ReadWrite | QFile::Text);
    editor->setPlainText(file->readAll());
    editor->setTextCursor(old_pos);
    Log::MessageLogger::info("Formatter", "Formatting completed");
  }
};
}  // namespace Core

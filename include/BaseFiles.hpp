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

namespace Core {
namespace Base {
class Files {
 public:
  Files() {
    base_ = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
  }
  QString getTempFormatFile() { return base_ + "/cp_editor_temp_format_file"; }
  QString getProgramFile(QString ext = "") {
    return base_ + "/sol" + (ext.isEmpty() ? ".cpp" : ext);
  }
  QString getInputFirst() { return base_ + "/cp_editor_temp_in1_file"; }
  QString getInputSecond() { return base_ + "/cp_editor_temp_in2_file"; }
  QString getInputThird() { return base_ + "/cp_editor_temp_in3_file"; }
  QString getOutputFirst() { return base_ + "/cp_editor_temp_out1_file"; }
  QString getOutputSecond() { return base_ + "/cp_editor_temp_out2_file"; }
  QString getOutputThird() { return base_ + "/cp_editor_temp_out3_file"; }
#ifndef _WIN32
  QString getBinaryOutput(QString ext = "") {
    return base_ + "/a" + (ext.isEmpty() ? ".out" : ext);
  }
#else
  QString getBinaryOutput() {
    return base_ + "/a" + (ext.isEmpty() ? ".exe" : ext);
  }
#endif
 private:
  QString base_;
};
}  // namespace Base

}  // namespace Core
#endif  // BASEFILES_HPP

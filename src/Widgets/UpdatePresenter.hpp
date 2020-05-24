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
#ifndef UPDATE_PRESENTER_DIALOG
#define UPDATE_PRESENTER_DIALOG

#include <QDialog>
#include <Telemetry/UpdateChecker.hpp>

class QLabel;
class QVBoxLayout;
class QHBoxLayout;
class QPushButton;
class QTextEdit;

namespace Widgets
{
class UpdatePresenter : public QDialog
{
    Q_OBJECT

  public:
    UpdatePresenter();
    void load(const Telemetry::UpdateChecker::UpdateMetaInformation &meta);

  private:
    QLabel *name = nullptr;
    QVBoxLayout *mainLayout = nullptr;
    QHBoxLayout *subLayout = nullptr;
    QTextEdit *textEdit = nullptr;
    QPushButton *downloadButton = nullptr;
    QPushButton *cancelButton = nullptr;

    Telemetry::UpdateChecker::UpdateMetaInformation information;
};
} // namespace Widgets
#endif
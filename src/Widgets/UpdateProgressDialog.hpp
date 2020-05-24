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

#ifndef UPDATE_DIALOG_PROGRESS
#define UPDATE_DIALOG_PROGRESS

#include <QDialog>
#include <Telemetry/UpdateChecker.hpp> // Forward declaration not possible as nested classes are required.

class QString;
class QProgressBar;
class QPushButton;
class QLabel;
class QVBoxLayout;

namespace Widgets
{
class UpdateProgressDialog : public QDialog
{
    Q_OBJECT
  public:
    UpdateProgressDialog();
    void start(bool beta = false);

  private slots:
    void updaterFailed(QString);
    void updaterFinished(Telemetry::UpdateChecker::UpdateMetaInformation);

  signals:
    void updateAvailable(Telemetry::UpdateChecker::UpdateMetaInformation);

  private:
    void resetState();

    QPushButton *cancelUpdate = nullptr;
    Telemetry::UpdateChecker *updateChecker = nullptr;
    QLabel *information = nullptr;
    QProgressBar *progressBar = nullptr;
    QVBoxLayout *mainLayout = nullptr;
};
} // namespace Widgets

#endif

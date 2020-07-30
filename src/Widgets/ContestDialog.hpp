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

#ifndef CONTEST_DIALOG_HPP
#define CONTEST_DIALOG_HPP

#include <QDialog>

class QGroupBox;
class QPushButton;
class QLineEdit;
class QToolButton;
class QHBoxLayout;
class QVBoxLayout;
class QFormLayout;
class QSpinBox;
class QComboBox;

namespace Widgets
{
class ContestDialog : public QDialog
{
    Q_OBJECT
  public:
    struct ContestData
    {
        int number;
        QString path;
        QString language;
    };

    explicit ContestDialog(QWidget *parent = nullptr);
    ContestData &contestData();
    ~ContestDialog();
  signals:
    void onContestCreated(ContestData data);

  private:
    void allocate();
    void setupUI();
    bool validate();
    void restrictWidgets();

    QString validationErrorMessage;

    ContestData _data;
    QGroupBox *groupBox = nullptr;
    QFormLayout *formLayout = nullptr;
    QSpinBox *problemCountSpinBox = nullptr;
    QComboBox *languageComboBox = nullptr;
    QLineEdit *contestNameLineEdit = nullptr;
    QLineEdit *pathLineEdit = nullptr;
    QToolButton *pathToolButton = nullptr;
    QHBoxLayout *pathItem = nullptr;

    QVBoxLayout *actionButtons = nullptr;
    QPushButton *reset = nullptr;
    QPushButton *create = nullptr;
    QPushButton *cancel = nullptr;

    QHBoxLayout *parentLayout = nullptr;
};
} // namespace Widgets

#endif

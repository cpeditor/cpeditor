/*
 * Copyright (C) 2019-2021 Ashar Khan <ashar786khan@gmail.com>
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

class QComboBox;
class QLabel;
class QLineEdit;
class QSpinBox;

namespace Widgets
{
class ContestDialog : public QDialog
{
    Q_OBJECT

  public:
    struct ContestData
    {
        QString path;
        int number;
        QString language;
    };

    explicit ContestDialog(QWidget *parent = nullptr);

    ContestData contestData() const;

  protected:
    void resizeEvent(QResizeEvent *event) override;

  private slots:
    void reset();

    void submit();

    void chooseDirectory();

    void updateContestPath();

  private:
    QString fullPath() const;

  private:
    QComboBox *languageComboBox = nullptr;
    QLineEdit *mainDirectoryEdit = nullptr;
    QLineEdit *subdirectoryEdit = nullptr;
    QLabel *contestPathLabel = nullptr;
    QSpinBox *problemCountSpinBox = nullptr;
};
} // namespace Widgets

#endif

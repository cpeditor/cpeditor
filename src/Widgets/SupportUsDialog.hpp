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

#ifndef SUPPORTUSDIALOG_HPP
#define SUPPORTUSDIALOG_HPP

#include <QDialog>

class SupportEntry : public QWidget
{
    Q_OBJECT

  public:
	explicit SupportEntry(const QString &text, const QString &icon, QString url, QWidget *parent = nullptr);

  signals:
    void clicked(const QString &url);

  private:
    QString url;
};

class SupportUsDialog : public QDialog
{
    Q_OBJECT

  public:
    explicit SupportUsDialog(QWidget *parent = nullptr);

  private slots:
    void onAnchorClicked(const QUrl &url);
};

#endif // SUPPORTUSDIALOG_HPP

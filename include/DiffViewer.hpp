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


#ifndef DIFFVIEWER_HPP
#define DIFFVIEWER_HPP

#include "MessageLogger.hpp"
#include <QMainWindow>
#include <QPlainTextEdit>

namespace Ui
{
class DiffViewer;
}

class DiffViewer : public QMainWindow
{
    Q_OBJECT

  public:
    explicit DiffViewer(QWidget *parent = nullptr);
    DiffViewer(QString *expected, QPlainTextEdit *ui, MessageLogger *log);
    void setTitle(QString title);
    ~DiffViewer();

  private slots:
    void on_exit_clicked();

    void on_read_clicked();

    void on_update_clicked();

  private:
    Ui::DiffViewer *ui;
    QString *exp;
    QPlainTextEdit *res;
    MessageLogger *log;
};

#endif // DIFFVIEWER_HPP

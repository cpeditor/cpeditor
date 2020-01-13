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
* Believe Software is "Software" and it isn't immune to bugs.
* 
*/


#ifndef EXPAND_HPP
#define EXPAND_HPP

#include "MessageLogger.hpp"
#include <QMainWindow>
#include <QPlainTextEdit>
#include <QTextBrowser>

namespace Ui
{
class Expand;
}

class Expand : public QMainWindow
{
    Q_OBJECT

  public:
    Expand(QPlainTextEdit *editor, MessageLogger *log, QWidget *parent = nullptr);
    Expand(QTextBrowser *browser, MessageLogger *log, QWidget *parent = nullptr);
    explicit Expand(QWidget *parent = nullptr);

    void setTitle(QString);
    void setReadFile(bool enabled);
    void setUpdate(bool enabled);

    QPlainTextEdit *getMainBody();
    ~Expand();

  private slots:
    void on_exit_clicked();

    void on_update_clicked();

    void on_read_clicked();

  private:
    Ui::Expand *ui;
    QPlainTextEdit *source = nullptr;
    QTextBrowser *current = nullptr;
    MessageLogger *log;
};

#endif // EXPAND_HPP

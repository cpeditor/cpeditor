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


#include "DiffViewer.hpp"

#include <MessageLogger.hpp>
#include <QFileDialog>

#include "../ui/ui_diffviewer.h"

DiffViewer::DiffViewer(QWidget *parent) : QMainWindow(parent), ui(new Ui::DiffViewer)
{
    ui->setupUi(this);
}

DiffViewer::DiffViewer(QString *expected, QPlainTextEdit *ui, MessageLogger *log) : DiffViewer()
{
    this->ui->expected->setPlainText(*expected);
    this->ui->resulted->setPlainText(ui->toPlainText());
    res = ui;
    this->log = log;
    exp = expected;
    this->ui->expected->setWordWrapMode(QTextOption::NoWrap);
    this->ui->resulted->setWordWrapMode(QTextOption::NoWrap);
}

void DiffViewer::setTitle(QString title)
{
    this->setWindowTitle(title);
}

DiffViewer::~DiffViewer()
{
    delete ui;
}

void DiffViewer::on_exit_clicked()
{
    this->close();
}

void DiffViewer::on_read_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Choose file", "", "*.*");
    if (filename.isEmpty())
        return;
    QFile *file = new QFile(filename);
    if (!file->open(QIODevice::ReadOnly | QFile::Text))
    {
        log->info("IO Operation", "Cannot open file for read.");
        delete file;
        return;
    }
    this->ui->expected->setPlainText(file->readAll());
    file->close();
    delete file;
}

void DiffViewer::on_update_clicked()
{
    exp->operator=(this->ui->expected->toPlainText());
    this->close();
}

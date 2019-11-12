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


#include "mainwindow.hpp"

#include <MessageLogger.hpp>
#include <QCXXHighlighter>
#include <QFileDialog>
#include <QMessageBox>
#include <QSyntaxStyle>
#include <iostream>

#include "../ui/ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setEditor();
  setLogger();
  setSettingsManager();
  restoreSettings();
}

MainWindow::~MainWindow() {
  delete ui;
  delete editor;
  delete setting;
  if (openFile != nullptr && openFile->isOpen())
    openFile->close();
  delete openFile;
}

void MainWindow::setEditor() {
  editor = new QCodeEditor();
  editor->setMinimumWidth(700);
  editor->setMinimumHeight(400);

  editor->setSyntaxStyle(QSyntaxStyle::defaultStyle());  // default is white
  editor->setHighlighter(new QCXXHighlighter());
  editor->setAutoIndentation(true);
  editor->setAutoParentheses(true);
  editor->setWordWrapMode(QTextOption::NoWrap);

  ui->verticalLayout_8->addWidget(editor);
}

void MainWindow::setLogger() {
  Log::MessageLogger::setContainer(ui->compiler_edit);
  ui->compiler_edit->setWordWrapMode(QTextOption::NoWrap);
  ui->compiler_edit->setReadOnly(true);
}

void MainWindow::setSettingsManager() {
  setting = new Settings::SettingManager();
}

void MainWindow::saveSettings() {
  setting->setDarkTheme(ui->actionDark_Theme->isChecked());
  setting->setWrapText(ui->actionWrap_Text->isChecked());
  setting->setAutoIndent(ui->actionAuto_Indentation->isChecked());
  setting->setAutoParenthesis(ui->actionAuto_Parenthesis->isChecked());

  // TODO (coder3101) : Add other two settins as well in this section.
}

void MainWindow::restoreSettings() {
  if (setting->isDarkTheme()) {
    ui->actionDark_Theme->setChecked(true);
    on_actionDark_Theme_triggered(true);
  }
  if (setting->isWrapText()) {
    ui->actionWrap_Text->setChecked(true);
    on_actionWrap_Text_triggered(true);
  }
  if (!setting->isAutoIndent()) {
    ui->actionAuto_Indentation->setChecked(false);
    on_actionAuto_Indentation_triggered(false);
  }
  if (!setting->isAutoParenthesis()) {
    ui->actionAuto_Parenthesis->setChecked(false);
    on_actionAuto_Parenthesis_triggered(false);
  }
}

void MainWindow::on_actionQuit_triggered() {
  saveSettings();
  auto res = QMessageBox::question(this, "Exit?",
                                   "Are you sure you want to exit the editor?",
                                   QMessageBox::Yes | QMessageBox::No);
  if (res == QMessageBox::Yes)
    QApplication::quit();
}

void MainWindow::on_actionDark_Theme_triggered(bool checked) {
  if (checked) {
    auto qFile = new QFile(":/styles/drakula.xml");
    qFile->open(QIODevice::ReadOnly);
    auto darkTheme = new QSyntaxStyle(this);
    darkTheme->load(qFile->readAll());
    editor->setSyntaxStyle(darkTheme);
  } else {
    editor->setSyntaxStyle(QSyntaxStyle::defaultStyle());
  }
}

void MainWindow::on_actionWrap_Text_triggered(bool checked) {
  if (checked)
    editor->setWordWrapMode(QTextOption::WordWrap);
  else
    editor->setWordWrapMode(QTextOption::NoWrap);
}

void MainWindow::on_actionAuto_Indentation_triggered(bool checked) {
  if (checked)
    editor->setAutoIndentation(true);
  else
    editor->setAutoIndentation(false);
}

void MainWindow::on_actionAuto_Parenthesis_triggered(bool checked) {
  if (checked)
    editor->setAutoParentheses(true);
  else
    editor->setAutoParentheses(false);
}

void MainWindow::on_actionOpen_triggered() {
  auto fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
                                               "C++ Files (*.cpp *.h)");
  if (fileName.isEmpty())
    return;
  QFile* newFile = new QFile(fileName);
  newFile->open(QFile::Text | QIODevice::ReadWrite);

  if (editor->toPlainText().size() != 0) {
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Overwrite?", "Opening a new file will overwrite this file?",
        QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No)
      return;
  }
  if (newFile->isOpen()) {
    editor->setPlainText(newFile->readAll());
    if (openFile != nullptr && openFile->isOpen())
      openFile->close();
    openFile = newFile;
    Log::MessageLogger::info("Open",
                             "Opened " + openFile->fileName().toStdString());
  } else {
    Log::MessageLogger::error(
        "Open", "Cannot Open, Do I have read and write permissions?");
  }
}

void MainWindow::on_actionSave_triggered() {
  if (openFile == nullptr) {
    auto filename = QFileDialog::getSaveFileName(this, tr("Save File"), "",
                                                 "C++ Files (*.cpp *.h)");
    if (filename.isEmpty())
      return;
    auto savedFile = new QFile(filename);
    savedFile->open(QIODevice::ReadWrite | QFile::Text);
    if (savedFile->isOpen()) {
      openFile = savedFile;
      openFile->write(editor->toPlainText().toStdString().c_str());
    } else {
      Log::MessageLogger::error(
          "Save", "Cannot Save file. Do I have write permission?");
    }
  } else {
    openFile->resize(0);
    openFile->write(editor->toPlainText().toStdString().c_str());
    Log::MessageLogger::info(
        "Save", "Saved with file name " + openFile->fileName().toStdString());
  }
}

void MainWindow::on_actionSave_as_triggered() {
  auto filename = QFileDialog::getSaveFileName(this, tr("Save File"), "",
                                               "C++ Files (*.cpp *.h)");
  if (filename.isEmpty())
    return;
  QFile savedFile(filename);
  savedFile.open(QIODevice::ReadWrite | QFile::Text);
  if (savedFile.isOpen()) {
    savedFile.write(editor->toPlainText().toStdString().c_str());
    Log::MessageLogger::info(
        "Save as", "Saved new file name " + savedFile.fileName().toStdString());
  } else {
    Log::MessageLogger::error(
        "Save as",
        "Cannot Save as new file, Is write permission allowed to me?");
  }
}

void MainWindow::on_actionAbout_triggered() {
  QMessageBox::about(
      this, tr("About CP Editor"),
      tr("<p>The <b>CP Editor</b> is a competitive programmer's editor "
         "which can ease the task of compiling, testing and running a program"
         "so that you (a great programmer) can focus fully on your algorithms "
         "designs. </p>"));
}

void MainWindow::on_actionAbout_Qt_triggered() {
  QApplication::aboutQt();
}

void MainWindow::closeEvent(QCloseEvent* event) {
  saveSettings();
  auto res = QMessageBox::question(this, "Exit?",
                                   "Are you sure you want to exit the"
                                   " editor?",
                                   QMessageBox::Yes | QMessageBox::No);
  if (res == QMessageBox::Yes)
    event->accept();
  else
    event->ignore();
}

void MainWindow::on_actionFormat_triggered() {}

void MainWindow::on_actionRun_triggered() {}

void MainWindow::on_actionCompile_triggered() {}

void MainWindow::on_compile_clicked() {
  on_actionCompile_triggered();
}

void MainWindow::on_run_clicked() {
  on_actionRun_triggered();
}

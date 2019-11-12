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


#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QCodeEditor>
#include <QFile>
#include <QMainWindow>
#include <SettingsManager.hpp>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow() override;
  void closeEvent(QCloseEvent* event) override;
 private slots:
  void on_actionQuit_triggered();

  void on_actionDark_Theme_triggered(bool checked);

  void on_actionWrap_Text_triggered(bool checked);

  void on_actionAuto_Indentation_triggered(bool checked);

  void on_actionAuto_Parenthesis_triggered(bool checked);

  void on_actionOpen_triggered();

  void on_actionSave_triggered();

  void on_actionSave_as_triggered();

  void on_actionAbout_triggered();

  void on_actionAbout_Qt_triggered();

  void on_actionFormat_triggered();

  void on_actionRun_triggered();

  void on_actionCompile_triggered();

  void on_compile_clicked();

  void on_run_clicked();

 private:
  Ui::MainWindow* ui;
  QCodeEditor* editor;
  QFile* openFile = nullptr;
  Settings::SettingManager* setting = nullptr;
  void setEditor();
  void setLogger();
  void setSettingsManager();
  void saveSettings();
  void restoreSettings();
};
#endif  // MAINWINDOW_HPP

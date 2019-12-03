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

#include <Core.hpp>
#include <Formatter.hpp>
#include <IO.hpp>
#include <QCodeEditor>
#include <QFile>
#include <QMainWindow>
#include <Runner.hpp>
#include <SettingsManager.hpp>
#include <UpdateNotifier.hpp>
#include <generated/version.hpp>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QString fileOpen, QWidget* parent = nullptr);
  ~MainWindow() override;
  void closeEvent(QCloseEvent* event) override;
 private slots:
  void on_actionQuit_triggered();

  void on_actionDark_Theme_triggered(bool checked);
  void on_actionWrap_Text_triggered(bool checked);
  void on_actionAuto_Indentation_triggered(bool checked);
  void on_actionAuto_Parenthesis_triggered(bool checked);

  void on_actionNew_triggered();
  void on_actionOpen_triggered();
  void on_actionSave_triggered();
  void on_actionSave_as_triggered();
  void on_actionAbout_triggered();
  void on_actionAbout_Qt_triggered();
  void on_actionFormat_triggered();
  void on_actionRun_triggered();
  void on_actionCompile_triggered();
  void on_actionChange_compile_command_triggered();
  void on_actionChange_run_command_triggered();
  void on_actionChange_format_command_triggered();
  void on_actionSet_Code_Template_triggered();

  void on_onlyRun_triggered();
  void on_compile_clicked();
  void on_run_clicked();
  void on_actionReset_Settings_triggered();
  void on_expected_clicked(bool checked);
  void on_runOnly_clicked();

  void firstExecutionFinished(QString, QString);
  void secondExecutionFinished(QString, QString);
  void thirdExecutionFinished(QString, QString);
  void onSaveTimerElapsed();

  void on_actionC_C_triggered(bool checked);
  void on_actionPython_triggered(bool checked);
  void on_actionJava_triggered(bool checked);

  void on_actionRun_Command_triggered();
  void on_actionKill_Processes_triggered();
  void on_actionFont_triggered();
  void on_actionAuto_Save_triggered(bool checked);
  void on_actionBeta_Updates_triggered(bool checked);
  void on_actionDetached_Execution_triggered();

 private:
  Ui::MainWindow* ui;
  QCodeEditor* editor;
  QString language;
  QFile* openFile = nullptr;
  Settings::SettingManager* setting = nullptr;
  Core::Formatter* formatter = nullptr;
  Core::IO::InputReader* inputReader = nullptr;
  Core::IO::OutputReader* outputReader = nullptr;
  Core::IO::OutputWriter* outputWriter = nullptr;
  Core::Compiler* compiler = nullptr;
  Core::Runner* runner = nullptr;
  QTimer* saveTimer = nullptr;
  Telemetry::UpdateNotifier* updater = nullptr;
  void setEditor();
  void setLogger();
  void runEditorDiagonistics();
  void setSettingsManager();
  void saveSettings();
  void restoreSettings();
  void setupCore();
  void launchSession();
  void checkUpdates();
};
#endif  // MAINWINDOW_HPP

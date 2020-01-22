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

#ifndef APPWINDOW_HPP
#define APPWINDOW_HPP

#include <QMainWindow>

#include "SettingsManager.hpp"
#include "UpdateNotifier.hpp"
#include "mainwindow.hpp"
#include "preferencewindow.hpp"

namespace Ui
{
class AppWindow;
}

class AppWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit AppWindow(QStringList args, QWidget *parent = nullptr);
    ~AppWindow() override;

    void closeEvent(QCloseEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;

  private slots:
    void on_actionSupport_me_triggered();

    void on_actionAbout_triggered();

    void on_actionAutosave_triggered(bool checked);

    void on_actionQuit_triggered();

    void on_actionNew_Tab_triggered();

    void on_actionOpen_triggered();

    void on_actionRestore_Settings_triggered();

    void on_actionSettings_triggered();

    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_actionSave_All_triggered();

    void on_actionClose_Current_triggered();

    void on_actionClose_All_triggered();

    void on_actionClose_Saved_triggered();

    void on_actionCheck_for_updates_triggered();

    void onEditorChanged();

    void applyEditorChanged();

    void onTabCloseRequested(int);

    void onTabChanged(int);

    void onSaveTimerElapsed();

    void onSettingsApplied();

    void onSplitterMoved(int, int);

    void onIncomingCompanionRequest(Network::CompanionData);

    void onViewModeToggle();

    void on_actionCompile_triggered();

    void on_actionCompile_Run_triggered();

    void on_actionRun_triggered();

    void on_actionFormat_code_triggered();

    void on_actionRun_Detached_triggered();

    void on_actionKill_Processes_triggered();

    void on_actionUse_Snippets_triggered();

    void on_actionEditor_Mode_triggered();

    void on_actionIO_Mode_triggered();

    void on_actionSplit_Mode_triggered();

    void on_confirmTriggered(MainWindow *widget);

    void onTabContextMenuRequested(const QPoint &pos);

  private:
    Ui::AppWindow *ui;
    MessageLogger *activeLogger = nullptr;
    QTimer *timer = nullptr;
    QMetaObject::Connection activeSplitterMoveConnections;
    QMetaObject::Connection companionEditorConnections;
    Settings::SettingManager *settingManager = nullptr;
    Telemetry::UpdateNotifier *updater = nullptr;
    PreferenceWindow *preferenceWindow = nullptr;
    Network::CompanionServer *server;
    QTimer *editorChangeApply = nullptr;

    void setConnections();
    void allocate();
    void applySettings();
    void saveSettings();
    bool diagonistics;
    QVector<QShortcut *> hotkeyObjects;
    void maybeSetHotkeys();
    bool closeTab(int index);
    void openTab(QString path, bool isCompanionTab = false);
    bool quit();

    MainWindow *currentWindow();
    MainWindow *windowIndex(int index);
};

#endif // APPWINDOW_HPP

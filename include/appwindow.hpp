/*
 * Copyright (C) 2019-2020 Ashar Khan <ashar786khan@gmail.com>
 *
 * This file is part of cpeditor.
 *
 * cpeditor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * I will not be responsible if cpeditor behaves in unexpected way and
 * causes your ratings to go down and or lose any important contest.
 *
 * Believe Software is "Software" and it isn't immune to bugs.
 *
 */

#ifndef APPWINDOW_HPP
#define APPWINDOW_HPP

#include <QMainWindow>

#include "Core/SettingsManager.hpp"
#include "Telemetry/UpdateNotifier.hpp"
#include "mainwindow.hpp"
#include "preferencewindow.hpp"
#include <QSystemTrayIcon>
#include <findreplacedialog.h>

namespace Ui
{
class AppWindow;
}

class AppWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit AppWindow(bool noHotExit, QWidget *parent = nullptr);
    explicit AppWindow(int depth, bool cpp, bool java, bool python, bool noHotExit, const QStringList &paths,
                       QWidget *parent = nullptr);
    explicit AppWindow(bool cpp, bool java, bool python, bool noHotExit, int number, const QString &path,
                       QWidget *parent = nullptr);
    ~AppWindow() override;

    void closeEvent(QCloseEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;

  public slots:
    void onReceivedMessage(quint32 instanceId, QByteArray message);

  private slots:
    void on_actionSupport_me_triggered();

    void on_actionAbout_triggered();

    void on_actionAboutQt_triggered();

    void on_actionBuildInfo_triggered();

    void on_actionAutosave_triggered(bool checked);

    void on_actionQuit_triggered();

    void on_actionNew_Tab_triggered();

    void on_actionOpen_triggered();

    void on_actionOpenContest_triggered();

    void on_actionRestore_Settings_triggered();

    void on_actionSettings_triggered();

    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_actionSave_All_triggered();

    void on_actionClose_Current_triggered();

    void on_actionClose_All_triggered();

    void on_actionClose_Saved_triggered();

    void on_actionCheck_for_updates_triggered();

    void onEditorFileChanged();

    void onEditorTextChanged(MainWindow *window);

    void onTabCloseRequested(int);

    void onTabChanged(int);

    void onSaveTimerElapsed();

    void onSettingsApplied();

    void onSplitterMoved(int, int);

    void onRightSplitterMoved(int, int);

    void onIncomingCompanionRequest(const Network::CompanionData &);

    void onViewModeToggle();

    void on_actionCompile_triggered();

    void on_actionCompile_Run_triggered();

    void on_actionRun_triggered();

    void on_action_find_replace_triggered();

    void on_actionFormat_code_triggered();

    void on_actionRun_Detached_triggered();

    void on_actionKill_Processes_triggered();

    void on_actionUse_Snippets_triggered();

    void on_actionEditor_Mode_triggered();

    void on_actionIO_Mode_triggered();

    void on_actionSplit_Mode_triggered();

    void on_action_indent_triggered();

    void on_action_unindent_triggered();

    void on_action_swap_line_up_triggered();

    void on_action_swap_line_down_triggered();

    void on_action_delete_line_triggered();

    void on_action_toggle_comment_triggered();

    void on_action_toggle_block_comment_triggered();

    void on_confirmTriggered(MainWindow *widget);

    void onTabContextMenuRequested(const QPoint &pos);

    void on_actionShow_Logs_triggered();

    void on_actionClear_Logs_triggered();

    void showOnTop();

    void onTrayIconActivated(QSystemTrayIcon::ActivationReason reason);

  private:
    Ui::AppWindow *ui;
    MessageLogger *activeLogger = nullptr;
    QTimer *timer = nullptr;
    QMetaObject::Connection activeSplitterMoveConnection;
    QMetaObject::Connection activeRightSplitterMoveConnection;
    Telemetry::UpdateNotifier *updater = nullptr;
    PreferenceWindow *preferenceWindow = nullptr;
    Network::CompanionServer *server;
    FindReplaceDialog *findReplaceDialog = nullptr;
    QSystemTrayIcon *trayIcon = nullptr;
    QMenu *trayIconMenu = nullptr;

    void setConnections();
    void allocate();
    void applySettings();
    void saveSettings();
    QVector<QShortcut *> hotkeyObjects;
    void maybeSetHotkeys();
    bool closeTab(int index);
    void openTab(const QString &path);
    void openTabs(const QStringList &paths);
    void openPaths(const QStringList &paths, bool cpp = true, bool java = true, bool python = true, int depth = -1);
    QStringList openFolder(const QString &path, bool cpp, bool java, bool python, int depth);
    void openContest(const QString &path, const QString &lang, int number);
    void saveEditorStatus(bool loadFromFile);
    bool quit();
    int getNewUntitledIndex();

    MainWindow *currentWindow();
    MainWindow *windowAt(int index);
};

#endif // APPWINDOW_HPP

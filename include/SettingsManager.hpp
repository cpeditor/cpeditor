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

#ifndef SETTINGSMANAGER_HPP
#define SETTINGSMANAGER_HPP

#include <QKeySequence>
#include <QRect>
#include <QSettings>
#include <QString>
#include <string>

#define SETTINGS_FILE "cp_editor_settings.ini"

namespace Settings
{

enum ViewMode{
    FULL_EDITOR,
    FULL_IO,
    SPLIT
};

struct SettingsData
{
    int companionPort;
    int tabStop;
    QRect geometry;
    QString font;
    QString defaultLanguage;

    QString templateCpp;
    QString templatePython;
    QString templateJava;

    QString runtimeArgumentsCpp;
    QString runtimeArgumentsJava;
    QString runtimeArgumentsPython;

    QString formatCommand;

    QString compileCommandJava;
    QString compileCommandCpp;

    QString runCommandJava;
    QString runCommandPython;

    QString editorTheme;

    bool isSystemThemeDark;
    bool isHotKeyInUse;
    bool isAutoParenthesis;
    bool isAutoIndent;
    bool isAutoSave;
    bool isWrapText;
    bool isBeta;
    bool isTabsBeingUsed;
    bool shouldSaveTests;
    bool isCompanionActive;
    bool isWindowMaximized;
    bool isCheckUpdateOnStartup;
    bool isUpdateCheckOnStartup;

    QKeySequence hotkeyRun;
    QKeySequence hotkeyCompile;
    QKeySequence hotkeyCompileRun;
    QKeySequence hotkeyFormat;
    QKeySequence hotkeyKill;
    QKeySequence hotkeyViewModeToggler;

    ViewMode viewMode;
};

class SettingManager
{
  public:
    SettingManager();

    int getConnectionPort();
    void setConnectionPort(int port);

    int getTabStop();
    void setTabStop(int num);

    QRect getGeometry();
    void setGeometry(QRect);

    QString getFont();
    void setFont(QString font);

    QString getDefaultLang();
    void setDefaultLanguage(QString lang);

    QString getTemplatePathCpp();
    QString getTemplatePathJava();
    QString getTemplatePathPython();
    void setTemplatePathCpp(QString path);
    void setTemplatePathJava(QString path);
    void setTemplatePathPython(QString path);

    QString getRuntimeArgumentsCpp();
    QString getRuntimeArgumentsJava();
    QString getRuntimeArgumentsPython();
    void setRuntimeArgumentsCpp(QString command);
    void setRuntimeArgumentsJava(QString command);
    void setRuntimeArgumentsPython(QString command);

    QString getFormatCommand();
    void setFormatCommand(QString command);

    QString getCompileCommandCpp();
    QString getCompileCommandJava();
    void setCompileCommandsCpp(QString command);
    void setCompileCommandsJava(QString command);

    QString getRunCommandJava();
    QString getRunCommandPython();
    void setRunCommandJava(QString command);
    void setRunCommandPython(QString command);

    QString getEditorTheme();
    void setEditorTheme(QString theme);

    bool isSystemThemeDark();
    void setSystemThemeDark(bool value);

    bool isHotkeyInUse();
    void setHotKeyInUse(bool value);

    bool isAutoParenthesis();
    void setAutoParenthesis(bool value);

    bool isAutoIndent();
    void setAutoIndent(bool value);

    bool isAutoSave();
    void setAutoSave(bool value);

    bool isWrapText();
    void setWrapText(bool value);

    bool isBeta();
    void setBeta(bool value);

    bool isTabs();
    void setTabs(bool value);

    bool isSaveTests();
    void setSaveTests(bool value);

    bool isCompetitiveCompanionActive();
    void setCompetitiveCompanionActive(bool value);

    bool isMaximizedWindow();
    void setMaximizedWindow(bool value);

    bool isCheckUpdateOnStartup();
    void checkUpdateOnStartup(bool value);

    ViewMode getViewMode();
    void setViewMode(ViewMode v);

    SettingsData toData();
    ~SettingManager();

    void setHotkeyFormat(QKeySequence sequence);
    void setHotkeyKill(QKeySequence sequence);
    void setHotkeyCompileRun(QKeySequence sequence);
    void setHotkeyRun(QKeySequence sequence);
    void setHotkeyCompile(QKeySequence sequence);
    void setHotkeyViewModeToggler(QKeySequence sequence);
    QKeySequence getHotkeyFormat();
    QKeySequence getHotkeyKill();
    QKeySequence getHotkeyCompileRun();
    QKeySequence getHotkeyRun();
    QKeySequence getHotkeyCompile();
    QKeySequence getHotkeyViewModeToggler();


  private:
    QString mSettingsFile;
    QSettings *mSettings;
};

} // namespace Settings

#endif // SETTINGSMANAGER_HPP

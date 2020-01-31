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
 * I will not be responsible if CPEditor behaves in an unexpected way and
 * causes your bad performance and / or lose any contest.
 *
 * Believe Software is "Software" and it isn't immune to bugs.
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

enum ViewMode
{
    FULL_EDITOR,
    FULL_IO,
    SPLIT
};

struct SettingsData
{
    int companionPort;
    int tabStop;
    int timeLimit;

    QRect geometry;
    QString font;
    QString defaultLanguage;

    QString templateCpp;
    QString templatePython;
    QString templateJava;

    QString runtimeArgumentsCpp;
    QString runtimeArgumentsJava;
    QString runtimeArgumentsPython;

    QString clangFormatBinary;
    QString clangFormatStyle;

    QString compileCommandJava;
    QString compileCommandCpp;

    QString runCommandJava;
    QString runCommandPython;

    QString editorTheme;

    bool isHotKeyInUse;
    bool isAutoParenthesis;
    bool isAutoIndent;
    bool isAutoSave;
    bool isWrapText;
    bool isBeta;
    bool isTabsReplaced;
    bool shouldSaveTests;
    bool isCompanionActive;
    bool isWindowMaximized;
    bool isCheckUpdateOnStartup;
    bool isUpdateCheckOnStartup;
    bool isFormatOnSave;

    QKeySequence hotkeyRun;
    QKeySequence hotkeyCompile;
    QKeySequence hotkeyCompileRun;
    QKeySequence hotkeyFormat;
    QKeySequence hotkeyKill;
    QKeySequence hotkeyViewModeToggler;
    QKeySequence hotkeySnippets;

    ViewMode viewMode;
    QByteArray splitterSizes;

    QString cfPath;
};

class SettingManager
{
  public:
    SettingManager();

    int getConnectionPort();
    void setConnectionPort(const int& port);

    int getTabStop();
    void setTabStop(const int& num);

    int getTimeLimit();
    void setTimeLimit(const int& ms);

    QRect getGeometry();
    void setGeometry(const QRect& rect);

    QString getFont();
    void setFont(const QString& font);

    QString getDefaultLang();
    void setDefaultLanguage(const QString& lang);

    QString getTemplatePathCpp();
    QString getTemplatePathJava();
    QString getTemplatePathPython();
    void setTemplatePathCpp(const QString& path);
    void setTemplatePathJava(const QString& path);
    void setTemplatePathPython(const QString& path);

    QString getRuntimeArgumentsCpp();
    QString getRuntimeArgumentsJava();
    QString getRuntimeArgumentsPython();
    void setRuntimeArgumentsCpp(const QString& command);
    void setRuntimeArgumentsJava(const QString& command);
    void setRuntimeArgumentsPython(const QString& command);

    QString getClangFormatBinary();
    void setClangFormatBinary(const QString& binary);
    QString getClangFormatStyle();
    void setClangFormatStyle(const QString& style);

    QString getCompileCommandCpp();
    QString getCompileCommandJava();
    void setCompileCommandsCpp(const QString& command);
    void setCompileCommandsJava(const QString& command);

    QString getRunCommandJava();
    QString getRunCommandPython();
    void setRunCommandJava(const QString& command);
    void setRunCommandPython(const QString& command);

    QString getEditorTheme();
    void setEditorTheme(const QString& theme);

    bool isHotkeyInUse();
    void setHotKeyInUse(const bool& value);

    bool isAutoParenthesis();
    void setAutoParenthesis(const bool& value);

    bool isAutoIndent();
    void setAutoIndent(const bool& value);

    bool isAutoSave();
    void setAutoSave(const bool& value);

    bool isWrapText();
    void setWrapText(const bool& value);

    bool isBeta();
    void setBeta(const bool& value);

    bool isTabsReplaced();
    void setTabsReplaced(const bool& value);

    bool isSaveTests();
    void setSaveTests(const bool& value);

    bool isUseHotExit();
    void setUseHotExit(const bool& value);

    bool isCompetitiveCompanionActive();
    void setCompetitiveCompanionActive(const bool& value);

    bool isCompetitiveCompanionOpenNewTab();
    void setCompetitiveCompanionOpenNewTab(const bool& value);

    bool isMaximizedWindow();
    void setMaximizedWindow(const bool& value);

    bool isCheckUpdateOnStartup();
    void checkUpdateOnStartup(const bool& value);

    bool isFormatOnSave();
    void formatOnSave(const bool& value);

    int getTransparency();
    void setTransparency(const int& val);

    ViewMode getViewMode();
    void setViewMode(const ViewMode& v);

    QByteArray getSplitterSizes();
    void setSplitterSizes(const QByteArray& state);

    QByteArray getRightSplitterSizes();
    void setRightSplitterSizes(const QByteArray& state);

    void setHotkeyFormat(const QKeySequence& sequence);
    void setHotkeyKill(const QKeySequence& sequence);
    void setHotkeyCompileRun(const QKeySequence& sequence);
    void setHotkeyRun(const QKeySequence& sequence);
    void setHotkeyCompile(const QKeySequence& sequence);
    void setHotkeyViewModeToggler(const QKeySequence& sequence);
    void setHotkeySnippets(const QKeySequence& sequence);
    QKeySequence getHotkeyFormat();
    QKeySequence getHotkeyKill();
    QKeySequence getHotkeyCompileRun();
    QKeySequence getHotkeyRun();
    QKeySequence getHotkeyCompile();
    QKeySequence getHotkeyViewModeToggler();
    QKeySequence getHotkeySnippets();

    QString getSnippet(const QString& lang, const QString& name);
    void setSnippet(const QString& lang, const QString& name, const QString& content);
    void removeSnippet(const QString& lang, const QString& name);
    QStringList getSnippetsNames(const QString& lang);

    int getNumberOfTabs();
    void setNumberOfTabs(const int& value);
    int getCurrentIndex();
    void setCurrentIndex(const int& index);
    void clearEditorStatus();
    QMap<QString, QVariant> getEditorStatus(const int& index);
    void setEditorStatus(const int& index, const QMap<QString, QVariant>& status);

    QString getCFPath();
    void setCFPath(const QString& path);

    void resetSettings();

    SettingsData toData();
    ~SettingManager();

  private:
    QString mSettingsFile;
    QSettings *mSettings;
};

} // namespace Settings

#endif // SETTINGSMANAGER_HPP

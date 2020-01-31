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

    inline int getConnectionPort();
    inline void setConnectionPort(const int& port);

    inline int getTabStop();
    inline void setTabStop(const int& num);

    inline int getTimeLimit();
    inline void setTimeLimit(const int& ms);

    inline QRect getGeometry();
    inline void setGeometry(const QRect& rect);

    inline QString getFont();
    inline void setFont(const QString& font);

    inline QString getDefaultLang();
    inline void setDefaultLanguage(const QString& lang);

    inline QString getTemplatePathCpp();
    inline QString getTemplatePathJava();
    inline QString getTemplatePathPython();
    inline void setTemplatePathCpp(const QString& path);
    inline void setTemplatePathJava(const QString& path);
    inline void setTemplatePathPython(const QString& path);

    inline QString getRuntimeArgumentsCpp();
    inline QString getRuntimeArgumentsJava();
    inline QString getRuntimeArgumentsPython();
    inline void setRuntimeArgumentsCpp(const QString& command);
    inline void setRuntimeArgumentsJava(const QString& command);
    inline void setRuntimeArgumentsPython(const QString& command);

    inline QString getClangFormatBinary();
    inline void setClangFormatBinary(const QString& binary);
    inline QString getClangFormatStyle();
    inline void setClangFormatStyle(const QString& style);

    inline QString getCompileCommandCpp();
    inline QString getCompileCommandJava();
    inline void setCompileCommandsCpp(const QString& command);
    inline void setCompileCommandsJava(const QString& command);

    inline QString getRunCommandJava();
    inline QString getRunCommandPython();
    inline void setRunCommandJava(const QString& command);
    inline void setRunCommandPython(const QString& command);

    inline QString getEditorTheme();
    inline void setEditorTheme(const QString& theme);

    inline bool isHotkeyInUse();
    inline void setHotKeyInUse(const bool& value);

    inline bool isAutoParenthesis();
    inline void setAutoParenthesis(const bool& value);

    inline bool isAutoIndent();
    inline void setAutoIndent(const bool& value);

    inline bool isAutoSave();
    inline void setAutoSave(const bool& value);

    inline bool isWrapText();
    inline void setWrapText(const bool& value);

    inline bool isBeta();
    inline void setBeta(const bool& value);

    inline bool isTabsReplaced();
    inline void setTabsReplaced(const bool& value);

    inline bool isSaveTests();
    inline void setSaveTests(const bool& value);

    inline bool isUseHotExit();
    inline void setUseHotExit(const bool& value);

    inline bool isCompetitiveCompanionActive();
    inline void setCompetitiveCompanionActive(const bool& value);

    inline bool isCompetitiveCompanionOpenNewTab();
    inline void setCompetitiveCompanionOpenNewTab(const bool& value);

    inline bool isMaximizedWindow();
    inline void setMaximizedWindow(const bool& value);

    inline bool isCheckUpdateOnStartup();
    inline void checkUpdateOnStartup(const bool& value);

    inline bool isFormatOnSave();
    inline void formatOnSave(const bool& value);

    inline int getTransparency();
    inline void setTransparency(const int& val);

    inline ViewMode getViewMode();
    inline void setViewMode(const ViewMode& v);

    inline QByteArray getSplitterSizes();
    inline void setSplitterSizes(const QByteArray& state);

    inline QByteArray getRightSplitterSizes();
    inline void setRightSplitterSizes(const QByteArray& state);

    inline void setHotkeyFormat(const QKeySequence& sequence);
    inline void setHotkeyKill(const QKeySequence& sequence);
    inline void setHotkeyCompileRun(const QKeySequence& sequence);
    inline void setHotkeyRun(const QKeySequence& sequence);
    inline void setHotkeyCompile(const QKeySequence& sequence);
    inline void setHotkeyViewModeToggler(const QKeySequence& sequence);
    inline void setHotkeySnippets(const QKeySequence& sequence);
    inline QKeySequence getHotkeyFormat();
    inline QKeySequence getHotkeyKill();
    inline QKeySequence getHotkeyCompileRun();
    inline QKeySequence getHotkeyRun();
    inline QKeySequence getHotkeyCompile();
    inline QKeySequence getHotkeyViewModeToggler();
    inline QKeySequence getHotkeySnippets();

    inline QString getSnippet(const QString& lang, const QString& name);
    inline void setSnippet(const QString& lang, const QString& name, const QString& content);
    inline void removeSnippet(const QString& lang, const QString& name);
    inline QStringList getSnippetsNames(const QString& lang);

    inline int getNumberOfTabs();
    inline void setNumberOfTabs(const int& value);
    inline int getCurrentIndex();
    inline void setCurrentIndex(const int& index);
    inline void clearEditorStatus();
    inline QMap<QString, QVariant> getEditorStatus(const int& index);
    inline void setEditorStatus(const int& index, const QMap<QString, QVariant>& status);

    inline QString getCFPath();
    inline void setCFPath(const QString& path);

    inline void resetSettings();

    inline SettingsData toData();
    ~SettingManager();

  private:
    QString mSettingsFile;
    QSettings *mSettings;
};

} // namespace Settings

#endif // SETTINGSMANAGER_HPP

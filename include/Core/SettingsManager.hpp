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

class SettingManager
{
  public:
    SettingManager();

    int getConnectionPort();
    void setConnectionPort(int port);

    int getTabStop();
    void setTabStop(int num);

    int getTimeLimit();
    void setTimeLimit(int ms);

    QRect getGeometry();
    void setGeometry(const QRect &);

    QString getFont();
    void setFont(const QString &font);

    QString getDefaultLanguage();
    void setDefaultLanguage(const QString &lang);

    QString getTemplatePath(const QString &lang);
    void setTemplatePath(const QString &lang, const QString &path);

    QString getRuntimeArguments(const QString &lang);
    void setRuntimeArguments(const QString &lang, const QString &command);

    QString getClangFormatBinary();
    void setClangFormatBinary(const QString &binary);
    QString getClangFormatStyle();
    void setClangFormatStyle(const QString &style);

    QString getCompileCommand(const QString &lang);
    void setCompileCommand(const QString &lang, const QString &command);

    QString getRunCommand(const QString &lang);
    void setRunCommand(const QString &lang, const QString &command);

    QString getEditorTheme();
    void setEditorTheme(const QString &theme);

    bool isHotkeyInUse();
    void setHotKeyInUse(bool value);

    bool isAutoParentheses();
    void setAutoParentheses(bool value);

    bool isAutoRemoveParentheses();
    void setAutoRemoveParentheses(bool value);

    bool isAutoIndent();
    void setAutoIndent(bool value);

    bool isAutoSave();
    void setAutoSave(bool value);

    bool isWrapText();
    void setWrapText(bool value);

    bool isBeta();
    void setBeta(bool value);

    bool isTabsReplaced();
    void setTabsReplaced(bool value);

    bool isSaveTests();
    void setSaveTests(bool value);

    bool isUseHotExit();
    void setUseHotExit(bool value);

    bool isCompetitiveCompanionActive();
    void setCompetitiveCompanionActive(bool value);

    bool isCompetitiveCompanionOpenNewTab();
    void setCompetitiveCompanionOpenNewTab(bool value);

    bool isMaximizedWindow();
    void setMaximizedWindow(bool value);

    bool isCheckUpdateOnStartup();
    void checkUpdateOnStartup(bool value);

    bool isFormatOnSave();
    void formatOnSave(bool value);

    int getTransparency();
    void setTransparency(int val);

    ViewMode getViewMode();
    void setViewMode(const ViewMode &v);

    QByteArray getSplitterSizes();
    void setSplitterSizes(const QByteArray &state);

    QByteArray getRightSplitterSizes();
    void setRightSplitterSizes(const QByteArray &state);

    void setHotkeyFormat(const QKeySequence &sequence);
    void setHotkeyKill(const QKeySequence &sequence);
    void setHotkeyCompileRun(const QKeySequence &sequence);
    void setHotkeyRun(const QKeySequence &sequence);
    void setHotkeyCompile(const QKeySequence &sequence);
    void setHotkeyViewModeToggler(const QKeySequence &sequence);
    void setHotkeySnippets(const QKeySequence &sequence);
    QKeySequence getHotkeyFormat();
    QKeySequence getHotkeyKill();
    QKeySequence getHotkeyCompileRun();
    QKeySequence getHotkeyRun();
    QKeySequence getHotkeyCompile();
    QKeySequence getHotkeyViewModeToggler();
    QKeySequence getHotkeySnippets();

    QString getSnippet(const QString &lang, const QString &name);
    void setSnippet(const QString &lang, const QString &name, const QString &content);
    void removeSnippet(const QString &lang, const QString &name);
    QStringList getSnippetsNames(const QString &lang);

    int getNumberOfTabs();
    void setNumberOfTabs(int value);
    int getCurrentIndex();
    void setCurrentIndex(int index);
    void clearEditorStatus();
    QMap<QString, QVariant> getEditorStatus(int index);
    void setEditorStatus(int index, const QMap<QString, QVariant> &status);
    bool isHotExitLoadFromFile();
    void setHotExitLoadFromFile(bool value);

    QString getCFPath();
    void setCFPath(const QString &path);

    QString getSavePath();
    void setSavePath(const QString &path);

    void resetSettings();

    ~SettingManager();

  private:
    QString mSettingsFile;
    QSettings *mSettings;
};

} // namespace Settings

#endif // SETTINGSMANAGER_HPP

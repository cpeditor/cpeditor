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

namespace Settings
{

enum ViewMode
{
    FULL_EDITOR,
    FULL_IO,
    SPLIT
};

class SettingsManager
{
  public:
    static void init();

    static int getConnectionPort();
    static void setConnectionPort(int port);

    static int getTabStop();
    static void setTabStop(int num);

    static int getTimeLimit();
    static void setTimeLimit(int ms);

    static QRect getGeometry();
    static void setGeometry(const QRect &);

    static QString getFont();
    static void setFont(const QString &font);

    static QString getDefaultLanguage();
    static void setDefaultLanguage(const QString &lang);

    static QString getTemplatePath(const QString &lang);
    static void setTemplatePath(const QString &lang, const QString &path);

    static QString getRuntimeArguments(const QString &lang);
    static void setRuntimeArguments(const QString &lang, const QString &command);

    static QString getClangFormatBinary();
    static void setClangFormatBinary(const QString &binary);
    static QString getClangFormatStyle();
    static void setClangFormatStyle(const QString &style);

    static QString getCompileCommand(const QString &lang);
    static void setCompileCommand(const QString &lang, const QString &command);

    static QString getRunCommand(const QString &lang);
    static void setRunCommand(const QString &lang, const QString &command);

    static QString getEditorTheme();
    static void setEditorTheme(const QString &theme);

    static bool isHotkeyInUse();
    static void setHotKeyInUse(bool value);

    static bool isAutoParentheses();
    static void setAutoParentheses(bool value);

    static bool isAutoRemoveParentheses();
    static void setAutoRemoveParentheses(bool value);

    static bool isAutoIndent();
    static void setAutoIndent(bool value);

    static bool isAutoSave();
    static void setAutoSave(bool value);

    static bool isWrapText();
    static void setWrapText(bool value);

    static bool isBeta();
    static void setBeta(bool value);

    static bool isTabsReplaced();
    static void setTabsReplaced(bool value);

    static bool isSaveTests();
    static void setSaveTests(bool value);

    static bool isUseHotExit();
    static void setUseHotExit(bool value);

    static bool isCompetitiveCompanionActive();
    static void setCompetitiveCompanionActive(bool value);

    static bool isCompetitiveCompanionOpenNewTab();
    static void setCompetitiveCompanionOpenNewTab(bool value);

    static bool isMaximizedWindow();
    static void setMaximizedWindow(bool value);

    static bool isCheckUpdateOnStartup();
    static void checkUpdateOnStartup(bool value);

    static bool isFormatOnSave();
    static void formatOnSave(bool value);

    static int getTransparency();
    static void setTransparency(int val);

    static ViewMode getViewMode();
    static void setViewMode(const ViewMode &v);

    static QByteArray getSplitterSizes();
    static void setSplitterSizes(const QByteArray &state);

    static QByteArray getRightSplitterSizes();
    static void setRightSplitterSizes(const QByteArray &state);

    static void setHotkeyFormat(const QKeySequence &sequence);
    static void setHotkeyKill(const QKeySequence &sequence);
    static void setHotkeyCompileRun(const QKeySequence &sequence);
    static void setHotkeyRun(const QKeySequence &sequence);
    static void setHotkeyCompile(const QKeySequence &sequence);
    static void setHotkeyViewModeToggler(const QKeySequence &sequence);
    static void setHotkeySnippets(const QKeySequence &sequence);
    static QKeySequence getHotkeyFormat();
    static QKeySequence getHotkeyKill();
    static QKeySequence getHotkeyCompileRun();
    static QKeySequence getHotkeyRun();
    static QKeySequence getHotkeyCompile();
    static QKeySequence getHotkeyViewModeToggler();
    static QKeySequence getHotkeySnippets();

    static QString getSnippet(const QString &lang, const QString &name);
    static void setSnippet(const QString &lang, const QString &name, const QString &content);
    static void removeSnippet(const QString &lang, const QString &name);
    static QStringList getSnippetsNames(const QString &lang);

    static int getNumberOfTabs();
    static void setNumberOfTabs(int value);
    static int getCurrentIndex();
    static void setCurrentIndex(int index);
    static void clearEditorStatus();
    static QMap<QString, QVariant> getEditorStatus(int index);
    static void setEditorStatus(int index, const QMap<QString, QVariant> &status);
    static bool isHotExitLoadFromFile();
    static void setHotExitLoadFromFile(bool value);

    static QString getCFPath();
    static void setCFPath(const QString &path);

    static QString getSavePath();
    static void setSavePath(const QString &path);

    static QSettings *settings();

    static void resetSettings();

    static void destroy();

  private:
    static QString mSettingsFile;
    static QSettings *mSettings;
};

} // namespace Settings

#endif // SETTINGSMANAGER_HPP

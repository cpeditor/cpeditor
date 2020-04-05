/*
 * Copyright (C) 2019-2020 Ashar Khan <ashar786khan@gmail.com>
 *
 * This file is part of CP Editor.
 *
 * CP Editor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * I will not be responsible if CP Editor behaves in unexpected way and
 * causes your ratings to go down and or lose any important contest.
 *
 * Believe Software is "Software" and it isn't immune to bugs.
 *
 */

/*
 * The SettingsManager is used to manage the preferences and
 * the saved statuses like hot exit status and window status.
 * All functions are static, so you can call them at anywhere.
 */

#ifndef SETTINGSMANAGER_HPP
#define SETTINGSMANAGER_HPP

#include <QKeySequence>
#include <QRect>
#include <QSettings>
#include <QString>

namespace Settings
{

// The view mode of the editor
enum ViewMode
{
    FULL_EDITOR, // only the editor is on the screen
    FULL_IO,     // only the IO part is on the screen
    SPLIT        // both the editor and the IO part are on the screen with a splitter
};

namespace Default
{
const bool saveFaster = false;
const bool textWrap = false;
const bool autoIndent = true;
const bool autoParentheses = true;
const bool autoRemoveParentheses = true;
const bool autoSave = false;
const bool beta = false;
const bool tabsReplaced = false;
const bool saveTests = false;
const bool useHotExit = true;
const bool checkUpdateOnStartup = true;
const bool competitiveCompanionActive = true;
const bool competitiveCompanionOpenNewTab = true;
const bool hotKeyInUse = false;
const bool formatOnSave = false;
const QString javaRunCommand = "java";
const QString pythonRunCommand = "python";
const QString cppCompileCommand = "g++ -Wall";
const QString javaCompileCommand = "javac";
const QString clangFormatBinary = "clang-format";
const QString clangFormatStyle = "BasedOnStyle: Google";
const int connectionPort = 10045;
const int tabStopWidth = 4;
const int timeLimit = 5000;
const QString font = "monospace,9,-1,5,50,0,0,0,0,0";
const QString defaultLanguage = "C++";
const QString editorTheme = "Light";
const int transparency = 100;
const QString CFPath = "cf";
const bool compileAndRunOnly = false;
const bool displayEolnInDiff = false;
} // namespace Default

class SettingsManager
{
  public:
    // initialize the SettingsManager, must be called before reading/writing any setting
    static void init();

    // most functions is a pair of get/set
    // note that the default settings are set in the "get" functions

    static int getConnectionPort();
    static void setConnectionPort(int port);

    static int getTabStop();
    static void setTabStop(int num);

    static int getTimeLimit();
    static void setTimeLimit(int ms);

    // save the geometry of the main window
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

    // save the splitter size between the edior and the IO part
    static QByteArray getSplitterSizes();
    static void setSplitterSizes(const QByteArray &state);

    // save the splitter size between the message logger and the testcases
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

    // these are used for code snippets
    static QString getSnippet(const QString &lang, const QString &name);
    static void setSnippet(const QString &lang, const QString &name, const QString &content);
    static void removeSnippet(const QString &lang, const QString &name);
    static QStringList getSnippetsNames(const QString &lang);

    // these are used for hot exit
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

    // this is used for the default save path
    static QString getSavePath();
    static void setSavePath(const QString &path);

    static bool isCompileAndRunOnly();
    static void setCompileAndRunOnly(bool value);

    static bool isDisplayEolnInDiff();
    static void setDisplayEolnInDiff(bool value);

    static bool isSaveFaster();
    static void setSaveFaster(bool value);

    // get the internal QSettings
    static QSettings *settings();

    // reset all settings
    static void resetSettings();

    // please call this when the application exits
    static void destroy();

  private:
    static QString mSettingsFile; // the path to the settings file
    static QSettings *mSettings;  // the QSettings used for managing the settings
};

} // namespace Settings

#endif // SETTINGSMANAGER_HPP

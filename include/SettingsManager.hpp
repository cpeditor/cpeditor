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

#include "Util.hpp"

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
  // All functions with code here are implicitly inlined by the compiler.
  public:
    SettingManager();

    int getConnectionPort() { return mSettings->value("companion_port", 10045).toInt(); }
    void setConnectionPort(const int& port) { mSettings->setValue("companion_port", num); }

    int getTabStop() { return mSettings->value("tab_stop", 4).toInt(); }
    void setTabStop(const int& num) { mSettings->setValue("tab_stop", num); }

    int getTimeLimit() { return mSettings->value("time_limit", 5000).toInt(); }
    void setTimeLimit(const int& ms) { mSettings->setValue("time_limit", val); }

    QRect getGeometry() { return mSettings->value("geometry").toRect(); }
    void setGeometry(const QRect& rect) { mSettings->setValue("geometry", rect); }

    QString getFont() { return mSettings->value("font", "").toString(); }
    void setFont(const QString& font) { mSettings->setValue("font", font); }

    QString getDefaultLanguage() { return mSettings->value("lang", "C++").toString(); }
    void setDefaultLanguage(const QString& lang) { mSettings->setValue("lang", lang); }

    QString getTemplatePathCpp() { return mSettings->value("template_cpp", "").toString(); }
    QString getTemplatePathJava() { return mSettings->value("template_java", "").toString(); }
    QString getTemplatePathPython() { return mSettings->value("template_python", "").toString(); }
    void setTemplatePathCpp(const QString& path) { mSettings->setValue("template_cpp", path); }
    void setTemplatePathJava(const QString& path) { mSettings->setValue("template_java", path); }
    void setTemplatePathPython(const QString& path) { mSettings->setValue("template_python", path); }

    QString getRuntimeArgumentsCpp() { return mSettings->value("runtime_cpp", "").toString(); }
    QString getRuntimeArgumentsJava() { return mSettings->value("runtime_java", "").toString(); }
    QString getRuntimeArgumentsPython() { return mSettings->value("runtime_python", "").toString(); }
    void setRuntimeArgumentsCpp(const QString& command) { mSettings->setValue("runtime_cpp", command); }
    void setRuntimeArgumentsJava(const QString& command) { mSettings->setValue("runtime_java", command); }
    void setRuntimeArgumentsPython(const QString& command) { mSettings->setValue("runtime_python", command); }

    QString getClangFormatBinary() { return mSettings->value("clang_format_binary", "clang-format").toString(); }
    void setClangFormatBinary(const QString& binary) { mSettings->setValue("clang_format_binary", binary); }
    QString getClangFormatStyle() { return mSettings->value("clang_format_style", "BasedOnStyle: Google").toString();}
    void setClangFormatStyle(const QString& style) { mSettings->setValue("clang_format_style", style); }

    QString getCompileCommandCpp() { return mSettings->value("compile_cpp", "g++ -Wall").toString(); }
    QString getCompileCommandJava() { return mSettings->value("compile_java", "javac").toString(); }
    void setCompileCommandsCpp(const QString& command) { mSettings->setValue("compile_cpp", command); }
    void setCompileCommandsJava(const QString& command) { mSettings->setValue("compile_java", command); }

    QString getRunCommandJava() { return mSettings->value("run_java", "java").toString(); }
    QString getRunCommandPython() { return mSettings->value("run_python", "python").toString(); }
    void setRunCommandJava(const QString& command) { mSettings->setValue("run_java", command); }
    void setRunCommandPython(const QString& command) { mSettings->setValue("run_python", command); }

    QString getEditorTheme() { return mSettings->value("editor_theme", "Light").toString(); }
    void setEditorTheme(const QString& theme) { mSettings->setValue("editor_theme", themeName); }

    bool isHotkeyInUse() { return mSettings->value("hotkey_use", "false").toBool(); }
    void setHotKeyInUse(const bool& value) { mSettings->setValue("hotkey_use", bool2string(value)); }

    bool isAutoParenthesis() { return mSettings->value("auto_parenthesis", "true").toBool(); }
    void setAutoParenthesis(const bool& value) { mSettings->setValue("auto_parenthesis", bool2string(value)); }

    bool isAutoIndent() { return mSettings->value("auto_indent", "true").toBool(); }
    void setAutoIndent(const bool& value) { mSettings->setValue("auto_indent", bool2string(value)); }

    bool isAutoSave() { return mSettings->value("autosave", "false").toBool(); }
    void setAutoSave(const bool& value) { mSettings->setValue("autosave", bool2string(value)); }

    bool isWrapText() { return mSettings->value("wrap_text", "false").toBool(); }
    void setWrapText(const bool& value) { mSettings->setValue("wrap_text", bool2string(value)); }

    bool isBeta() { return mSettings->value("beta", "false").toBool(); }
    void setBeta(const bool& value) { mSettings->setValue("beta", bool2string(value)); }

    bool isTabsReplaced() { return mSettings->value("replace_tabs", "false").toBool(); }
    void setTabsReplaced(const bool& value) { mSettings->setValue("replace_tabs", bool2string(value)); }

    bool isSaveTests() { return mSettings->value("save_tests", "false").toBool(); }
    void setSaveTests(const bool& value) { mSettings->setValue("save_tests", bool2string(value)); }

    bool isUseHotExit() { return mSettings->value("use_hot_exit", "true").toBool(); }
    void setUseHotExit(const bool& value) { mSettings->setValue("use_hot_exit", bool2string(value)); }

    bool isCompetitiveCompanionActive() { return mSettings->value("competitive_use", "false").toBool(); }
    void setCompetitiveCompanionActive(const bool& value) { mSettings->setValue("competitive_use", value); }

    bool isCompetitiveCompanionOpenNewTab() { return mSettings->value("companion_new_tab", "true").toBool(); }
    void setCompetitiveCompanionOpenNewTab(const bool& value) { mSettings->setValue("companion_new_tab", value); }

    bool isMaximizedWindow() { return mSettings->value("win_max", "false").toBool(); }
    void setMaximizedWindow(const bool& value) { mSettings->setValue("win_max", bool2string(value)); }

    bool isCheckUpdateOnStartup() { return mSettings->value("update_start_check", "true").toBool(); }
    void checkUpdateOnStartup(const bool& value) { mSettings->setValue("update_start_check", bool2string(value)); }

    bool isFormatOnSave() { return mSettings->value("format_on_save", "false").toBool(); }
    void formatOnSave(const bool& value) { mSettings->setValue("format_on_save", bool2string(value)); }

    int getTransparency() { return mSettings->value("transparency", 100).toInt(); }
    void setTransparency(const int& val) { mSettings->setValue("transparency", val); }

    ViewMode getViewMode();
    void setViewMode(const ViewMode& v);

    QByteArray getSplitterSizes() { return mSettings->value("splitter_sizes").toByteArray(); }
    void setSplitterSizes(const QByteArray& state) { mSettings->setValue("splitter_sizes", state); }

    QByteArray getRightSplitterSizes() { return mSettings->value("right_splitter_sizes").toByteArray(); }
    void setRightSplitterSizes(const QByteArray& state) { mSettings->setValue("right_splitter_sizes", state); }

    void setHotkeyFormat(const QKeySequence& sequence) { mSettings->setValue("hotkey_format", sequence.toString()); }
    void setHotkeyKill(const QKeySequence& sequence) { mSettings->setValue("hotkey_kill", sequence.toString()); }
    void setHotkeyCompileRun(const QKeySequence& sequence) { mSettings->setValue("hotkey_compile_run", sequence.toString()); }
    void setHotkeyRun(const QKeySequence& sequence) { mSettings->setValue("hotkey_run", sequence.toString()); }
    void setHotkeyCompile(const QKeySequence& sequence) { mSettings->setValue("hotkey_compile", sequence.toString()); }
    void setHotkeyViewModeToggler(const QKeySequence& sequence) { mSettings->setValue("hotkey_mode_toggle", sequence.toString()); }
    void setHotkeySnippets(const QKeySequence& sequence) { mSettings->setValue("hotkey_snippets", sequence.toString()); }
    QKeySequence getHotkeyFormat() { return QKeySequence::fromString(mSettings->value("hotkey_format", "").toString()); }
    QKeySequence getHotkeyKill() { return QKeySequence::fromString(mSettings->value("hotkey_kill", "").toString()); }
    QKeySequence getHotkeyCompileRun() { return QKeySequence::fromString(mSettings->value("hotkey_compile_run", "").toString()); }
    QKeySequence getHotkeyRun() { return QKeySequence::fromString(mSettings->value("hotkey_run", "").toString()); }
    QKeySequence getHotkeyCompile() { return QKeySequence::fromString(mSettings->value("hotkey_compile", "").toString()); }
    QKeySequence getHotkeyViewModeToggler() { return QKeySequence::fromString(mSettings->value("hotkey_mode_toggle", "").toString()); }
    QKeySequence getHotkeySnippets() { return QKeySequence::fromString(mSettings->value("hotkey_snippets", "").toString()); }

    QString getSnippet(const QString& lang, const QString& name) { return mSettings->value("snippets/" + lang + "/" + name, "").toString(); }
    void setSnippet(const QString& lang, const QString& name, const QString& content) { mSettings->setValue("snippets/" + lang + "/" + name, content); }
    void removeSnippet(const QString& lang, const QString& name) { mSettings->remove("snippets/" + lang + "/" + name); }
    QStringList getSnippetsNames(const QString& lang);

    int getNumberOfTabs() { return mSettings->value("number_of_tabs", 0).toInt(); }
    void setNumberOfTabs(const int& value) { mSettings->setValue("number_of_tabs", value); }
    int getCurrentIndex() { return mSettings->value("current_index", -1).toInt(); }
    void setCurrentIndex(const int& index) { mSettings->setValue("current_index", index); }
    void clearEditorStatus() { mSettings->remove("editor_status"); }
    QMap<QString, QVariant> getEditorStatus(const int& index) { return mSettings->value("editor_status/" + QString::number(index)).toMap(); }
    void setEditorStatus(const int& index, const QMap<QString, QVariant>& status) { mSettings->setValue("editor_status/" + QString::number(index), status); }

    QString getCFPath() { return mSettings->value("cf_path", "cf").toString(); }
    void setCFPath(const QString& path) { mSettings->setValue("cf_path", path); }

    void resetSettings() { mSettings->clear(); }

    SettingsData toData();
    ~SettingManager();

  private:
    QString mSettingsFile;
    QSettings *mSettings;
};

} // namespace Settings

#endif // SETTINGSMANAGER_HPP

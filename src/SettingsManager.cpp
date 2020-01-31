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

#include <MessageLogger.hpp>
#include <QApplication>
#include <QStandardPaths>
#include <SettingsManager.hpp>

namespace Settings
{
SettingManager::SettingManager()
{
    mSettingsFile = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/" + SETTINGS_FILE;
    mSettings = new QSettings(mSettingsFile, QSettings::IniFormat);

    // backwords compatibility

    if (getDefaultLang() == "Cpp")
        setDefaultLanguage("C++");

    auto names = getSnippetsNames("Cpp");
    for (auto name : names)
        setSnippet("C++", name, getSnippet("Cpp", name));
    mSettings->remove("snippets/Cpp");
}

inline bool SettingManager::isWrapText()
{
    return mSettings->value("wrap_text", "false").toBool();
}

inline bool SettingManager::isAutoIndent()
{
    return mSettings->value("auto_indent", "true").toBool();
}

inline bool SettingManager::isAutoParenthesis()
{
    return mSettings->value("auto_parenthesis", "true").toBool();
}

inline bool SettingManager::isAutoSave()
{
    return mSettings->value("autosave", "false").toBool();
}

inline bool SettingManager::isBeta()
{
    return mSettings->value("beta", "false").toBool();
}

inline bool SettingManager::isTabsReplaced()
{
    return mSettings->value("replace_tabs", "false").toBool();
}

inline bool SettingManager::isSaveTests()
{
    return mSettings->value("save_tests", "false").toBool();
}

inline bool SettingManager::isUseHotExit()
{
    return mSettings->value("use_hot_exit", "true").toBool();
}

inline bool SettingManager::isMaximizedWindow()
{
    return mSettings->value("win_max", "false").toBool();
}

inline bool SettingManager::isCheckUpdateOnStartup()
{
    return mSettings->value("update_start_check", "true").toBool();
}

inline bool SettingManager::isCompetitiveCompanionActive()
{
    return mSettings->value("competitive_use", "false").toBool();
}

inline bool SettingManager::isCompetitiveCompanionOpenNewTab()
{
    return mSettings->value("companion_new_tab", "true").toBool();
}

inline bool SettingManager::isHotkeyInUse()
{
    return mSettings->value("hotkey_use", "false").toBool();
}

inline bool SettingManager::isFormatOnSave()
{
    return mSettings->value("format_on_save", "false").toBool();
}

inline QString SettingManager::getRunCommandJava()
{
    return mSettings->value("run_java", "java").toString();
}
inline QString SettingManager::getRunCommandPython()
{
    return mSettings->value("run_python", "python").toString();
}
inline QString SettingManager::getCompileCommandCpp()
{
    return mSettings->value("compile_cpp", "g++ -Wall").toString();
}
inline QString SettingManager::getCompileCommandJava()
{
    return mSettings->value("compile_java", "javac").toString();
}
inline QString SettingManager::getClangFormatBinary()
{
    return mSettings->value("clang_format_binary", "clang-format").toString();
}
inline QString SettingManager::getClangFormatStyle()
{
    return mSettings->value("clang_format_style", "BasedOnStyle: Google").toString();
}
inline QString SettingManager::getRuntimeArgumentsCpp()
{
    return mSettings->value("runtime_cpp", "").toString();
}
inline QString SettingManager::getRuntimeArgumentsJava()
{
    return mSettings->value("runtime_java", "").toString();
}
inline QString SettingManager::getRuntimeArgumentsPython()
{
    return mSettings->value("runtime_python", "").toString();
}
inline QString SettingManager::getDefaultLang()
{
    auto res = mSettings->value("lang", "C++").toString();
    return res;
}
inline QString SettingManager::getTemplatePathCpp()
{
    return mSettings->value("template_cpp", "").toString();
}
inline QString SettingManager::getTemplatePathJava()
{
    return mSettings->value("template_java", "").toString();
}
inline QString SettingManager::getTemplatePathPython()
{
    return mSettings->value("template_python", "").toString();
}
inline QString SettingManager::getFont()
{
    return mSettings->value("font", "").toString();
}

inline QRect SettingManager::getGeometry()
{
    return mSettings->value("geometry").toRect();
}

inline int SettingManager::getTabStop()
{
    return mSettings->value("tab_stop", 4).toInt();
}

inline int SettingManager::getConnectionPort()
{
    return mSettings->value("companion_port", 10045).toInt();
}

inline int SettingManager::getTimeLimit()
{
    return mSettings->value("time_limit", 5000).toInt();
}

inline void SettingManager::setAutoIndent(const bool& value)
{
    if (value)
        mSettings->setValue("auto_indent", QString::fromStdString("true"));
    else
        mSettings->setValue("auto_indent", QString::fromStdString("false"));
}

inline void SettingManager::setCompetitiveCompanionActive(const bool& value)
{
    mSettings->setValue("competitive_use", value);
}

inline void SettingManager::setCompetitiveCompanionOpenNewTab(const bool& value)
{
    mSettings->setValue("companion_new_tab", value);
}

inline void SettingManager::setWrapText(const bool& value)
{
    if (value)
        mSettings->setValue("wrap_text", QString::fromStdString("true"));
    else
        mSettings->setValue("wrap_text", QString::fromStdString("false"));
}

inline void SettingManager::setAutoParenthesis(const bool& value)
{
    if (value)
        mSettings->setValue("auto_parenthesis", QString::fromStdString("true"));
    else
        mSettings->setValue("auto_parenthesis", QString::fromStdString("false"));
}

inline void SettingManager::setAutoSave(const bool& value)
{
    if (value)
        mSettings->setValue("autosave", QString::fromStdString("true"));
    else
        mSettings->setValue("autosave", QString::fromStdString("false"));
}

inline void SettingManager::setBeta(const bool& value)
{
    if (value)
        mSettings->setValue("beta", QString::fromStdString("true"));
    else
        mSettings->setValue("beta", QString::fromStdString("false"));
}

inline void SettingManager::setTabsReplaced(const bool& value)
{
    if (value)
        mSettings->setValue("replace_tabs", QString::fromStdString("true"));
    else
        mSettings->setValue("replace_tabs", QString::fromStdString("false"));
}

inline void SettingManager::setSaveTests(const bool& value)
{
    if (value)
        mSettings->setValue("save_tests", QString::fromStdString("true"));
    else
        mSettings->setValue("save_tests", QString::fromStdString("false"));
}

inline void SettingManager::setUseHotExit(const bool& value)
{
    if (value)
        mSettings->setValue("use_hot_exit", QString::fromStdString("true"));
    else
        mSettings->setValue("use_hot_exit", QString::fromStdString("false"));
}

inline void SettingManager::setMaximizedWindow(const bool& value)
{
    if (value)
        mSettings->setValue("win_max", QString::fromStdString("true"));
    else
        mSettings->setValue("win_max", QString::fromStdString("false"));
}

inline void SettingManager::checkUpdateOnStartup(const bool& value)
{
    if (value)
        mSettings->setValue("update_start_check", QString::fromStdString("true"));
    else
        mSettings->setValue("update_start_check", QString::fromStdString("false"));
}

inline void SettingManager::setHotKeyInUse(const bool& value)
{
    if (value)
        mSettings->setValue("hotkey_use", QString::fromStdString("true"));
    else
        mSettings->setValue("hotkey_use", QString::fromStdString("false"));
}

inline void SettingManager::formatOnSave(const bool& value)
{
    if (value)
        mSettings->setValue("format_on_save", QString::fromStdString("true"));
    else
        mSettings->setValue("format_on_save", QString::fromStdString("false"));
}

inline void SettingManager::setTabStop(const int& num)
{
    mSettings->setValue("tab_stop", num);
}

inline void SettingManager::setConnectionPort(const int& num)
{
    mSettings->setValue("companion_port", num);
}

inline void SettingManager::setTimeLimit(const int& val)
{
    mSettings->setValue("time_limit", val);
}

inline void SettingManager::setRunCommandJava(const QString& command)
{
    mSettings->setValue("run_java", command);
}
inline void SettingManager::setRunCommandPython(const QString& command)
{
    mSettings->setValue("run_python", command);
}
inline void SettingManager::setCompileCommandsCpp(const QString& command)
{
    mSettings->setValue("compile_cpp", command);
}
inline void SettingManager::setEditorTheme(const QString& themeName)
{
    mSettings->setValue("editor_theme", themeName);
}
inline QString SettingManager::getEditorTheme()
{
    return mSettings->value("editor_theme", "Light").toString();
}
inline void SettingManager::setCompileCommandsJava(const QString& command)
{
    mSettings->setValue("compile_java", command);
}
inline void SettingManager::setClangFormatBinary(const QString& binary)
{
    mSettings->setValue("clang_format_binary", binary);
}
inline void SettingManager::setClangFormatStyle(const QString &style)
{
    mSettings->setValue("clang_format_style", style);
}
inline void SettingManager::setTemplatePathCpp(const QString& path)
{
    mSettings->setValue("template_cpp", path);
}
inline void SettingManager::setTemplatePathJava(const QString& path)
{
    mSettings->setValue("template_java", path);
}
inline void SettingManager::setTemplatePathPython(const QString& path)
{
    mSettings->setValue("template_python", path);
}
inline void SettingManager::setRuntimeArgumentsCpp(const QString& command)
{
    mSettings->setValue("runtime_cpp", command);
}
inline void SettingManager::setRuntimeArgumentsJava(const QString& command)
{
    mSettings->setValue("runtime_java", command);
}
inline void SettingManager::setRuntimeArgumentsPython(const QString& command)
{
    mSettings->setValue("runtime_python", command);
}
inline void SettingManager::setDefaultLanguage(const QString& lang)
{
    mSettings->setValue("lang", lang);
}
inline void SettingManager::setFont(const QString& font)
{
    mSettings->setValue("font", font);
}

inline void SettingManager::setGeometry(const QRect& rect)
{
    mSettings->setValue("geometry", rect);
}

inline QKeySequence SettingManager::getHotkeyCompile()
{
    return QKeySequence::fromString(mSettings->value("hotkey_compile", "").toString());
}
inline QKeySequence SettingManager::getHotkeyRun()
{
    return QKeySequence::fromString(mSettings->value("hotkey_run", "").toString());
}
inline QKeySequence SettingManager::getHotkeyCompileRun()
{
    return QKeySequence::fromString(mSettings->value("hotkey_compile_run", "").toString());
}
inline QKeySequence SettingManager::getHotkeyKill()
{
    return QKeySequence::fromString(mSettings->value("hotkey_kill", "").toString());
}
inline QKeySequence SettingManager::getHotkeyFormat()
{
    return QKeySequence::fromString(mSettings->value("hotkey_format", "").toString());
}
inline QKeySequence SettingManager::getHotkeyViewModeToggler()
{
    return QKeySequence::fromString(mSettings->value("hotkey_mode_toggle", "").toString());
}
inline QKeySequence SettingManager::getHotkeySnippets()
{
    return QKeySequence::fromString(mSettings->value("hotkey_snippets", "").toString());
}

inline QString SettingManager::getSnippet(const QString& lang, const QString& name)
{
    return mSettings->value("snippets/" + lang + "/" + name, "").toString();
}
inline void SettingManager::setSnippet(const QString& lang, const QString& name, const QString& content)
{
    mSettings->setValue("snippets/" + lang + "/" + name, content);
}
inline void SettingManager::removeSnippet(const QString& lang, const QString& name)
{
    mSettings->remove("snippets/" + lang + "/" + name);
}
QStringList SettingManager::getSnippetsNames(const QString& lang)
{
    mSettings->beginGroup("snippets");
    mSettings->beginGroup(lang);
    auto ret = mSettings->allKeys();
    mSettings->endGroup();
    mSettings->endGroup();
    ret.sort(Qt::CaseInsensitive);
    return ret;
}

inline int SettingManager::getNumberOfTabs()
{
    return mSettings->value("number_of_tabs", 0).toInt();
}
inline void SettingManager::setNumberOfTabs(const int& value)
{
    mSettings->setValue("number_of_tabs", value);
}
inline int SettingManager::getCurrentIndex()
{
    return mSettings->value("current_index", -1).toInt();
}
inline void SettingManager::setCurrentIndex(const int& index)
{
    mSettings->setValue("current_index", index);
}
inline void SettingManager::clearEditorStatus()
{
    mSettings->remove("editor_status");
}
inline QMap<QString, QVariant> SettingManager::getEditorStatus(const int& index)
{
    return mSettings->value("editor_status/" + QString::number(index)).toMap();
}
inline void SettingManager::setEditorStatus(const int& index, const QMap<QString, QVariant>& status)
{
    mSettings->setValue("editor_status/" + QString::number(index), status);
}

inline int SettingManager::getTransparency()
{
    return mSettings->value("transparency", 100).toInt();
}

inline void SettingManager::setTransparency(const int& val)
{
    mSettings->setValue("transparency", val);
}

inline QString SettingManager::getCFPath()
{
    return mSettings->value("cf_path", "cf").toString();
}

inline void SettingManager::setCFPath(const QString& path)
{
    mSettings->setValue("cf_path", path);
}

inline void SettingManager::setHotkeyViewModeToggler(const QKeySequence& sequence)
{
    mSettings->setValue("hotkey_mode_toggle", sequence.toString());
}
inline void SettingManager::setHotkeyCompile(const QKeySequence& sequence)
{
    mSettings->setValue("hotkey_compile", sequence.toString());
}
inline void SettingManager::setHotkeyRun(const QKeySequence& sequence)
{
    mSettings->setValue("hotkey_run", sequence.toString());
}
inline void SettingManager::setHotkeyCompileRun(const QKeySequence& sequence)
{
    mSettings->setValue("hotkey_compile_run", sequence.toString());
}
inline void SettingManager::setHotkeyKill(const QKeySequence& sequence)
{
    mSettings->setValue("hotkey_kill", sequence.toString());
}
inline void SettingManager::setHotkeyFormat(const QKeySequence& sequence)
{
    mSettings->setValue("hotkey_format", sequence.toString());
}
inline void SettingManager::setHotkeySnippets(const QKeySequence& sequence)
{
    mSettings->setValue("hotkey_snippets", sequence.toString());
}

inline ViewMode SettingManager::getViewMode()
{
    QString strings = mSettings->value("view_mode", "split").toString();
    if (strings == "split")
        return Settings::ViewMode::SPLIT;
    else if (strings == "code")
        return Settings::ViewMode::FULL_EDITOR;
    else
        return Settings::ViewMode::FULL_IO;
}

inline void SettingManager::setViewMode(const ViewMode& viewmode)
{
    QString ans;
    if (viewmode == Settings::FULL_EDITOR)
        ans = "code";
    else if (viewmode == Settings::FULL_IO)
        ans = "io";
    else
        ans = "split";
    mSettings->setValue("view_mode", ans);
}

inline QByteArray SettingManager::getSplitterSizes()
{
    return mSettings->value("splitter_sizes").toByteArray();
}

inline void SettingManager::setSplitterSizes(const QByteArray& state)
{
    mSettings->setValue("splitter_sizes", state);
}

inline QByteArray SettingManager::getRightSplitterSizes()
{
    return mSettings->value("right_splitter_sizes").toByteArray();
}

inline void SettingManager::setRightSplitterSizes(const QByteArray& state)
{
    mSettings->setValue("right_splitter_sizes", state);
}

SettingManager::~SettingManager()
{
    mSettings->sync();
    delete mSettings;
}

inline SettingsData SettingManager::toData()
{
    SettingsData data;
    data.companionPort = getConnectionPort();
    data.tabStop = getTabStop();
    data.timeLimit = getTimeLimit();
    data.geometry = getGeometry();
    data.font = getFont();
    data.defaultLanguage = getDefaultLang();
    data.templateCpp = getTemplatePathCpp();
    data.templateJava = getTemplatePathJava();
    data.templatePython = getTemplatePathPython();
    data.runtimeArgumentsCpp = getRuntimeArgumentsCpp();
    data.runtimeArgumentsJava = getRuntimeArgumentsJava();
    data.runtimeArgumentsPython = getRuntimeArgumentsPython();
    data.clangFormatBinary = getClangFormatBinary();
    data.clangFormatStyle = getClangFormatStyle();
    data.compileCommandCpp = getCompileCommandCpp();
    data.compileCommandJava = getCompileCommandJava();
    data.runCommandJava = getRunCommandJava();
    data.runCommandPython = getRunCommandPython();
    data.editorTheme = getEditorTheme();
    data.isHotKeyInUse = isHotkeyInUse();
    data.isAutoParenthesis = isAutoParenthesis();
    data.isAutoIndent = isAutoIndent();
    data.isAutoSave = isAutoSave();
    data.isWrapText = isWrapText();
    data.isBeta = isBeta();
    data.isTabsReplaced = isTabsReplaced();
    data.shouldSaveTests = isSaveTests();
    data.isCompanionActive = isCompetitiveCompanionActive();
    data.isWindowMaximized = isMaximizedWindow();
    data.isCheckUpdateOnStartup = isCheckUpdateOnStartup();
    data.isFormatOnSave = isFormatOnSave();
    data.hotkeyCompile = getHotkeyCompile();
    data.hotkeyRun = getHotkeyRun();
    data.hotkeyCompileRun = getHotkeyCompileRun();
    data.hotkeyKill = getHotkeyKill();
    data.hotkeyFormat = getHotkeyFormat();
    data.hotkeyViewModeToggler = getHotkeyViewModeToggler();
    data.hotkeySnippets = getHotkeySnippets();
    data.viewMode = getViewMode();
    data.splitterSizes = getSplitterSizes();
    data.cfPath = getCFPath();

    return data;
}

inline void SettingManager::resetSettings()
{
    mSettings->clear();
}

} // namespace Settings

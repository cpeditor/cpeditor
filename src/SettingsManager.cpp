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
}

bool SettingManager::isSystemThemeDark()
{
    return mSettings->value("dark_theme", "false").toBool();
}

bool SettingManager::isWrapText()
{
    return mSettings->value("wrap_text", "false").toBool();
}

bool SettingManager::isAutoIndent()
{
    return mSettings->value("auto_indent", "true").toBool();
}

bool SettingManager::isAutoParenthesis()
{
    return mSettings->value("auto_parenthesis", "true").toBool();
}
bool SettingManager::isAutoSave()
{
    return mSettings->value("autosave", "false").toBool();
}
bool SettingManager::isBeta()
{
    return mSettings->value("beta", "false").toBool();
}

bool SettingManager::isTabs()
{
    return mSettings->value("use_tabs", "false").toBool();
}

bool SettingManager::isSaveTests()
{
    return mSettings->value("save_tests", "false").toBool();
}

bool SettingManager::isMaximizedWindow()
{
    return mSettings->value("win_max", "false").toBool();
}

bool SettingManager::isCheckUpdateOnStartup()
{
    return mSettings->value("update_start_check", "true").toBool();
}

bool SettingManager::isCompetitiveCompanionActive()
{
    return mSettings->value("competitive_use", "false").toBool();
}
bool SettingManager::isHotkeyInUse()
{
    return mSettings->value("hotkey_use", "false").toBool();
}

QString SettingManager::getRunCommandJava()
{
    return mSettings->value("run_java", "").toString();
}
QString SettingManager::getRunCommandPython()
{
    return mSettings->value("run_python", "").toString();
}
QString SettingManager::getCompileCommandCpp()
{
    return mSettings->value("compile_cpp", "g++ -Wall").toString();
}
QString SettingManager::getCompileCommandJava()
{
    return mSettings->value("compile_java", "javac").toString();
}
QString SettingManager::getFormatCommand()
{
    return mSettings->value("format", "clang-format -i").toString();
}
QString SettingManager::getRuntimeArgumentsCpp()
{
    return mSettings->value("runtime_cpp", "").toString();
}
QString SettingManager::getRuntimeArgumentsJava()
{
    return mSettings->value("runtime_java", "").toString();
}
QString SettingManager::getRuntimeArgumentsPython()
{
    return mSettings->value("runtime_python", "").toString();
}
QString SettingManager::getDefaultLang()
{
    return mSettings->value("lang", "Cpp").toString();
}
QString SettingManager::getTemplatePathCpp()
{
    return mSettings->value("template_cpp", "").toString();
}
QString SettingManager::getTemplatePathJava()
{
    return mSettings->value("template_java", "").toString();
}
QString SettingManager::getTemplatePathPython()
{
    return mSettings->value("template_python", "").toString();
}
QString SettingManager::getFont()
{
    return mSettings->value("font", "").toString();
}

QRect SettingManager::getGeometry()
{
    return mSettings->value("geometry").toRect();
}

int SettingManager::getTabStop()
{
    return mSettings->value("tab_stop", 4).toInt();
}

int SettingManager::getConnectionPort()
{
    return mSettings->value("companion_port", 10045).toInt();
}

void SettingManager::setSystemThemeDark(bool value)
{
    if (value)
        mSettings->setValue("dark_theme", QString::fromStdString("true"));
    else
        mSettings->setValue("dark_theme", QString::fromStdString("false"));
}

void SettingManager::setAutoIndent(bool value)
{
    if (value)
        mSettings->setValue("auto_indent", QString::fromStdString("true"));
    else
        mSettings->setValue("auto_indent", QString::fromStdString("false"));
}

void SettingManager::setCompetitiveCompanionActive(bool value)
{
    mSettings->setValue("competitive_use", value);
}

void SettingManager::setWrapText(bool value)
{
    if (value)
        mSettings->setValue("wrap_text", QString::fromStdString("true"));
    else
        mSettings->setValue("wrap_text", QString::fromStdString("false"));
}

void SettingManager::setAutoParenthesis(bool value)
{
    if (value)
        mSettings->setValue("auto_parenthesis", QString::fromStdString("true"));
    else
        mSettings->setValue("auto_parenthesis", QString::fromStdString("false"));
}

void SettingManager::setAutoSave(bool value)
{
    if (value)
        mSettings->setValue("autosave", QString::fromStdString("true"));
    else
        mSettings->setValue("autosave", QString::fromStdString("false"));
}

void SettingManager::setBeta(bool value)
{
    if (value)
        mSettings->setValue("beta", QString::fromStdString("true"));
    else
        mSettings->setValue("beta", QString::fromStdString("false"));
}

void SettingManager::setTabs(bool value)
{
    if (value)
        mSettings->setValue("use_tabs", QString::fromStdString("true"));
    else
        mSettings->setValue("use_tabs", QString::fromStdString("false"));
}

void SettingManager::setSaveTests(bool value)
{
    if (value)
        mSettings->setValue("save_tests", QString::fromStdString("true"));
    else
        mSettings->setValue("save_tests", QString::fromStdString("false"));
}

void SettingManager::setMaximizedWindow(bool value)
{
    if (value)
        mSettings->setValue("win_max", QString::fromStdString("true"));
    else
        mSettings->setValue("win_max", QString::fromStdString("false"));
}

void SettingManager::checkUpdateOnStartup(bool value)
{
    if (value)
        mSettings->setValue("update_start_check", QString::fromStdString("true"));
    else
        mSettings->setValue("update_start_check", QString::fromStdString("false"));
}

void SettingManager::setHotKeyInUse(bool value)
{
    if (value)
        mSettings->setValue("hotkey_use", QString::fromStdString("true"));
    else
        mSettings->setValue("hotkey_use", QString::fromStdString("false"));
}

void SettingManager::setTabStop(int num)
{
    mSettings->setValue("tab_stop", num);
}

void SettingManager::setConnectionPort(int num)
{
    mSettings->setValue("companion_port", num);
}

void SettingManager::setRunCommandJava(QString command)
{
    mSettings->setValue("run_java", command);
}
void SettingManager::setRunCommandPython(QString command)
{
    mSettings->setValue("run_python", command);
}
void SettingManager::setCompileCommandsCpp(QString command)
{
    mSettings->setValue("compile_cpp", command);
}
void SettingManager::setEditorTheme(QString themeName)
{
    mSettings->setValue("editor_theme", themeName);
}
QString SettingManager::getEditorTheme()
{
    return mSettings->value("editor_theme", "Light").toString();
}
void SettingManager::setCompileCommandsJava(QString command)
{
    mSettings->setValue("compile_java", command);
}
void SettingManager::setFormatCommand(QString value)
{
    mSettings->setValue("format", value);
}
void SettingManager::setTemplatePathCpp(QString path)
{
    mSettings->setValue("template_cpp", path);
}
void SettingManager::setTemplatePathJava(QString path)
{
    mSettings->setValue("template_java", path);
}
void SettingManager::setTemplatePathPython(QString path)
{
    mSettings->setValue("template_python", path);
}
void SettingManager::setRuntimeArgumentsCpp(QString command)
{
    mSettings->setValue("runtime_cpp", command);
}
void SettingManager::setRuntimeArgumentsJava(QString command)
{
    mSettings->setValue("runtime_java", command);
}
void SettingManager::setRuntimeArgumentsPython(QString command)
{
    mSettings->setValue("runtime_python", command);
}
void SettingManager::setDefaultLanguage(QString lang)
{
    mSettings->setValue("lang", lang);
}
void SettingManager::setFont(QString font)
{
    mSettings->setValue("font", font);
}

void SettingManager::setGeometry(QRect rect)
{
    mSettings->setValue("geometry", rect);
}

QKeySequence SettingManager::getHotkeyCompile()
{
    return QKeySequence::fromString(mSettings->value("hotkey_compile", "").toString());
}
QKeySequence SettingManager::getHotkeyRun()
{
    return QKeySequence::fromString(mSettings->value("hotkey_run", "").toString());
}
QKeySequence SettingManager::getHotkeyCompileRun()
{
    return QKeySequence::fromString(mSettings->value("hotkey_compile_run", "").toString());
}
QKeySequence SettingManager::getHotkeyKill()
{
    return QKeySequence::fromString(mSettings->value("hotkey_kill", "").toString());
}
QKeySequence SettingManager::getHotkeyFormat()
{
    return QKeySequence::fromString(mSettings->value("hotkey_format", "").toString());
}
QKeySequence SettingManager::getHotkeyViewModeToggler()
{
    return QKeySequence::fromString(mSettings->value("hotkey_mode_toggle", "").toString());
}

QString SettingManager::getSnippet(QString lang, QString name)
{
    return mSettings->value("snippets/" + lang + "/" + name, "").toString();
}
void SettingManager::setSnippet(QString lang, QString name, QString content)
{
    mSettings->setValue("snippets/" + lang + "/" + name, content);
}
void SettingManager::removeSnippet(QString lang, QString name)
{
    mSettings->remove("snippets/" + lang + "/" + name);
}
QStringList SettingManager::getSnippetsNames(QString lang)
{
    mSettings->beginGroup("snippets");
    mSettings->beginGroup(lang);
    auto ret = mSettings->allKeys();
    mSettings->endGroup();
    mSettings->endGroup();
    ret.sort(Qt::CaseInsensitive);
    return ret;
}

void SettingManager::setHotkeyViewModeToggler(QKeySequence sequence)
{
    mSettings->setValue("hotkey_mode_toggle", sequence.toString());
}
void SettingManager::setHotkeyCompile(QKeySequence sequence)
{
    mSettings->setValue("hotkey_compile", sequence.toString());
}
void SettingManager::setHotkeyRun(QKeySequence sequence)
{
    mSettings->setValue("hotkey_run", sequence.toString());
}
void SettingManager::setHotkeyCompileRun(QKeySequence sequence)
{
    mSettings->setValue("hotkey_compile_run", sequence.toString());
}
void SettingManager::setHotkeyKill(QKeySequence sequence)
{
    mSettings->setValue("hotkey_kill", sequence.toString());
}
void SettingManager::setHotkeyFormat(QKeySequence sequence)
{
    mSettings->setValue("hotkey_format", sequence.toString());
}

ViewMode SettingManager::getViewMode()
{
    QString strings = mSettings->value("view_mode", "split").toString();
    if (strings == "split")
        return Settings::ViewMode::SPLIT;
    else if (strings == "code")
        return Settings::ViewMode::FULL_EDITOR;
    else
        return Settings::ViewMode::FULL_IO;
}

void SettingManager::setViewMode(ViewMode v)
{
    QString ans;
    if (v == Settings::FULL_EDITOR)
        ans = "code";
    else if (v == Settings::FULL_IO)
        ans = "io";
    else
        ans = "split";
    mSettings->setValue("view_mode", ans);
}

SettingManager::~SettingManager()
{
    mSettings->sync();
    delete mSettings;
}

SettingsData SettingManager::toData()
{
    SettingsData data;
    data.companionPort = getConnectionPort();
    data.tabStop = getTabStop();
    data.geometry = getGeometry();
    data.font = getFont();
    data.defaultLanguage = getDefaultLang();
    data.templateCpp = getTemplatePathCpp();
    data.templateJava = getTemplatePathJava();
    data.templatePython = getTemplatePathPython();
    data.runtimeArgumentsCpp = getRuntimeArgumentsCpp();
    data.runtimeArgumentsJava = getRuntimeArgumentsJava();
    data.runtimeArgumentsPython = getRuntimeArgumentsPython();
    data.formatCommand = getFormatCommand();
    data.compileCommandCpp = getCompileCommandCpp();
    data.compileCommandJava = getCompileCommandJava();
    data.runCommandJava = getRunCommandJava();
    data.runCommandPython = getRunCommandPython();
    data.editorTheme = getEditorTheme();
    data.isSystemThemeDark = isSystemThemeDark();
    data.isHotKeyInUse = isHotkeyInUse();
    data.isAutoParenthesis = isAutoParenthesis();
    data.isAutoIndent = isAutoIndent();
    data.isAutoSave = isAutoSave();
    data.isWrapText = isWrapText();
    data.isBeta = isBeta();
    data.isTabsBeingUsed = isTabs();
    data.shouldSaveTests = isSaveTests();
    data.isCompanionActive = isCompetitiveCompanionActive();
    data.isWindowMaximized = isMaximizedWindow();
    data.isCheckUpdateOnStartup = isCheckUpdateOnStartup();
    data.hotkeyCompile = getHotkeyCompile();
    data.hotkeyRun = getHotkeyRun();
    data.hotkeyCompileRun = getHotkeyCompileRun();
    data.hotkeyKill = getHotkeyKill();
    data.hotkeyFormat = getHotkeyFormat();
    data.hotkeyViewModeToggler = getHotkeyViewModeToggler();
    data.viewMode = getViewMode();

    return data;
}

} // namespace Settings

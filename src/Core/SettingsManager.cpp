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

#include "Core/SettingsManager.hpp"
#include "Core/EventLogger.hpp"
#include "Core/MessageLogger.hpp"
#include <QApplication>
#include <QDir>
#include <QStandardPaths>

namespace Settings
{

const QString OLD_SETTINGS_FILE = "cp_editor_settings.ini";
const QString SETTINGS_FILE = ".cp_editor_settings.ini";

QString SettingsManager::mSettingsFile;
QSettings *SettingsManager::mSettings;

void SettingsManager::init()
{
    Core::Log::i("settingmanager/constructor", "Initiating Settings manager");

    // move from the old path to the new one for backward compatibility

    mSettingsFile = QDir(QStandardPaths::writableLocation(QStandardPaths::HomeLocation)).filePath(SETTINGS_FILE);
    QString oldSettingsFile =
        QDir(QStandardPaths::writableLocation(QStandardPaths::HomeLocation)).filePath(OLD_SETTINGS_FILE);

    if (!QFile::exists(mSettingsFile) && QFile::exists(oldSettingsFile))
    {
        Core::Log::i("settingmanager/constructor",
                     mSettingsFile + "doesn't exist, but " + oldSettingsFile + " exists.");

        if (QFile::copy(oldSettingsFile, mSettingsFile))
        {
            Core::Log::i("settingmanager/constructor", "Old Settings migrated to new Settings File");

            if (QFile::remove(oldSettingsFile))
                Core::Log::i("settingmanager/constructor", oldSettingsFile + " File deleted successfully.");
            else
                Core::Log::e("settingmanager/constructor", oldSettingsFile + " File failed to delete.");
        }
        else
        {
            Core::Log::e("settingmanager/constructor", "Setting migration failed");
            mSettingsFile = oldSettingsFile;
            Core::Log::w("settingmanager/constructor", "Reverted to old settings file");
        }
    }
    else
    {
        Core::Log::i(
            "settingmanager/constructor",
            "The new settings file exists or the old settings file does not exist, use the new settings file.");
    }

    // create the QSettings
    mSettings = new QSettings(mSettingsFile, QSettings::IniFormat);

    // move "Cpp" to "C++" for backward compatibility

    if (getDefaultLanguage() == "Cpp")
        setDefaultLanguage("C++");

    auto names = getSnippetsNames("Cpp");
    for (auto name : names)
        setSnippet("C++", name, getSnippet("Cpp", name));
    mSettings->remove("snippets/Cpp");
}

bool SettingsManager::isWrapText()
{
    return mSettings->value("wrap_text", "false").toBool();
}

bool SettingsManager::isAutoIndent()
{
    return mSettings->value("auto_indent", "true").toBool();
}

bool SettingsManager::isAutoParentheses()
{
    return mSettings->value("auto_parenthesis", "true").toBool();
}

bool SettingsManager::isAutoRemoveParentheses()
{
    return mSettings->value("auto_remove_parentheses", "true").toBool();
}

bool SettingsManager::isAutoSave()
{
    return mSettings->value("autosave", "false").toBool();
}

bool SettingsManager::isBeta()
{
    return mSettings->value("beta", "false").toBool();
}

bool SettingsManager::isTabsReplaced()
{
    return mSettings->value("replace_tabs", "false").toBool();
}

bool SettingsManager::isSaveTests()
{
    return mSettings->value("save_tests", "false").toBool();
}

bool SettingsManager::isUseHotExit()
{
    return mSettings->value("use_hot_exit", "true").toBool();
}

bool SettingsManager::isMaximizedWindow()
{
    return mSettings->value("win_max", "false").toBool();
}

bool SettingsManager::isCheckUpdateOnStartup()
{
    return mSettings->value("update_start_check", "true").toBool();
}

bool SettingsManager::isCompetitiveCompanionActive()
{
    return mSettings->value("competitive_use", "true").toBool();
}

bool SettingsManager::isCompetitiveCompanionOpenNewTab()
{
    return mSettings->value("companion_new_tab", "true").toBool();
}

bool SettingsManager::isHotkeyInUse()
{
    return mSettings->value("hotkey_use", "false").toBool();
}
bool SettingsManager::isFormatOnSave()
{
    return mSettings->value("format_on_save", "false").toBool();
}

QString SettingsManager::getRunCommand(const QString &lang)
{
    if (lang == "Java")
        return mSettings->value("run_java", "java").toString();
    else if (lang == "Python")
        return mSettings->value("run_python", "python").toString();
    else
        return mSettings->value("run_" + lang).toString();
}
QString SettingsManager::getCompileCommand(const QString &lang)
{
    if (lang == "C++")
        return mSettings->value("compile_cpp", "g++ -Wall").toString();
    else if (lang == "Java")
        return mSettings->value("compile_java", "javac").toString();
    else
        return mSettings->value("compile_" + lang).toString();
}
QString SettingsManager::getClangFormatBinary()
{
    return mSettings->value("clang_format_binary", "clang-format").toString();
}
QString SettingsManager::getClangFormatStyle()
{
    return mSettings->value("clang_format_style", "BasedOnStyle: Google").toString();
}
QString SettingsManager::getRuntimeArguments(const QString &lang)
{
    if (lang == "C++")
        return mSettings->value("runtime_cpp", "").toString();
    else if (lang == "Java")
        return mSettings->value("runtime_java", "").toString();
    else if (lang == "Python")
        return mSettings->value("runtime_python", "").toString();
    else
        return mSettings->value("runtime_" + lang).toString();
}
QString SettingsManager::getDefaultLanguage()
{
    auto res = mSettings->value("lang", "C++").toString();
    return res;
}
QString SettingsManager::getTemplatePath(const QString &lang)
{
    if (lang == "C++")
        return mSettings->value("template_cpp", "").toString();
    else if (lang == "Java")
        return mSettings->value("template_java", "").toString();
    else if (lang == "Python")
        return mSettings->value("template_python", "").toString();
    else
        return mSettings->value("template_" + lang).toString();
}
QString SettingsManager::getFont()
{
    return mSettings->value("font", "monospace").toString();
}

QRect SettingsManager::getGeometry()
{
    return mSettings->value("geometry").toRect();
}

int SettingsManager::getTabStop()
{
    return mSettings->value("tab_stop", 4).toInt();
}

int SettingsManager::getConnectionPort()
{
    return mSettings->value("companion_port", 10045).toInt();
}

int SettingsManager::getTimeLimit()
{
    return mSettings->value("time_limit", 5000).toInt();
}

void SettingsManager::setAutoIndent(bool value)
{
    mSettings->setValue("auto_indent", value ? "true" : "false");
}

void SettingsManager::setCompetitiveCompanionActive(bool value)
{
    mSettings->setValue("competitive_use", value);
}

void SettingsManager::setCompetitiveCompanionOpenNewTab(bool value)
{
    mSettings->setValue("companion_new_tab", value);
}

void SettingsManager::setWrapText(bool value)
{
    mSettings->setValue("wrap_text", value ? "true" : "false");
}

void SettingsManager::setAutoParentheses(bool value)
{
    mSettings->setValue("auto_parenthesis", value ? "true" : "false");
}

void SettingsManager::setAutoRemoveParentheses(bool value)
{
    mSettings->setValue("auto_remove_parentheses", value ? "true" : "false");
}

void SettingsManager::setAutoSave(bool value)
{
    mSettings->setValue("autosave", value ? "true" : "false");
}

void SettingsManager::setBeta(bool value)
{
    mSettings->setValue("beta", value ? "true" : "false");
}

void SettingsManager::setTabsReplaced(bool value)
{
    mSettings->setValue("replace_tabs", value ? "true" : "false");
}

void SettingsManager::setSaveTests(bool value)
{
    mSettings->setValue("save_tests", value ? "true" : "false");
}

void SettingsManager::setUseHotExit(bool value)
{
    mSettings->setValue("use_hot_exit", value ? "true" : "false");
}

void SettingsManager::setMaximizedWindow(bool value)
{
    mSettings->setValue("win_max", value ? "true" : "false");
}

void SettingsManager::checkUpdateOnStartup(bool value)
{
    mSettings->setValue("update_start_check", value ? "true" : "false");
}

void SettingsManager::setHotKeyInUse(bool value)
{
    mSettings->setValue("hotkey_use", value ? "true" : "false");
}

void SettingsManager::formatOnSave(bool value)
{
    mSettings->setValue("format_on_save", value ? "true" : "false");
}

void SettingsManager::setTabStop(int num)
{
    mSettings->setValue("tab_stop", num);
}

void SettingsManager::setConnectionPort(int num)
{
    mSettings->setValue("companion_port", num);
}

void SettingsManager::setTimeLimit(int val)
{
    mSettings->setValue("time_limit", val);
}

void SettingsManager::setRunCommand(const QString &lang, const QString &command)
{
    if (lang == "Java")
        mSettings->setValue("run_java", command);
    else if (lang == "Python")
        mSettings->setValue("run_python", command);
    else
        mSettings->setValue("run_" + lang, command);
}
void SettingsManager::setCompileCommand(const QString &lang, const QString &command)
{
    if (lang == "C++")
        mSettings->setValue("compile_cpp", command);
    else if (lang == "Java")
        mSettings->setValue("compile_java", command);
    else
        mSettings->setValue("compile_" + lang, command);
}
void SettingsManager::setEditorTheme(const QString &themeName)
{
    mSettings->setValue("editor_theme", themeName);
}
QString SettingsManager::getEditorTheme()
{
    return mSettings->value("editor_theme", "Light").toString();
}
void SettingsManager::setClangFormatBinary(const QString &binary)
{
    mSettings->setValue("clang_format_binary", binary);
}
void SettingsManager::setClangFormatStyle(const QString &style)
{
    Core::Log::i("settingmanager/setclangformatStyle") << "style \n" << style << endl;
    mSettings->setValue("clang_format_style", style);
}
void SettingsManager::setTemplatePath(const QString &lang, const QString &path)
{
    if (lang == "C++")
        mSettings->setValue("template_cpp", path);
    else if (lang == "Java")
        mSettings->setValue("template_java", path);
    else if (lang == "Python")
        mSettings->setValue("template_python", path);
    else
        mSettings->setValue("template_" + lang, path);
}
void SettingsManager::setRuntimeArguments(const QString &lang, const QString &command)
{
    if (lang == "C++")
        mSettings->setValue("runtime_cpp", command);
    else if (lang == "Java")
        mSettings->setValue("runtime_java", command);
    else if (lang == "Python")
        mSettings->setValue("runtime_python", command);
    else
        mSettings->setValue("runtime_" + lang, command);
}
void SettingsManager::setDefaultLanguage(const QString &lang)
{
    mSettings->setValue("lang", lang);
}
void SettingsManager::setFont(const QString &font)
{
    mSettings->setValue("font", font);
}

void SettingsManager::setGeometry(const QRect &rect)
{
    int sx = rect.x();
    int sy = rect.y();
    Core::Log::i("settingmanager/setGeometry") << "Rect is :  (" << sx << "," << sy << ")" << endl;
    mSettings->setValue("geometry", rect);
}

QKeySequence SettingsManager::getHotkeyCompile()
{
    return QKeySequence::fromString(mSettings->value("hotkey_compile", "").toString());
}
QKeySequence SettingsManager::getHotkeyRun()
{
    return QKeySequence::fromString(mSettings->value("hotkey_run", "").toString());
}
QKeySequence SettingsManager::getHotkeyCompileRun()
{
    return QKeySequence::fromString(mSettings->value("hotkey_compile_run", "").toString());
}
QKeySequence SettingsManager::getHotkeyKill()
{
    return QKeySequence::fromString(mSettings->value("hotkey_kill", "").toString());
}
QKeySequence SettingsManager::getHotkeyFormat()
{
    return QKeySequence::fromString(mSettings->value("hotkey_format", "").toString());
}
QKeySequence SettingsManager::getHotkeyViewModeToggler()
{
    return QKeySequence::fromString(mSettings->value("hotkey_mode_toggle", "").toString());
}
QKeySequence SettingsManager::getHotkeySnippets()
{
    return QKeySequence::fromString(mSettings->value("hotkey_snippets", "").toString());
}

QString SettingsManager::getSnippet(const QString &lang, const QString &name)
{
    return mSettings->value("snippets/" + lang + "/" + name, "").toString();
}
void SettingsManager::setSnippet(const QString &lang, const QString &name, const QString &content)
{
    mSettings->setValue("snippets/" + lang + "/" + name, content);
}
void SettingsManager::removeSnippet(const QString &lang, const QString &name)
{
    mSettings->remove("snippets/" + lang + "/" + name);
}
QStringList SettingsManager::getSnippetsNames(const QString &lang)
{
    mSettings->beginGroup("snippets");
    mSettings->beginGroup(lang);
    auto ret = mSettings->allKeys();
    mSettings->endGroup();
    mSettings->endGroup();
    ret.sort(Qt::CaseInsensitive);
    return ret;
}

int SettingsManager::getNumberOfTabs()
{
    return mSettings->value("number_of_tabs", 0).toInt();
}
void SettingsManager::setNumberOfTabs(int value)
{
    mSettings->setValue("number_of_tabs", value);
}
int SettingsManager::getCurrentIndex()
{
    return mSettings->value("current_index", -1).toInt();
}
void SettingsManager::setCurrentIndex(int index)
{
    mSettings->setValue("current_index", index);
}
void SettingsManager::clearEditorStatus()
{
    Core::Log::i("settingmanager/clearEditorStatus", "Editor status is being cleared");
    mSettings->remove("editor_status");
}
QMap<QString, QVariant> SettingsManager::getEditorStatus(int index)
{
    return mSettings->value("editor_status/" + QString::number(index)).toMap();
}
void SettingsManager::setEditorStatus(int index, const QMap<QString, QVariant> &status)
{
    Core::Log::i("settingmanager/setEditorStatus", "Editor status is being saved");
    mSettings->setValue("editor_status/" + QString::number(index), status);
}
bool SettingsManager::isHotExitLoadFromFile()
{
    return mSettings->value("hot_exit_load_from_file", "false").toBool();
}
void SettingsManager::setHotExitLoadFromFile(bool value)
{
    mSettings->setValue("hot_exit_load_from_file", value ? "true" : "false");
}

int SettingsManager::getTransparency()
{
    return mSettings->value("transparency", 100).toInt();
}

void SettingsManager::setTransparency(int val)
{
    mSettings->setValue("transparency", val);
}

QString SettingsManager::getCFPath()
{
    return mSettings->value("cf_path", "cf").toString();
}

void SettingsManager::setCFPath(const QString &path)
{
    mSettings->setValue("cf_path", path);
}

QString SettingsManager::getSavePath()
{
    return mSettings->value("save_path").toString();
}

void SettingsManager::setSavePath(const QString &path)
{
    mSettings->setValue("save_path", path);
}

bool SettingsManager::isCompileAndRunOnly()
{
    return mSettings->value("compile_and_run_only", "false").toBool();
}

void SettingsManager::setCompileAndRunOnly(bool value)
{
    mSettings->setValue("compile_and_run_only", value ? "true" : "false");
}

bool SettingsManager::isDisplayEolnInDiff()
{
    return mSettings->value("display_eoln_in_diff", "false").toBool();
}

void SettingsManager::setDisplayEolnInDiff(bool value)
{
    mSettings->setValue("display_eoln_in_diff", value ? "true" : "false");
}

bool SettingsManager::isSaveFaster()
{
    return mSettings->value("save_faster", "false").toBool();
}

void SettingsManager::setSaveFaster(bool value)
{
    mSettings->setValue("save_faster", value ? "true" : "false");
}

void SettingsManager::setHotkeyViewModeToggler(const QKeySequence &sequence)
{
    mSettings->setValue("hotkey_mode_toggle", sequence.toString());
}
void SettingsManager::setHotkeyCompile(const QKeySequence &sequence)
{
    mSettings->setValue("hotkey_compile", sequence.toString());
}
void SettingsManager::setHotkeyRun(const QKeySequence &sequence)
{
    mSettings->setValue("hotkey_run", sequence.toString());
}
void SettingsManager::setHotkeyCompileRun(const QKeySequence &sequence)
{
    mSettings->setValue("hotkey_compile_run", sequence.toString());
}
void SettingsManager::setHotkeyKill(const QKeySequence &sequence)
{
    mSettings->setValue("hotkey_kill", sequence.toString());
}
void SettingsManager::setHotkeyFormat(const QKeySequence &sequence)
{
    mSettings->setValue("hotkey_format", sequence.toString());
}
void SettingsManager::setHotkeySnippets(const QKeySequence &sequence)
{
    mSettings->setValue("hotkey_snippets", sequence.toString());
}

ViewMode SettingsManager::getViewMode()
{
    QString strings = mSettings->value("view_mode", "split").toString();
    if (strings == "split")
        return Settings::ViewMode::SPLIT;
    else if (strings == "code")
        return Settings::ViewMode::FULL_EDITOR;
    else
        return Settings::ViewMode::FULL_IO;
}

void SettingsManager::setViewMode(const ViewMode &v)
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

QByteArray SettingsManager::getSplitterSizes()
{
    return mSettings->value("splitter_sizes").toByteArray();
}

void SettingsManager::setSplitterSizes(const QByteArray &state)
{
    mSettings->setValue("splitter_sizes", state);
}

QByteArray SettingsManager::getRightSplitterSizes()
{
    return mSettings->value("right_splitter_sizes").toByteArray();
}

void SettingsManager::setRightSplitterSizes(const QByteArray &state)
{
    mSettings->setValue("right_splitter_sizes", state);
}

QSettings *SettingsManager::settings()
{
    return mSettings;
}

void SettingsManager::destroy()
{
    Core::Log::i("settingmanager/destroyed", "Sync and destoryed setting manager object");
    mSettings->sync();
    delete mSettings;
}

void SettingsManager::resetSettings()
{
    // the default settings are set in the "get" functions, so a simple clear is enough
    Core::Log::i("settingmanager/resetSetting", "mSettings->clear(), invoked");
    mSettings->clear();
}

} // namespace Settings

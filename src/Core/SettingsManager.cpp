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
    Core::Log::i("settingmanager/constructor", "Invoked");

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
    mSettings = new QSettings(mSettingsFile, QSettings::IniFormat);

    if (getDefaultLanguage() == "Cpp")
        setDefaultLanguage("C++");

    auto names = getSnippetsNames("Cpp");
    for (auto name : names)
        setSnippet("C++", name, getSnippet("Cpp", name));
    mSettings->remove("snippets/Cpp");
}

bool SettingsManager::isWrapText()
{
    Core::Log::i("settingmanager/isWrapText", "Invoked");
    return mSettings->value("wrap_text", "false").toBool();
}

bool SettingsManager::isAutoIndent()
{
    Core::Log::i("settingmanager/isAutoIndent", "Invoked");
    return mSettings->value("auto_indent", "true").toBool();
}

bool SettingsManager::isAutoParentheses()
{
    Core::Log::i("settingmanager/isAutoParentheses", "Invoked");
    return mSettings->value("auto_parenthesis", "true").toBool();
}

bool SettingsManager::isAutoRemoveParentheses()
{
    Core::Log::i("settingmanager/isAutoRemoveParentheses", "Invoked");
    return mSettings->value("auto_remove_parentheses", "true").toBool();
}

bool SettingsManager::isAutoSave()
{
    Core::Log::i("settingmanager/isAutoSave", "Invoked");
    return mSettings->value("autosave", "false").toBool();
}

bool SettingsManager::isBeta()
{
    Core::Log::i("settingmanager/isBeta", "Invoked");
    return mSettings->value("beta", "false").toBool();
}

bool SettingsManager::isTabsReplaced()
{
    Core::Log::i("settingmanager/isTabReplaced", "Invoked");
    return mSettings->value("replace_tabs", "false").toBool();
}

bool SettingsManager::isSaveTests()
{
    Core::Log::i("settingmanager/isSaveTests", "Invoked");
    return mSettings->value("save_tests", "false").toBool();
}

bool SettingsManager::isUseHotExit()
{
    Core::Log::i("settingmanager/isuseHotExit", "Invoked");
    return mSettings->value("use_hot_exit", "true").toBool();
}

bool SettingsManager::isMaximizedWindow()
{
    Core::Log::i("settingmanager/ismaximizedWindow", "Invoked");
    return mSettings->value("win_max", "false").toBool();
}

bool SettingsManager::isCheckUpdateOnStartup()
{
    Core::Log::i("settingmanager/ischeckUpdateOnStartup", "Invoked");
    return mSettings->value("update_start_check", "true").toBool();
}

bool SettingsManager::isCompetitiveCompanionActive()
{
    Core::Log::i("settingmanager/isCompetitiveCompanionActive", "Invoked");
    return mSettings->value("competitive_use", "true").toBool();
}

bool SettingsManager::isCompetitiveCompanionOpenNewTab()
{
    Core::Log::i("settingmanager/isCompetitiveompanionOpenNewTab", "Invoked");
    return mSettings->value("companion_new_tab", "true").toBool();
}

bool SettingsManager::isHotkeyInUse()
{
    Core::Log::i("settingmanager/isHotKeyInUse", "Invoked");
    return mSettings->value("hotkey_use", "false").toBool();
}
bool SettingsManager::isFormatOnSave()
{
    Core::Log::i("settingmanager/isFormatOnSave", "Invoked");
    return mSettings->value("format_on_save", "false").toBool();
}

QString SettingsManager::getRunCommand(const QString &lang)
{
    Core::Log::i("settingmanager/getRunCommand") << "lang " << lang << endl;
    if (lang == "Java")
        return mSettings->value("run_java", "java").toString();
    else if (lang == "Python")
        return mSettings->value("run_python", "python").toString();
    else
        return mSettings->value("run_" + lang).toString();
}
QString SettingsManager::getCompileCommand(const QString &lang)
{
    Core::Log::i("settingmanager/getCompileCommand") << "lang " << lang << endl;
    if (lang == "C++")
        return mSettings->value("compile_cpp", "g++ -Wall").toString();
    else if (lang == "Java")
        return mSettings->value("compile_java", "javac").toString();
    else
        return mSettings->value("compile_" + lang).toString();
}
QString SettingsManager::getClangFormatBinary()
{
    Core::Log::i("settingmanager/getClangFormatBinary", "Invoked");
    return mSettings->value("clang_format_binary", "clang-format").toString();
}
QString SettingsManager::getClangFormatStyle()
{
    Core::Log::i("settingmanager/getClangFormatstyle", "Invoked");
    return mSettings->value("clang_format_style", "BasedOnStyle: Google").toString();
}
QString SettingsManager::getRuntimeArguments(const QString &lang)
{
    Core::Log::i("settingmanager/getRuntimeArgs") << "lang " << lang << endl;
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
    Core::Log::i("settingmanager/getDefaultLanguage", "Invoked");
    auto res = mSettings->value("lang", "C++").toString();
    return res;
}
QString SettingsManager::getTemplatePath(const QString &lang)
{
    Core::Log::i("settingmanager/getTemplatePath") << "lang " << lang << endl;
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
    Core::Log::i("settingmanager/getfont", "Invoked");
    return mSettings->value("font", "monospace").toString();
}

QRect SettingsManager::getGeometry()
{
    Core::Log::i("settingmanager/getGeometry", "Invoked");
    return mSettings->value("geometry").toRect();
}

int SettingsManager::getTabStop()
{
    Core::Log::i("settingmanager/getTabStop", "Invoked");
    return mSettings->value("tab_stop", 4).toInt();
}

int SettingsManager::getConnectionPort()
{
    Core::Log::i("settingmanager/getConnectionPort", "Invoked");
    return mSettings->value("companion_port", 10045).toInt();
}

int SettingsManager::getTimeLimit()
{
    Core::Log::i("settingmanager/getTimeLimit", "Invoked");
    return mSettings->value("time_limit", 5000).toInt();
}

void SettingsManager::setAutoIndent(bool value)
{
    Core::Log::i("settingmanager/setAutoIndent") << "value is " << value << endl;
    mSettings->setValue("auto_indent", value ? "true" : "false");
}

void SettingsManager::setCompetitiveCompanionActive(bool value)
{
    Core::Log::i("settingmanager/setCompetitiveCompanionActive") << "value is " << value << endl;
    mSettings->setValue("competitive_use", value);
}

void SettingsManager::setCompetitiveCompanionOpenNewTab(bool value)
{
    Core::Log::i("settingmanager/setCompetitiveCompanionOpenNewTab") << "value is " << value << endl;
    mSettings->setValue("companion_new_tab", value);
}

void SettingsManager::setWrapText(bool value)
{
    Core::Log::i("settingmanager/setWrapText") << "value is " << value << endl;
    mSettings->setValue("wrap_text", value ? "true" : "false");
}

void SettingsManager::setAutoParentheses(bool value)
{
    Core::Log::i("settingmanager/setAutoParentheses") << "value is " << value << endl;
    mSettings->setValue("auto_parenthesis", value ? "true" : "false");
}

void SettingsManager::setAutoRemoveParentheses(bool value)
{
    Core::Log::i("settingmanager/setAutoRemoveParenthesis") << "value is " << value << endl;
    mSettings->setValue("auto_remove_parentheses", value ? "true" : "false");
}

void SettingsManager::setAutoSave(bool value)
{
    Core::Log::i("settingmanager/setAutoSave") << "value is " << value << endl;
    mSettings->setValue("autosave", value ? "true" : "false");
}

void SettingsManager::setBeta(bool value)
{
    Core::Log::i("settingmanager/setBeta") << "value is " << value << endl;
    mSettings->setValue("beta", value ? "true" : "false");
}

void SettingsManager::setTabsReplaced(bool value)
{
    Core::Log::i("settingmanager/replaceTabs") << "value is " << value << endl;
    mSettings->setValue("replace_tabs", value ? "true" : "false");
}

void SettingsManager::setSaveTests(bool value)
{
    Core::Log::i("settingmanager/setSaveTests") << "value is " << value << endl;
    mSettings->setValue("save_tests", value ? "true" : "false");
}

void SettingsManager::setUseHotExit(bool value)
{
    Core::Log::i("settingmanager/useHotExit") << "value is " << value << endl;
    mSettings->setValue("use_hot_exit", value ? "true" : "false");
}

void SettingsManager::setMaximizedWindow(bool value)
{
    Core::Log::i("settingmanager/windowMaximized") << "value is " << value << endl;
    mSettings->setValue("win_max", value ? "true" : "false");
}

void SettingsManager::checkUpdateOnStartup(bool value)
{
    Core::Log::i("settingmanager/updateOnStart") << "value is " << value << endl;
    mSettings->setValue("update_start_check", value ? "true" : "false");
}

void SettingsManager::setHotKeyInUse(bool value)
{
    Core::Log::i("settingmanager/hotKeyInUse") << "value is " << value << endl;
    mSettings->setValue("hotkey_use", value ? "true" : "false");
}

void SettingsManager::formatOnSave(bool value)
{
    Core::Log::i("settingmanager/formatOnSave") << "value is " << value << endl;
    mSettings->setValue("format_on_save", value ? "true" : "false");
}

void SettingsManager::setTabStop(int num)
{
    Core::Log::i("settingmanager/setTabStop") << "stop distance " << num << endl;
    mSettings->setValue("tab_stop", num);
}

void SettingsManager::setConnectionPort(int num)
{
    Core::Log::i("settingmanager/setConnectionPort") << "port is " << num << endl;
    mSettings->setValue("companion_port", num);
}

void SettingsManager::setTimeLimit(int val)
{
    Core::Log::i("settingmanager/setTimeLimit") << "time " << val << endl;
    mSettings->setValue("time_limit", val);
}

void SettingsManager::setRunCommand(const QString &lang, const QString &command)
{
    Core::Log::i("settingmanager/setRunCommand") << "lang " << lang << " command " << command << endl;
    if (lang == "Java")
        mSettings->setValue("run_java", command);
    else if (lang == "Python")
        mSettings->setValue("run_python", command);
    else
        mSettings->setValue("run_" + lang, command);
}
void SettingsManager::setCompileCommand(const QString &lang, const QString &command)
{
    Core::Log::i("settingmanager/setCompileCommand") << "lang " << lang << " command " << command << endl;
    if (lang == "C++")
        mSettings->setValue("compile_cpp", command);
    else if (lang == "Java")
        mSettings->setValue("compile_java", command);
    else
        mSettings->setValue("compile_" + lang, command);
}
void SettingsManager::setEditorTheme(const QString &themeName)
{
    Core::Log::i("settingmanager/setEditortheme") << "theme " << themeName << endl;
    mSettings->setValue("editor_theme", themeName);
}
QString SettingsManager::getEditorTheme()
{
    Core::Log::i("settingmanager/getEditorTheme", "Invoked");
    return mSettings->value("editor_theme", "Light").toString();
}
void SettingsManager::setClangFormatBinary(const QString &binary)
{
    Core::Log::i("settingmanager/setClangFormatBinary") << "binary  " << binary << endl;
    mSettings->setValue("clang_format_binary", binary);
}
void SettingsManager::setClangFormatStyle(const QString &style)
{
    Core::Log::i("settingmanager/setclangformatStyle") << "style \n" << style << endl;
    mSettings->setValue("clang_format_style", style);
}
void SettingsManager::setTemplatePath(const QString &lang, const QString &path)
{
    Core::Log::i("settingmanager/setTemplatePath") << " lang " << lang << " path " << path << endl;
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
    Core::Log::i("settingmanager/setRuntimeArgs") << " lang " << lang << " command " << command << endl;
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
    Core::Log::i("settingmanager/setDefaultLang") << " lang " << lang << endl;
    mSettings->setValue("lang", lang);
}
void SettingsManager::setFont(const QString &font)
{
    Core::Log::i("settingmanager/setFont") << "Font is " << font << endl;
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
    Core::Log::i("settingmanager/getHotkeyCompile", "Invoked");
    return QKeySequence::fromString(mSettings->value("hotkey_compile", "").toString());
}
QKeySequence SettingsManager::getHotkeyRun()
{
    Core::Log::i("settingmanager/getHotkeyRun", "Invoked");
    return QKeySequence::fromString(mSettings->value("hotkey_run", "").toString());
}
QKeySequence SettingsManager::getHotkeyCompileRun()
{
    Core::Log::i("settingmanager/getHotkeyCompileRun", "Invoked");
    return QKeySequence::fromString(mSettings->value("hotkey_compile_run", "").toString());
}
QKeySequence SettingsManager::getHotkeyKill()
{
    Core::Log::i("settingmanager/getHotkeyKill", "Invoked");
    return QKeySequence::fromString(mSettings->value("hotkey_kill", "").toString());
}
QKeySequence SettingsManager::getHotkeyFormat()
{
    Core::Log::i("settingmanager/getHotkeyFormat", "Invoked");
    return QKeySequence::fromString(mSettings->value("hotkey_format", "").toString());
}
QKeySequence SettingsManager::getHotkeyViewModeToggler()
{
    Core::Log::i("settingmanager/getHotkeyViewModeToggler", "Invoked");
    return QKeySequence::fromString(mSettings->value("hotkey_mode_toggle", "").toString());
}
QKeySequence SettingsManager::getHotkeySnippets()
{
    Core::Log::i("settingmanager/getHotkeySnippets", "Invoked");
    return QKeySequence::fromString(mSettings->value("hotkey_snippets", "").toString());
}

QString SettingsManager::getSnippet(const QString &lang, const QString &name)
{
    Core::Log::i("settingmanager/getSnippet") << "lang " << lang << " name " << name << endl;
    return mSettings->value("snippets/" + lang + "/" + name, "").toString();
}
void SettingsManager::setSnippet(const QString &lang, const QString &name, const QString &content)
{
    Core::Log::i("settingmanager/setSnippet") << "lang " << lang << " name " << name << " content \n"
                                              << content << endl;
    mSettings->setValue("snippets/" + lang + "/" + name, content);
}
void SettingsManager::removeSnippet(const QString &lang, const QString &name)
{
    Core::Log::i("settingmanager/removeSnippets") << "lang " << lang << " name " << name << endl;
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
    Core::Log::i("settingmanager/getSnippetName") << "getting snippet for " << lang << endl;
    return ret;
}

int SettingsManager::getNumberOfTabs()
{
    Core::Log::i("settingmanager/getNumberOfTabs", "Invoked");
    return mSettings->value("number_of_tabs", 0).toInt();
}
void SettingsManager::setNumberOfTabs(int value)
{
    Core::Log::i("settingmanager/setNumberOfTabs") << "value is " << value << endl;
    mSettings->setValue("number_of_tabs", value);
}
int SettingsManager::getCurrentIndex()
{
    Core::Log::i("settingmanager/getCurrentIndex", "Invoked");
    return mSettings->value("current_index", -1).toInt();
}
void SettingsManager::setCurrentIndex(int index)
{
    Core::Log::i("settingmanager/setCurrentIndex") << "index is : " << index << endl;
    mSettings->setValue("current_index", index);
}
void SettingsManager::clearEditorStatus()
{
    Core::Log::i("settingmanager/clearEditorStatus", "Editor status is being cleared");
    mSettings->remove("editor_status");
}
QMap<QString, QVariant> SettingsManager::getEditorStatus(int index)
{
    Core::Log::i("settingmanager/getEditorStatus", "Invoked");
    return mSettings->value("editor_status/" + QString::number(index)).toMap();
}
void SettingsManager::setEditorStatus(int index, const QMap<QString, QVariant> &status)
{
    Core::Log::i("settingmanager/setEditorStatus", "Editor status is being saved");
    mSettings->setValue("editor_status/" + QString::number(index), status);
}
bool SettingsManager::isHotExitLoadFromFile()
{
    Core::Log::i("settingmanager/getHotExitLoadFromFile", "Invoked");
    return mSettings->value("hot_exit_load_from_file", "false").toBool();
}
void SettingsManager::setHotExitLoadFromFile(bool value)
{
    Core::Log::i("settingmanager/setHotExitLoadFromFile") << "value is : " << value << endl;
    mSettings->setValue("hot_exit_load_from_file", value ? "true" : "false");
}

int SettingsManager::getTransparency()
{
    Core::Log::i("settingmanager/getTransparency", "Invoked");
    return mSettings->value("transparency", 100).toInt();
}

void SettingsManager::setTransparency(int val)
{
    Core::Log::i("settingmanager/setTransparency") << "value is : " << val << endl;
    mSettings->setValue("transparency", val);
}

QString SettingsManager::getCFPath()
{
    Core::Log::i("settingmanager/getCFPath", "Invoked");
    return mSettings->value("cf_path", "cf").toString();
}

void SettingsManager::setCFPath(const QString &path)
{
    Core::Log::i("settingmanager/setCFPath") << "path is : " << path << endl;
    mSettings->setValue("cf_path", path);
}

QString SettingsManager::getSavePath()
{
    Core::Log::i("settingmanager/getSavePath", "Invoked");
    return mSettings->value("save_path").toString();
}

void SettingsManager::setSavePath(const QString &path)
{
    Core::Log::i("settingmanager/setSavePath") << "path is : " << path << endl;
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

void SettingsManager::setHotkeyViewModeToggler(const QKeySequence &sequence)
{
    Core::Log::i("settingmanager/sethotViewModeToggler") << "sequence is : " << sequence.toString() << endl;
    mSettings->setValue("hotkey_mode_toggle", sequence.toString());
}
void SettingsManager::setHotkeyCompile(const QKeySequence &sequence)
{
    Core::Log::i("settingmanager/sethotKeyCompile") << "sequence is : " << sequence.toString() << endl;
    mSettings->setValue("hotkey_compile", sequence.toString());
}
void SettingsManager::setHotkeyRun(const QKeySequence &sequence)
{
    Core::Log::i("settingmanager/sethotKeyRun") << "sequence is : " << sequence.toString() << endl;
    mSettings->setValue("hotkey_run", sequence.toString());
}
void SettingsManager::setHotkeyCompileRun(const QKeySequence &sequence)
{
    Core::Log::i("settingmanager/sethotKeyCompileRun") << "sequence is : " << sequence.toString() << endl;
    mSettings->setValue("hotkey_compile_run", sequence.toString());
}
void SettingsManager::setHotkeyKill(const QKeySequence &sequence)
{
    Core::Log::i("settingmanager/sethotKeyKill") << "sequence is : " << sequence.toString() << endl;
    mSettings->setValue("hotkey_kill", sequence.toString());
}
void SettingsManager::setHotkeyFormat(const QKeySequence &sequence)
{
    Core::Log::i("settingmanager/sethotKeyFormat") << "sequence is : " << sequence.toString() << endl;
    mSettings->setValue("hotkey_format", sequence.toString());
}
void SettingsManager::setHotkeySnippets(const QKeySequence &sequence)
{
    Core::Log::i("settingmanager/sethotKeySnippet") << "sequence is : " << sequence.toString() << endl;
    mSettings->setValue("hotkey_snippets", sequence.toString());
}

ViewMode SettingsManager::getViewMode()
{
    QString strings = mSettings->value("view_mode", "split").toString();
    Core::Log::i("settingmanager/getViewModeSizes") << "view mode is : " << strings << endl;

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

    Core::Log::i("settingmanager/viewMode set to ") << ans << endl;
}

QByteArray SettingsManager::getSplitterSizes()
{
    Core::Log::i("settingmanager/getSplitterSizes", "Invoked");
    return mSettings->value("splitter_sizes").toByteArray();
}

void SettingsManager::setSplitterSizes(const QByteArray &state)
{
    Core::Log::i("settingmanager/setSplittersizes") << "State is : " << state << endl;
    mSettings->setValue("splitter_sizes", state);
}

QByteArray SettingsManager::getRightSplitterSizes()
{
    Core::Log::i("settingmanager/getRightSplitterSizes", "Invoked");
    return mSettings->value("right_splitter_sizes").toByteArray();
}

void SettingsManager::setRightSplitterSizes(const QByteArray &state)
{
    Core::Log::i("settingmanager/setRightSplittersizes") << "State is : " << state << endl;
    mSettings->setValue("right_splitter_sizes", state);
}

QSettings *SettingsManager::settings()
{
    Core::Log::i("settingmanager/settings", "invoked");
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
    Core::Log::i("settingmanager/resetSetting", "mSettings->clear(), invoked");
    mSettings->clear();
}

} // namespace Settings

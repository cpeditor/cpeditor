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

    if (getDefaultLanguage() == "Cpp")
        setDefaultLanguage("C++");

    auto names = getSnippetsNames("Cpp");
    for (auto name : names)
        setSnippet("C++", name, getSnippet("Cpp", name));
    mSettings->remove("snippets/Cpp");
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

void SettingManager::setViewMode(const ViewMode& viewmode)
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
    data.timeLimit = getTimeLimit();
    data.geometry = getGeometry();
    data.font = getFont();
    data.defaultLanguage = getDefaultLanguage();
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

} // namespace Settings

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

#include "Settings/SettingsUpdater.hpp"
#include "Core/SessionManager.hpp"
#include "Settings/SettingsManager.hpp"
#include "generated/SettingsHelper.hpp"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMap>
#include <QSettings>

const static QMap<QString, QString> updateInfo = {{"tab_stop", "Tab Width"},
                                                  {"font", "Editor Font"},
                                                  {"clang_format_binary", "Clang Format/Path"},
                                                  {"clang_format_style", "Clang Format/Style"},
                                                  {"template_cpp", "C++/Template Path"},
                                                  {"compile_cpp", "C++/Compile Command"},
                                                  {"runtime_cpp", "C++/Run Arguments"},
                                                  {"template_java", "Java/Template Path"},
                                                  {"compile_java", "Java/Compile Command"},
                                                  {"runtime_java", "Java/Run Arguments"},
                                                  {"run_java", "Java/Run Command"},
                                                  {"template_java", "Python/Template Path"},
                                                  {"runtime_python", "Python/Run Arguments"},
                                                  {"run_python", "Python/Run Command"},
                                                  {"auto_parenthesis", "Auto Complete Parentheses"},
                                                  {"auto_remove_parentheses", "Auto Remove Parentheses"},
                                                  {"autosave", "Auto Save"},
                                                  {"win_max", "Maximized Window"},
                                                  {"update_start_check", "Check Update"},
                                                  {"format_on_save", "Auto Format"},
                                                  {"transparency", "Opacity"},
                                                  {"splitter_sizes", "Splitter Size"},
                                                  {"right_splitter_sizes", "Right Splitter Size"},
                                                  {"competitive_use", "Competitive Companion/Enable"},
                                                  {"connection_port", "Competitive Companion/Connection Port"},
                                                  {"companion_new_tab", "Competitive Companion/Open New Tab"},
                                                  {"hotkey_format", "Hotkey/Format"},
                                                  {"hotkey_kill", "Hotkey/Kill"},
                                                  {"hotkey_compile_run", "Hotkey/Compile Run"},
                                                  {"hotkey_run", "Hotkey/Run"},
                                                  {"hotkey_compile", "Hotkey/Compile"},
                                                  {"hotkey_mode_toggle", "Hotkey/Change View Mode"},
                                                  {"hotkey_snippets", "Hotkey/Snippets"},
                                                  {"use_hot_exit", "Hot Exit/Enable"},
                                                  {"cf_path", "CF/Path"},
                                                  {"compile_and_run_only", "Show Compile And Run Only"},
                                                  {"language", "Locale"}};

void SettingsUpdater::updateSetting(QSettings &setting)
{
    for (const auto &old : updateInfo.keys())
    {
        QString nw = updateInfo[old];
        if (!SettingsManager::contains(nw) && setting.contains(old))
        {
            SettingsManager::set(nw, setting.value(old));
        }
    }

    if (setting.childGroups().contains("snippets"))
    {
        setting.beginGroup("snippets");
        auto langs = setting.childGroups();
        for (const QString &lang : langs)
        {
            setting.beginGroup(lang);
            auto obj = SettingsHelper::getLanguageConfig(lang == "Cpp" ? "C++" : lang);
            QStringList used = obj.getSnippets();
            for (const QString &key : setting.childKeys())
            {
                if (!used.contains(key))
                    obj.setSnippet(key, setting.value(key).toString());
            }
            setting.endGroup();
        }
        setting.endGroup();
    }

    if (setting.childGroups().contains("editor_status") && Core::SessionManager::lastSessionPath().isEmpty())
    {
        QJsonObject json;
        json.insert("currentIndex", setting.value("hot_exit/current_index").toInt());

        QJsonArray arr;

        setting.beginGroup("editor_status");
        auto indices = setting.childKeys();
        for (const auto &index : indices)
        {
            arr.push_back(QJsonDocument::fromVariant(setting.value(index).toMap()).object());
        }
        setting.endGroup();

        json.insert("tabs", arr);

        Core::SessionManager::saveSession(QJsonDocument::fromVariant(json.toVariantMap()).toJson());
    }
}

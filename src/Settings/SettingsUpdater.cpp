/*
 * Copyright (C) 2019-2021 Ashar Khan <ashar786khan@gmail.com>
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
#include <QDebug>
#include <QSet>
#include <QSettings>

void SettingsUpdater::updateSetting()
{
}

void SettingsUpdater::updateSetting_INI(QSettings &setting)
{
#ifdef QT_DEBUG
    // Check for key conflicts
    QSet<QString> keys;

    auto addKey = [&](const QString &key) {
        if (keys.contains(key))
            qFatal("Duplicate key in the settings: %s", // NOLINT: C-Style varargs should not be called.
                   key.toStdString().c_str());
        keys.insert(key);
    };

    for (const auto &si : qAsConst(SettingsInfo::settings))
    {
        addKey(si.name);
    }
#endif
    // Remove all fixes.

#define QAS(o, n, t)                                                                                                   \
    if (setting.contains(o))                                                                                           \
        c.set##n(setting.value(o).value<t>());

    QMap<QString, QString> formatters{{"clang_format", "Clang Format"}, {"yapf", "YAPF"}};
    for (const auto &key : formatters.keys())
    {
        if (setting.childGroups().contains(key))
        {
            setting.beginGroup(key);
            auto c = SettingsHelper::getFormatter(formatters[key]);
            QAS("program", Program, QString)
            QAS("style", Style, QString)
            QAS("arguments", Arguments, QString)
            setting.endGroup();
        }
    }

    QMap<QString, QString> langs{{"cpp", "C++"}, {"java", "Java"}, {"python", "Python"}};
    for (const auto &key : langs.keys())
    {
        if (setting.childGroups().contains(key))
        {
            auto c = SettingsHelper::getLanguageConfig(langs[key]);
            setting.beginGroup(key);
            QAS("template_path", TemplatePath, QString)
            QAS("template_cursor_position_regex", TemplateCursorPositionRegex, QString)
            QAS("template_cursor_position_offset_type", TemplateCursorPositionOffsetType, QString)
            QAS("template_cursor_position_offset_characters", TemplateCursorPositionOffsetCharacters, int)
            QAS("run_arguments", RunArguments, QString)
            if (key != "python")
            {
                QAS("compile_command", CompileCommand, QString)
                QAS("compiler_output_codec", CompilerOutputCodec, QString)
                QAS("output_path", OutputPath, QString)
            }
            if (key != "cpp")
            {
                QAS("run_command", RunCommand, QString)
            }
            if (key == "java")
            {
                QAS("class_name", ClassName, QString)
            }
            if (setting.contains("parentheses"))
            {
                auto l = setting.value("parentheses").toList();
                QStringList ks;
                for (const auto &v : l)
                {
                    auto vl = v.toList();
                    if (vl.size() != 5)
                        continue;
                    auto b = vl[0].toChar();
                    auto e = vl[1].toChar();
                    ks.push_back(QString("%1%2").arg(b).arg(e));
                    auto ac = vl[2].toInt();
                    auto ar = vl[3].toInt();
                    auto tj = vl[4].toInt();
                    auto p = c.getParentheses(ks.back());
                    p.setAutoComplete(ac);
                    p.setAutoRemove(ar);
                    p.setTabJumpOut(tj);
                }
                c.setParentheses(ks);
            }
            setting.endGroup();
        }
        if (setting.childGroups().contains("lsp"))
        {
            setting.beginGroup("lsp");
            auto l = SettingsHelper::getLSP(langs[key]);
#define T(o, n, t)                                                                                                     \
    if (setting.contains(o "_" + key))                                                                                 \
        l.set##n(setting.value(o "_" + key).value<t>());

            T("path", Path, QString)
            T("use_linting", UseLinting, bool)
            T("use_autocomplete", UseAutocomplete, bool)
            T("delay", Delay, int)
            T("args", Args, QString)
            setting.endGroup();
        }
    }

    if (setting.childGroups().contains("default_path"))
    {
        setting.beginGroup("default_path");
        if (setting.contains("names_and_paths"))
        {
            auto l = setting.value("names_and_paths").toList();
            QStringList ks;
            for (const auto &v : l)
            {
                auto vl = v.toList();
                if (vl.size() != 2)
                    continue;
                auto k = vl[0].toString();
                auto p = vl[1].toString();
                ks.push_back(k);
                SettingsHelper::getDefaultPath(k).setPath(p);
            }
            SettingsHelper::setDefaultPath(ks);
        }
        if (setting.childGroups().contains("action"))
        {
            setting.beginGroup("action");
            QMap<QString, QString> acts = {
                {"open_file", "Open File"},
                {"save_file", "Save File"},
                {"open_contest", "Open Contest"},
                {"load_single_test_case", "Load Single Test Case"},
                {"add_pairs_of_test_cases", "Add Pairs Of Test Cases"},
                {"save_test_case_to_a_file", "Save Test Case To A File"},
                {"custom_checker", "Custom Checker"},
                {"export_and_import_settings", "Export And Import Settings"},
                {"export_and_load_session", "Export And Load Session"},
                {"extract_and_load_snippets", "Extract And Load Snippets"},
            };
            for (const auto &key : acts.keys())
            {
                if (setting.contains(key + "/uses"))
                    SettingsHelper::getDefaultPathAction(acts[key]).setUses(setting.value(key + "/uses").toString());
                if (setting.contains(key + "/changes"))
                    SettingsHelper::getDefaultPathAction(acts[key]).setChanges(
                        setting.value(key + "/changes").toString());
            }
            setting.endGroup();
        }
        setting.endGroup();
    }
}

void SettingsUpdater::updateSettingFinal()
{
}

/*
 * Copyright (C) 2019-2020 Ashar Khan <ashar786khan@gmail.com>
 *
 * This file is part of cpeditor.
 *
 * cpeditor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * I will not be responsible if cpeditor behaves in unexpected way and
 * causes your ratings to go down and or lose any important contest.
 *
 * Believe Software is "Software" and it isn't immune to bugs.
 *
 */

#include "Util.hpp"
#include "Core/EventLogger.hpp"
#include "Core/SettingsManager.hpp"
#include "Extensions/EditorTheme.hpp"
#include <QFile>
#include <QSaveFile>

namespace Util
{

bool saveFile(const QString &path, const QString &content, const QString &head, bool safe, MessageLogger *log)
{
    if (safe)
    {
        QSaveFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            Core::Log::e("Util/saveFile/safe/" + head) << "Failed to open [" << path << "]" << endl;
            if (log != nullptr)
                log->error(head, "Failed to open [" + path + "]. Do I have write permission?");
            return false;
        }
        file.write(content.toUtf8());
        if (!file.commit())
        {
            Core::Log::e("Util/saveFile/safe/" + head) << "Failed to save to [" << path << "]" << endl;
            if (log != nullptr)
                log->error(head, "Failed to save to [" + path + "]. Do I have write permission?");
            return false;
        }
    }
    else
    {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            Core::Log::e("Util/saveFile/unsafe/" + head) << "Failed to open [" << path << "]" << endl;
            if (log != nullptr)
                log->error(head, "Failed to open [" + path + "]. Do I have write permission?");
            return false;
        }
        if (file.write(content.toUtf8()) == -1)
        {
            Core::Log::e("Util/saveFile/unsafe/" + head) << "Failed to save to [" << path << "]" << endl;
            if (log != nullptr)
                log->error(head, "Failed to save to [" + path + "]. Do I have write permission?");
            return false;
        }
    }
    Core::Log::i("Util/saveFile/" + head) << "Successfully saved to [" << path << "]" << endl;
    return true;
}

void applySettingsToEditor(QCodeEditor *editor)
{
    Core::Log::i("Util/applySettingsToEditor", "Invoked");

    editor->setTabReplace(Settings::SettingsManager::isTabsReplaced());
    editor->setTabReplaceSize(Settings::SettingsManager::getTabStop());
    editor->setAutoIndentation(Settings::SettingsManager::isAutoIndent());
    editor->setAutoParentheses(Settings::SettingsManager::isAutoParentheses());
    editor->setAutoRemoveParentheses(Settings::SettingsManager::isAutoRemoveParentheses());

    if (!Settings::SettingsManager::getFont().isEmpty())
    {
        QFont font;
        font.fromString(Settings::SettingsManager::getFont());
        editor->setFont(font);
    }

    const int tabStop = Settings::SettingsManager::getTabStop();
    QFontMetrics metric(editor->font());
    editor->setTabReplaceSize(tabStop);

    if (Settings::SettingsManager::isWrapText())
        editor->setWordWrapMode(QTextOption::WordWrap);
    else
        editor->setWordWrapMode(QTextOption::NoWrap);

    if (Settings::SettingsManager::getEditorTheme() == "Light")
        editor->setSyntaxStyle(Themes::EditorTheme::getLightTheme());
    else if (Settings::SettingsManager::getEditorTheme() == "Drakula")
        editor->setSyntaxStyle(Themes::EditorTheme::getDrakulaTheme());
    else if (Settings::SettingsManager::getEditorTheme() == "Monkai")
        editor->setSyntaxStyle(Themes::EditorTheme::getMonkaiTheme());
    else if (Settings::SettingsManager::getEditorTheme() == "Solarised")
        editor->setSyntaxStyle(Themes::EditorTheme::getSolarisedTheme());
    else if (Settings::SettingsManager::getEditorTheme() == "Solarised Dark")
        editor->setSyntaxStyle(Themes::EditorTheme::getSolarisedDarkTheme());
    else
        editor->setSyntaxStyle(Themes::EditorTheme::getLightTheme());
}

} // namespace Util
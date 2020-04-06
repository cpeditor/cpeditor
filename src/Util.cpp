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

#include "Util.hpp"
#include "Core/EventLogger.hpp"
#include "Core/SettingsHelper.hpp"
#include "Extensions/EditorTheme.hpp"
#include <QFile>
#include <QSaveFile>

namespace Util
{

QString fileNameFilter(bool cpp, bool java, bool python)
{
    QString filter;

    if (cpp)
        filter += " *." + cppSuffix.join(" *.");
    if (java)
        filter += " *." + javaSuffix.join(" *.");
    if (python)
        filter += " *." + pythonSuffix.join(" *.");

    return "Source Files (" + filter.trimmed() + ")";
}

bool saveFile(const QString &path, const QString &content, const QString &head, bool safe, MessageLogger *log)
{
    if (safe && !SettingsHelper::isSaveFaster())
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

    editor->setTabReplace(SettingsHelper::isReplaceTabs());
    editor->setTabReplaceSize(SettingsHelper::getTabWidth());
    editor->setAutoIndentation(SettingsHelper::isAutoIndent());
    editor->setAutoParentheses(SettingsHelper::isAutoCompleteParentheses());
    editor->setAutoRemoveParentheses(SettingsHelper::isAutoRemoveParentheses());

    editor->setFont(SettingsHelper::getFont());

    const int tabStop = SettingsHelper::getTabWidth();
    QFontMetrics metric(editor->font());
    editor->setTabReplaceSize(tabStop);

    if (SettingsHelper::isWrapText())
        editor->setWordWrapMode(QTextOption::WordWrap);
    else
        editor->setWordWrapMode(QTextOption::NoWrap);

    if (SettingsHelper::getEditorTheme() == "Light")
        editor->setSyntaxStyle(Themes::EditorTheme::getLightTheme());
    else if (SettingsHelper::getEditorTheme() == "Drakula")
        editor->setSyntaxStyle(Themes::EditorTheme::getDrakulaTheme());
    else if (SettingsHelper::getEditorTheme() == "Monkai")
        editor->setSyntaxStyle(Themes::EditorTheme::getMonkaiTheme());
    else if (SettingsHelper::getEditorTheme() == "Solarised")
        editor->setSyntaxStyle(Themes::EditorTheme::getSolarisedTheme());
    else if (SettingsHelper::getEditorTheme() == "Solarised Dark")
        editor->setSyntaxStyle(Themes::EditorTheme::getSolarisedDarkTheme());
    else
        editor->setSyntaxStyle(Themes::EditorTheme::getLightTheme());
}

} // namespace Util
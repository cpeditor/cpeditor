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
#include "Extensions/EditorTheme.hpp"
#include <QCXXHighlighter>
#include <QFile>
#include <QJavaHighlighter>
#include <QPythonHighlighter>
#include <QSaveFile>
#include <generated/SettingsHelper.hpp>

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
            if (log != nullptr)
                log->error(head, "Failed to open [" + path + "]. Do I have write permission?");
            else
                LOG_ERR("Failed to open [" << path << "]");
            return false;
        }
        file.write(content.toUtf8());
        if (!file.commit())
        {
            if (log != nullptr)
                log->error(head, "Failed to save to [" + path + "]. Do I have write permission?");
            else
                LOG_ERR("Failed to save to [" << path << "]");
            return false;
        }
    }
    else
    {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            if (log != nullptr)
                log->error(head, "Failed to open [" + path + "]. Do I have write permission?");
            else
                LOG_ERR("unsafe: Failed to open [" << path << "]");
            return false;
        }
        if (file.write(content.toUtf8()) == -1)
        {
            if (log != nullptr)
                log->error(head, "Failed to save to [" + path + "]. Do I have write permission?");
            else
                LOG_ERR("unsafe: Failed to save to [" << path << "]");
            return false;
        }
    }
    LOG_INFO("Successfully saved to [" << path << "]");
    return true;
}

QString readFile(const QString &path, const QString &head, MessageLogger *log)
{
    if (!QFile::exists(path))
        return QString();
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        if (log != nullptr)
            log->error(head, QString("Failed to open [%1]. Do I have read permission?").arg(path));
        else
            LOG_WARN(QString("Failed to open [%1]").arg(path));
        return QString();
    }
    QString content = file.readAll();
    if (content.isNull())
        return "";
    return content;
}

void applySettingsToEditor(QCodeEditor *editor)
{
    LOG_INFO("Applying settings to QCodeEditor");

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
        editor->setSyntaxStyle(Extensions::EditorTheme::getLightTheme());
    else if (SettingsHelper::getEditorTheme() == "Dracula")
        editor->setSyntaxStyle(Extensions::EditorTheme::getDraculaTheme());
    else if (SettingsHelper::getEditorTheme() == "Monokai")
        editor->setSyntaxStyle(Extensions::EditorTheme::getMonokaiTheme());
    else if (SettingsHelper::getEditorTheme() == "Solarized")
        editor->setSyntaxStyle(Extensions::EditorTheme::getSolarizedTheme());
    else if (SettingsHelper::getEditorTheme() == "Solarized Dark")
        editor->setSyntaxStyle(Extensions::EditorTheme::getSolarizedDarkTheme());
    else
        editor->setSyntaxStyle(Extensions::EditorTheme::getLightTheme());
}

void setEditorLanguage(QCodeEditor *editor, const QString &language)
{
    if (language == "Python")
    {
        editor->setHighlighter(new QPythonHighlighter);
        editor->setCompleter(nullptr);
    }
    else if (language == "Java")
    {
        editor->setHighlighter(new QJavaHighlighter);
        editor->setCompleter(nullptr);
    }
    else
    {
        editor->setHighlighter(new QCXXHighlighter);
        editor->setCompleter(nullptr);
    }
}

QPalette windowsDarkThemePalette()
{
    QPalette darkPalette;
    QColor darkColor = QColor(45, 45, 45);
    QColor disabledColor = QColor(127, 127, 127);
    darkPalette.setColor(QPalette::Window, darkColor);
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(18, 18, 18));
    darkPalette.setColor(QPalette::AlternateBase, darkColor);
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Disabled, QPalette::Text, disabledColor);
    darkPalette.setColor(QPalette::Button, darkColor);
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, disabledColor);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);
    darkPalette.setColor(QPalette::Disabled, QPalette::HighlightedText, disabledColor);

    return darkPalette;
}

} // namespace Util
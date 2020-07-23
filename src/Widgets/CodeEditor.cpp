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

#include "Widgets/CodeEditor.hpp"
#include "Core/EventLogger.hpp"
#include "Core/KSyntaxHighlightingRepository.hpp"
#include "Extensions/EditorTheme.hpp"
#include "Settings/SettingsManager.hpp"
#include "generated/SettingsHelper.hpp"
#include <QSyntaxStyle>
#include <definition.h>
#include <syntaxhighlighter.h>
#include <theme.h>

CodeEditor::CodeEditor(QWidget *parent) : QCodeEditor(parent)
{
    highlighter = new KSyntaxHighlighting::SyntaxHighlighter(document());
}

void CodeEditor::applySettings(const QString &language)
{
    LOG_INFO("Applying settings to a CodeEditor");

    setTabReplace(SettingsHelper::isReplaceTabs());
    setTabReplaceSize(SettingsHelper::getTabWidth());
    setAutoIndentation(SettingsHelper::isAutoIndent());

    setFont(SettingsHelper::getEditorFont());

    const int tabStop = SettingsHelper::getTabWidth();
    QFontMetrics metric(font());
    setTabReplaceSize(tabStop);

    if (SettingsHelper::isWrapText())
        setWordWrapMode(QTextOption::WordWrap);
    else
        setWordWrapMode(QTextOption::NoWrap);

    auto style = Extensions::EditorTheme::query(SettingsHelper::getEditorTheme());
    if (!style)
        style = Extensions::EditorTheme::query("Light");
    setSyntaxStyle(style);

    setExtraBottomMargin(SettingsHelper::isExtraBottomMargin());

    if (language.isEmpty())
        return;

    highlighter->setDefinition(
        KSyntaxHighlightingRepository::getSyntaxHighlightingRepository()->definitionForName(language));
    if (SettingsHelper::getSyntaxHighlightingStyle() == "Default")
    {
        if (KSyntaxHighlightingRepository::themeNames().contains(SettingsHelper::getEditorTheme()))
            highlighter->setTheme(KSyntaxHighlightingRepository::getSyntaxHighlightingRepository()->theme(
                SettingsHelper::getEditorTheme()));
        else if (style->getFormat("Text").background().color().lightness() <= 127)
            highlighter->setTheme(KSyntaxHighlightingRepository::getSyntaxHighlightingRepository()->defaultTheme(
                KSyntaxHighlighting::Repository::DarkTheme));
        else
            highlighter->setTheme(KSyntaxHighlightingRepository::getSyntaxHighlightingRepository()->defaultTheme(
                KSyntaxHighlighting::Repository::LightTheme));
    }
    else
    {
        highlighter->setTheme(KSyntaxHighlightingRepository::getSyntaxHighlightingRepository()->theme(
            SettingsHelper::getSyntaxHighlightingStyle()));
    }
    highlighter->rehighlight();

    QVector<QCodeEditor::Parenthesis> parentheses;

    auto list = SettingsManager::get(language + "/Parentheses").toList();

    for (auto var : list)
    {
        auto li = var.toList();
        if (li.length() != 5)
        {
            LOG_ERR(INFO_OF(li.length()));
            continue;
        }

        auto left = li[0].toChar();
        auto right = li[1].toChar();

        auto getFlag = [](Qt::CheckState state, bool def) {
            switch (state)
            {
            case Qt::Checked:
                return true;
            case Qt::PartiallyChecked:
                return def;
            case Qt::Unchecked:
                return false;
            default:
                Q_UNREACHABLE();
            }
        };

        bool autoComplete = getFlag(Qt::CheckState(li[2].toInt()), SettingsHelper::isAutoCompleteParentheses());
        bool autoRemove = getFlag(Qt::CheckState(li[3].toInt()), SettingsHelper::isAutoRemoveParentheses());
        bool tabJumpOut = getFlag(Qt::CheckState(li[4].toInt()), SettingsHelper::isTabJumpOutParentheses());

        parentheses.push_back({left, right, autoComplete, autoRemove, tabJumpOut});
    }

    setParentheses(parentheses);
}

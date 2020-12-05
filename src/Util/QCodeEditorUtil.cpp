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

#include "Util/QCodeEditorUtil.hpp"
#include "Core/EventLogger.hpp"
#include "Extensions/EditorTheme.hpp"
#include "Settings/SettingsManager.hpp"
#include "generated/SettingsHelper.hpp"
#include "third_party/FakeVim/fakevim/fakevimactions.h"
#include <QCXXHighlighter>
#include <QCodeEditor>
#include <QJavaHighlighter>
#include <QPythonHighlighter>

namespace Util
{
void applySettingsToEditor(QCodeEditor *editor, const QString &language)
{
    LOG_INFO("Applying settings to QCodeEditor");

    editor->setFont(SettingsHelper::getEditorFont());

    if (SettingsHelper::isWrapText())
        editor->setWordWrapMode(QTextOption::WordWrap);
    else
        editor->setWordWrapMode(QTextOption::NoWrap);

    editor->setHighlightCurrentLine(SettingsHelper::isHighlightCurrentLine());

    auto *style = Extensions::EditorTheme::query(SettingsHelper::getEditorTheme());
    if (!style)
        style = Extensions::EditorTheme::query("Light");
    editor->setSyntaxStyle(style);

    editor->setExtraBottomMargin(SettingsHelper::isExtraBottomMargin());

    if (language.isEmpty())
        return;

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

    if (editor->vimCursor())
        return;

    QVector<QCodeEditor::Parenthesis> parentheses;

    auto list = SettingsManager::get(language + "/Parentheses").toList();

    for (auto const &var : list)
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

    editor->setParentheses(parentheses);

    editor->setTabReplace(SettingsHelper::isReplaceTabs());
    editor->setAutoIndentation(SettingsHelper::isAutoIndent());

    const int tabStop = SettingsHelper::getTabWidth();
    QFontMetrics metric(editor->font());
    editor->setTabReplaceSize(tabStop);
}

} // namespace Util

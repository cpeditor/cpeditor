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

#include "Util/QCodeEditorUtil.hpp"
#include "Core/EventLogger.hpp"
#include "Extensions/EditorTheme.hpp"
#include "Settings/SettingsManager.hpp"
#include "generated/SettingsHelper.hpp"
#include <QCXXHighlighter>
#include <QCodeEditor>
#include <QJavaHighlighter>
#include <QPythonHighlighter>

namespace Util
{
void applySettingsToEditor(QCodeEditor *editor, const QString &language)
{
    LOG_INFO("Applying settings to QCodeEditor");

    editor->setCursorWidth(SettingsHelper::getCursorWidth());
    editor->setTabReplace(SettingsHelper::isReplaceTabs());
    editor->setTabReplaceSize(SettingsHelper::getTabWidth());
    editor->setAutoIndentation(SettingsHelper::isAutoIndent());

    editor->setFont(SettingsHelper::getEditorFont());

    const int tabStop = SettingsHelper::getTabWidth();
    QFontMetrics metric(editor->font());
    editor->setTabReplaceSize(tabStop);

    if (SettingsHelper::isWrapText())
        editor->setWordWrapMode(QTextOption::WordWrap);
    else
        editor->setWordWrapMode(QTextOption::NoWrap);

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

    QVector<QCodeEditor::Parenthesis> parentheses;

    auto list = SettingsHelper::getLanguageConfig(language).queryParentheses();

    for (const auto &key : list)
    {
        if (key.length() != 2)
        {
            LOG_ERR(INFO_OF(key.length()));
            continue;
        }
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
        const auto &cfg = SettingsHelper::getLanguageConfig(language).getParentheses(key);
        bool autoComplete =
            getFlag(static_cast<Qt::CheckState>(cfg.getAutoComplete()), SettingsHelper::isAutoCompleteParentheses());
        bool autoRemove =
            getFlag(static_cast<Qt::CheckState>(cfg.getAutoRemove()), SettingsHelper::isAutoRemoveParentheses());
        bool tabJumpOut =
            getFlag(static_cast<Qt::CheckState>(cfg.getTabJumpOut()), SettingsHelper::isTabJumpOutParentheses());

        parentheses.push_back({key[0], key[1], autoComplete, autoRemove, tabJumpOut});
    }

    editor->setParentheses(parentheses);
}
} // namespace Util

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

#ifndef CODEEDITOR_HPP
#define CODEEDITOR_HPP

#include <QPlainTextEdit>
#include <QTextEdit>

namespace KSyntaxHighlighting
{
class SyntaxHighlighter;
}

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

  public:
    enum class SeverityLevel
    {
        Hint,
        Information,
        Warning,
        Error
    };

    struct Parenthesis
    {
        QChar left, right;
        bool autoComplete, autoRemove, tabJumpOut;

        Parenthesis(const QChar &l = '(', const QChar &r = ')', bool complete = true, bool remove = true,
                    bool jumpout = true)
            : left(l), right(r), autoComplete(complete), autoRemove(remove), tabJumpOut(jumpout)
        {
        }
    };

    /**
     * @brief Constructor.
     * @param widget Pointer to parent widget.
     */
    explicit CodeEditor(QWidget *widget = nullptr);

    /**
     * @brief Method for getting first visible block
     * index.
     * @return Index.
     */
    int getFirstVisibleBlock();

    /**
     * @brief squiggle Puts a underline squiggle under text ranges in Editor
     * @param level defines the color of the underline depending upon the severity
     * @param tooltipMessage The tooltip hover message to show when over selection.
     * @note QPair<int, int>: first -> Line number in 1-based indexing
     *                        second -> Character number in 0-based indexing
     */
    void squiggle(SeverityLevel level, QPair<int, int>, QPair<int, int>, QString tooltipMessage);

    /**
     * @brief clearSquiggle, Clears complete squiggle from editor
     */
    void clearSquiggle();

    void applySettings(const QString &lang);

  Q_SIGNALS:
    /**
     * @brief Signal, the font is changed by the wheel event.
     */
    void fontChanged(const QFont &newFont);

  public Q_SLOTS:

    /**
     * @brief Slot, that will proceed extra selection
     * for current cursor position.
     */
    void updateExtraSelection1();
    void updateExtraSelection2();

    /**
     * @brief Slot, that indent the selected lines.
     */
    void indent();

    /**
     * @brief Slot, that unindent the selected lines.
     */
    void unindent();

    /**
     * @brief Slot, that swap the selected lines up.
     */
    void swapLineUp();

    /**
     * @brief Slot, that swap the selected lines down.
     */
    void swapLineDown();

    /**
     * @brief Slot, that delete the selected lines.
     */
    void deleteLine();

    /**
     * @brief Slot, that duplicate the current line or the selection.
     */
    void duplicate();

    /**
     * @brief Slot, that toggle single line comment of the
     * selected lines.
     */
    void toggleComment();

    /**
     * @brief Slot, that toggle block comment of the selection.
     */
    void toggleBlockComment();

  protected:
    /**
     * @brief Method, that's called on any text insertion of
     * mimedata into editor. If it's text - it inserts text
     * as plain text.
     */
    void insertFromMimeData(const QMimeData *source) override;

    /**
     * @brief Method, that's called on any widget resize.
     * This method if overloaded for line number area
     * resizing.
     */
    void resizeEvent(QResizeEvent *e) override;

    /**
     * @brief Method, update the bottom margin when the font changes.
     */
    void changeEvent(QEvent *e) override;

    /**
     * @brief Method, update the font size when the wheel is rotated with Ctrl pressed
     */
    void wheelEvent(QWheelEvent *e) override;

    /**
     * @brief Method, that's called on any key press, posted
     * into code editor widget. This method is overloaded for:
     *
     * 1. Completion
     * 2. Tab to spaces
     * 3. Low indentation
     * 4. Auto parenthesis
     */
    void keyPressEvent(QKeyEvent *e) override;

    /**
     * @brief Method for tooltip generation
     */
    bool event(QEvent *e) override;

  private Q_SLOTS:
    /**
     * @brief Slot, that updates the bottom margin.
     */
    void updateBottomMargin();

  private:
    /**
     * @brief Method for getting character under
     * cursor.
     * @param offset Offset to cursor.
     */
    QChar charUnderCursor(int offset = 0) const;

    /**
     * @brief Method, that adds highlighting of
     * currently selected line to extra selection list.
     */
    void highlightCurrentLine();

    /**
     * @brief Method, that adds highlighting of
     * parenthesis if available.
     */
    void highlightParenthesis();

    void highlightOccurrences();

    /**
     * @brief Method for remove the first group of regex
     * in each line of the selection.
     * @param regex remove its first group
     * @param force if true, remove regardless of whether
     * all lines are begun with regex; if false remove
     * only when all lines are begun with regex.
     * @return if regex is removed
     */
    bool removeInEachLineOfSelection(const QRegularExpression &regex, bool force);

    /**
     * @brief Method for add the str at the begin of regex
     * in each line of the selection.
     * @param regex add at the begin of its match
     * @param str string to add
     */
    void addInEachLineOfSelection(const QRegularExpression &regex, const QString &str);

    /**
     * @brief The SquiggleInformation struct, Line number will be index of vector+1;
     */
    struct SquiggleInformation
    {
        SquiggleInformation() = default;

        SquiggleInformation(QPair<int, int> start, QPair<int, int> stop, QString text)
            : m_startPos(start), m_stopPos(stop), m_tooltipText(text)
        {
        }

        QPair<int, int> m_startPos;
        QPair<int, int> m_stopPos;
        QString m_tooltipText;
    };

    QList<QTextEdit::ExtraSelection> extra1, extra2, extra_squiggles;

    QString m_tabReplace;

    QVector<SquiggleInformation> m_squiggler;

    QVector<Parenthesis> parentheses;

    KSyntaxHighlighting::SyntaxHighlighter *highlighter;

    QString language;
};

#endif // CODEEDITOR_HPP

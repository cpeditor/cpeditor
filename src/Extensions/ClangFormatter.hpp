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

/*
 * The Formatter is used to format codes.
 * It runs synchronously, so the event loop is blocked while formatting.
 * The time limit for formatting is 2 seconds.
 */

#ifndef FORMATTER_HPP
#define FORMATTER_HPP

#include <QPair>
#include <QString>

class MessageLogger;
class QCodeEditor;
class QStringList;

namespace Extensions
{
class ClangFormatter
{
  public:
    /**
     * @brief construct a Formatter
     * @param clangFormatBinary the path to the Clang Format binary used for formatting
     * @param clangFormatStyle the Clang Format style used for formatting
     * @param logger the message logger used for displaying messages
     */
    ClangFormatter(const QString &clangFormatBinary, const QString &clangFormatStyle, MessageLogger *logger);

    /**
     * @brief format the codes in the given editor
     * @param editor the editor to be formatted
     * @param filePath the file path of the file to be formatted
     * @param lang the language to be formatted
     * @param selectionOnly whether to format the selection only if there is one, doesn't matter if there is no
     *        selection
     */
    void format(QCodeEditor *editor, const QString &filePath, const QString &lang, bool selectionOnly);

    /**
     * @brief check whether the given settings are valid
     * @param checkBinary the Clang Format binary to be checked
     * @param checkStyle the Clang Format style to be checked
     */
    static bool check(const QString &checkBinary, const QString &checkStyle);

    /**
     * @brief update the Clang Format binary of a checker
     * @param newBinary the path to the new Clang Format binary to use
     */
    void updateBinary(const QString &newBinary);

    /**
     * @brief update the Clang Format style of a checker
     * @param newStyle the new Clang Format style to use
     */
    void updateStyle(const QString &newStyle);

  private:
    /**
     * @brief get the format result
     * @param args the command line arguments passed to Clang Format
     * @return a QPair, the first element is the new cursor position, the second element is the new text
     *         if the formatting failed, it will return {-1, QString()}
     */
    QPair<int, QString> getFormatResult(const QStringList &args);

    QString binary;     // the path to the Clang Format binary
    QString style;      // the Clang Format style
    MessageLogger *log; // the message logger to display messages
};

} // namespace Extensions
#endif // FORMATTER_HPP

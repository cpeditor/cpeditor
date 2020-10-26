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

#ifndef YAPF_FORMATTER_HPP
#define YAPF_FORMATTER_HPP

#include <QObject>

class MessageLogger;
class QCodeEditor;

namespace Extensions
{
class YAPFormatter : public QObject
{
    Q_OBJECT

  public:
    /**
     * @brief construct a Formatter
     * @param yapFormatBinary the path to the YAP Format binary used for formatting
     * @param yapArgument the arguments passed to binary to invoke the yapf
     * @param yapFormatStyle the YAP Format style used for formatting
     * @param logger the message logger used for displaying messages
     * @param parent the parent of the QObject
     */
    YAPFormatter(const QString &yapFormatBinary, const QString &yapArgument, const QString &yapFormatStyle,
                          MessageLogger *logger, QObject *parent = nullptr);

    /**
     * @brief format the codes in the given editor
     * @param editor the editor to be formatted
     * @param filePath the file path of the file to be formatted
     * @param lang the language to be formatted
     * @param selectionOnly whether to format the selection only if there is one, doesn't matter if there is no
     *        selection
     * @param verbose if verbose, "Formatting completed" will be logged even if the code is unchanged after formatting
     */
    void format(QCodeEditor *editor, const QString &filePath, bool selectionOnly, bool verbose);

    /**
     * @brief update the Python binary of a checker
     * @param newBinary the path to the new YAP Format binary to use
     */
    void updateBinary(const QString &newBinary);

    /**
     * @brief update the YAP Format binary of a checker
     * @param newBinary the path to the new YAP Format binary to use
     */
	void updateArguments(const QString &newArgs);

    /**
     * @brief update the YAP Format style of a checker
     * @param newStyle the new YAP Format style to use
     */
    void updateStyle(const QString &newStyle);

  private:
    /**
     * @brief get the format result
     * @param args the command line arguments passed to YAP Format
     * @return a QString, the new text, if the formatting failed, it will return QString()
     */
    QString getFormatResult(const QStringList &args);

    QString binary;     // the path to the Python binary
    QString style;      // the YAP Format style
	QString args;  		// the Argument to start YAP

    MessageLogger *log; // the message logger to display messages
};

} // namespace Extensions
#endif // FORMATTER_HPP

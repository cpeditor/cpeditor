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
 * The MessageLogger is used to send messages to the user directly in the GUI.
 */

#ifndef MESSAGELOGGER_HPP
#define MESSAGELOGGER_HPP

#include <QTextBrowser>

class PreferencesWindow;

class MessageLogger : public QTextBrowser
{
    Q_OBJECT

  public:
    explicit MessageLogger(PreferencesWindow *preferences, QWidget *parent = nullptr);

    /**
     * @brief show a message
     * @param head the head of the message, indicates where the message is from
     * @param body the main part of the message
     * @param color the color of the message, use the default color if this parameter is empty
     * @param htmlEscaped convert the message to the HTML escaped format to avoid hidden "<>" and wrong spaces
     */
    void message(const QString &head, const QString &body, const QString &color, bool htmlEscaped = true);

    /**
     * @brief show a warning message
     * @param head the head of the message, indicates where the message is from
     * @param body the main part of the message
     * @param htmlEscaped convert the message to the HTML escaped format to avoid hidden "<>" and wrong spaces
     * @note the message is green
     */
    void warn(const QString &head, const QString &body, bool htmlEscaped = true);

    /**
     * @brief show a infomation message
     * @param head the head of the message, indicates where the message is from
     * @param body the main part of the message
     * @param htmlEscaped convert the message to the HTML escaped format to avoid hidden "<>" and wrong spaces
     * @note the message is in the default color
     */
    void info(const QString &head, const QString &body, bool htmlEscaped = true);

    /**
     * @brief show a error message
     * @param head the head of the message, indicates where the message is from
     * @param body the main part of the message
     * @param htmlEscaped convert the message to the HTML escaped format to avoid hidden "<>" and wrong spaces
     * @note the message is red
     */
    void error(const QString &head, const QString &body, bool htmlEscaped = true);

  private slots:
    void onAnchorClicked(const QUrl &link);

  private:
    PreferencesWindow *preferencesWindow = nullptr;
};

#endif // MESSAGELOGGER_HPP

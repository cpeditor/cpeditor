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

#include "Util/Util.hpp"
#include <QFile>
#include <QPalette>
#include <QTranslator>
#include <QWidget>
#include <generated/SettingsInfo.hpp>

namespace Util
{

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

QStringList splitArgument(QString arg)
{
    enum
    {
        SA_OUT,
        SA_NORMAL,
        SA_SQUOTE, // single quote
        SA_DQUOTE  // double quote
    } status = SA_OUT;

    QStringList result;
    QString current;
    for (int ptr = 0; ptr < arg.size(); ++ptr)
    {
        switch (status)
        {
        case SA_OUT:
            switch (arg[ptr].unicode())
            {
            case '"':
                status = SA_DQUOTE;
                break;
            case '\'':
                status = SA_SQUOTE;
                break;
            case ' ':
                break;
            default:
                status = SA_NORMAL;
                current = arg[ptr];
            }
            break;
        case SA_NORMAL:
            switch (arg[ptr].unicode())
            {
            case '"':
                status = SA_DQUOTE;
                break;
            case '\'':
                status = SA_SQUOTE;
                break;
            case '\\':
                if (++ptr == arg.size())
                {
                    status = SA_OUT;
                    result.push_back(current);
                    current = "";
                    break;
                }
                current.push_back(arg[ptr]);
                break;
            case ' ':
                status = SA_OUT;
                result.push_back(current);
                current = "";
                break;
            default:
                current.append(arg[ptr]);
            }
            break;
        case SA_SQUOTE:
            switch (arg[ptr].unicode())
            {
            case '\'':
                status = SA_NORMAL;
                break;
            default:
                current.append(arg[ptr]);
            }
            break;
        case SA_DQUOTE:
            switch (arg[ptr].unicode())
            {
            case '"':
                status = SA_NORMAL;
                break;
            case '\\':
                if (++ptr == arg.size())
                {
                    status = SA_OUT;
                    result.push_back(current);
                    current = "";
                    break;
                }
                current.push_back(arg[ptr]);
                break;
            default:
                current.push_back(arg[ptr]);
            }
        }
    }
    if (status == SA_NORMAL)
        result.push_back(current);
    return result;
}

const static QMap<QString, QString> locales = {{"简体中文", "SimplifiedChinese"},
                                               // {"繁體中文", "TraditionalChinese"},
                                               {"Русский", "Russian"}};

void applyNewLocale(const QString &language)
{
    if (language != "English")
    {
        QTranslator *translator = new QTranslator(qApp);
        translator->load(QString(":/translations/%1.qm").arg(locales[language]));
        qApp->installTranslator(translator);
        updateSettingInfo();
    }
}

void showWidgetOnTop(QWidget *widget)
{
    widget->show();
    widget->setWindowState((widget->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    widget->activateWindow();
    widget->raise();
}

} // namespace Util

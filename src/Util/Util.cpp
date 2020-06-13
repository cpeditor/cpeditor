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
#include <QWidget>

namespace Util
{

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

void showWidgetOnTop(QWidget *widget)
{
    widget->show();
    widget->setWindowState((widget->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    widget->activateWindow();
    widget->raise();
}

} // namespace Util

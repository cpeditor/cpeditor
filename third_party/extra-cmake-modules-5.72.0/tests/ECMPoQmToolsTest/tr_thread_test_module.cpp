/*
    SPDX-FileCopyrightText: 2015 Alex Merry <alex.merry@kde.org>

    SPDX-License-Identifier: BSD-3-Clause
*/

#include <QCoreApplication>
#include <QTextStream>

#include <stdio.h>

extern "C" Q_DECL_EXPORT void print_strings()
{
    QTextStream output(stdout);

    output << QCoreApplication::translate("testcontext", "test string") << ":";
    output << QCoreApplication::translate("testcontext", "test plural %n", 0, 5) << '\n';
}

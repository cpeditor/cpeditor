/*
    SPDX-License-Identifier: BSD-3-Clause
*/

#include <QCoreApplication>

#include "log1.h"
#include "log2.h"
#include "log3.h"

#include <iostream>

int main(int argc, char **argv)
{
    // Make the unit test more reliable on system which have these env vars set
    qunsetenv("QT_LOGGING_CONF");
    qunsetenv("QT_LOGGING_RULES");

    QCoreApplication qapp(argc, argv);

    bool success = true;

    // NB: we cannot test against QtInfoMsg, as that (a) does not exist before
    // Qt 5.5, and (b) has incorrect semantics in Qt 5.5, in that it is
    // treated as more severe than QtCriticalMsg.

    if (log1().categoryName() != QLatin1String("log.one")) {
        qWarning("log1 category was \"%s\", expected \"log.one\"", log1().categoryName());
        success = false;
    }
#if QT_VERSION < QT_VERSION_CHECK(5, 4, 0)
    if (!log1().isDebugEnabled()) {
        qWarning("log1 debug messages were not enabled");
        success = false;
    }
#else
    if (log1().isDebugEnabled()) {
        qWarning("log1 debug messages were enabled");
        success = false;
    }
    if (!log1().isWarningEnabled()) {
        qWarning("log1 warning messages were not enabled");
        success = false;
    }
#endif

    if (foo::bar::log2().categoryName() != QLatin1String("log.two")) {
        qWarning("log2 category was \"%s\", expected \"log.two\"", foo::bar::log2().categoryName());
        success = false;
    }
#if QT_VERSION < QT_VERSION_CHECK(5, 4, 0)
    if (!foo::bar::log2().isDebugEnabled()) {
        qWarning("log2 debug messages were not enabled");
        success = false;
    }
#else
    if (foo::bar::log2().isDebugEnabled()) {
        qWarning("log2 debug messages were enabled");
        success = false;
    }
    if (!foo::bar::log2().isWarningEnabled()) {
        qWarning("log2 warning messages were not enabled");
        success = false;
    }
#endif

    if (log3().categoryName() != QLatin1String("three")) {
        qWarning("log3 category was \"%s\", expected \"three\"", log3().categoryName());
        success = false;
    }
#if QT_VERSION < QT_VERSION_CHECK(5, 4, 0)
    if (!log3().isDebugEnabled()) {
        qWarning("log3 debug messages were not enabled");
        success = false;
    }
#else
    if (log3().isDebugEnabled()) {
        qWarning("log3 debug messages were enabled");
        success = false;
    }
    if (log3().isWarningEnabled()) {
        qWarning("log3 warning messages were enabled");
        success = false;
    }
    if (!log3().isCriticalEnabled()) {
        qWarning("log3 critical messages were not enabled");
        success = false;
    }
#endif

    return success ? 0 : 1;
}

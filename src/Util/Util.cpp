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

#include "Util/Util.hpp"
#include "Core/EventLogger.hpp"
#include "Core/Translator.hpp"
#include "generated/version.hpp"
#include <QRegularExpression>
#include <QSettings>
#include <QUrl>
#include <QWidget>

namespace Util
{

void showWidgetOnTop(QWidget *widget)
{
    widget->show();
    widget->setWindowState((widget->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    widget->activateWindow();
    widget->raise();
}

QString sanitizeAnchorName(const QString &str)
{
    return str.trimmed()
        .remove(QRegularExpression(R"([!"\#$%&'()*+,\-./:;<=>?@\[\\\]^_‘{|}~]|，|（|）|。)"))
        .toLower()
        .replace(' ', '-');
}

QString websiteLink(const QString &path)
{
    return QUrl(QString("https://cpeditor.org/%1/%2/%3").arg(MINOR_VERSION).arg(Core::Translator::langCode()).arg(path))
        .url(QUrl::NormalizePathSegments);
}

void associateFile(const QString &appPath, const QString &ext)
{
    QString baseUrl("HKEY_CURRENT_USER\\Software\\Classes");
    QString className("cpeditor." + ext);
    QSettings settingClasses(baseUrl, QSettings::NativeFormat);
    settingClasses.setValue("/" + className + "/Shell/Open/Command/.", R"(")" + appPath + R"(" "%1")");
    settingClasses.setValue("/" + className + "/DefaultIcon/.", appPath + ",0");
    settingClasses.setValue("/" + ext + "/OpenWithProgIds/" + className, "");
    settingClasses.sync();
}

} // namespace Util

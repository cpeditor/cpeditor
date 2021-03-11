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

#include "Core/StyleManager.hpp"
#include "Core/EventLogger.hpp"
#include <QApplication>
#include <QPalette>
#include <QSettings>
#include <QStyle>
#include <QStyleFactory>

namespace Core
{

QPalette StyleManager::defaultPalette;
QString StyleManager::defaultStyle;
QString StyleManager::currentStyle = "default";

void StyleManager::setDefault()
{
    defaultPalette = qApp->palette();
    defaultStyle = qApp->style()->objectName();
    LOG_INFO(INFO_OF(defaultStyle));
}

bool StyleManager::setStyle(const QString &styleName)
{
    bool isValid = styleList().contains(styleName);
    if (!isValid)
    {
        LOG_ERR("Cannot set style " << styleName
                                    << ". No such style found supported on this platform. Supported styles are: ["
                                    << styleList().join(", ") << "]");
        return false;
    }

    if (currentStyle == styleName)
    {
        LOG_INFO("The style is not changed. " << INFO_OF(styleName));
        return true;
    }

    currentStyle = styleName;

#ifdef Q_OS_WIN
    if (styleName == "Auto Fusion")
    {
        if (isWindowsDarkThemeForApps())
            currentStyle = "Dark Fusion";
        else
            currentStyle = "Light Fusion";
    }
#endif

    setPalette(currentStyle);

    if (currentStyle == "default")
    {
        qApp->setStyle(defaultStyle);
    }
    else if (currentStyle == "Light Fusion" || currentStyle == "Dark Fusion")
    {
        qApp->setStyle("Fusion");
    }
    else
    {
        qApp->setStyle(currentStyle);
    }

    return true;
}

QStringList StyleManager::styleList()
{
    auto list = QStyleFactory::keys();

    if (list.contains("Fusion"))
    {
        list.removeAll("Fusion");
        list.append("Light Fusion");
        list.append("Dark Fusion");

#ifdef Q_OS_WIN // On Windows we can check if we have dark theme enabled from system.
        list.append("Auto Fusion");
#endif
    }

    list.prepend("default");

    return list;
}

#ifdef Q_OS_WIN
bool StyleManager::isWindowsDarkThemeForApps()
{
    QSettings settings(R"(HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Themes\Personalize)",
                       QSettings::NativeFormat);
    return settings.value("AppsUseLightTheme") == 0;
}
#endif

void StyleManager::setPalette(const QString &styleName)
{
    if (styleName == "Light Fusion")
        qApp->setPalette(lightFusionPalette());
    else if (styleName == "Dark Fusion")
        qApp->setPalette(darkFusionPalette());
    else
        qApp->setPalette(defaultPalette);
}

QPalette StyleManager::darkFusionPalette()
{
    QPalette darkPalette = defaultPalette;

    QColor darkColor = QColor(45, 45, 45);
    QColor disabledColor = QColor(127, 127, 127);

    darkPalette.setColor(QPalette::Window, darkColor);
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(18, 18, 18));
    darkPalette.setColor(QPalette::AlternateBase, darkColor);
    darkPalette.setColor(QPalette::ToolTipBase, QColor(18, 18, 18));
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Disabled, QPalette::Text, disabledColor);
    darkPalette.setColor(QPalette::Button, darkColor);
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, disabledColor);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::white);
    darkPalette.setColor(QPalette::Disabled, QPalette::HighlightedText, disabledColor);

    return darkPalette;
}

QPalette StyleManager::lightFusionPalette()
{
    QPalette lightPalette = defaultPalette;

    QColor lightColor = QColor(210, 210, 210);
    QColor disabledColor = QColor(128, 128, 128);

    lightPalette.setColor(QPalette::Window, lightColor);
    lightPalette.setColor(QPalette::WindowText, Qt::black);
    lightPalette.setColor(QPalette::Base, QColor(237, 237, 237));
    lightPalette.setColor(QPalette::AlternateBase, lightColor);
    lightPalette.setColor(QPalette::ToolTipBase, QColor(237, 237, 237));
    lightPalette.setColor(QPalette::ToolTipText, Qt::black);
    lightPalette.setColor(QPalette::Text, Qt::black);
    lightPalette.setColor(QPalette::Disabled, QPalette::Text, disabledColor);
    lightPalette.setColor(QPalette::Button, lightColor);
    lightPalette.setColor(QPalette::ButtonText, Qt::black);
    lightPalette.setColor(QPalette::Disabled, QPalette::ButtonText, disabledColor);
    lightPalette.setColor(QPalette::BrightText, Qt::red);
    lightPalette.setColor(QPalette::Link, QColor(213, 115, 37));
    lightPalette.setColor(QPalette::Highlight, QColor(213, 115, 37));
    lightPalette.setColor(QPalette::HighlightedText, Qt::white);
    lightPalette.setColor(QPalette::Disabled, QPalette::HighlightedText, disabledColor);

    return lightPalette;
}

} // namespace Core

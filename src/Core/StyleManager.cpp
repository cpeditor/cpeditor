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

#include "Core/StyleManager.hpp"
#include "Core/EventLogger.hpp"
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QStyleFactory>

#include <QPalette>

#include <QApplication>

namespace Core
{

QStyle *StyleManager::defaultStyle = nullptr;
QString StyleManager::defaultStyleSheet;

void StyleManager::initiateStyle()
{
    defaultStyle = qApp->style();
    defaultStyleSheet = qApp->styleSheet();
    QString styleName = "auto_fusion"; // This value should be read from SettingsHelper::getStyleName(); with its UI
    LOG_INFO_IF(setStyle(styleName), "Initializing application with style: " << styleName);
}

bool StyleManager::setStyle(QString styleName)
{
    bool isValid = styleList().contains(styleName);
    if (!isValid)
    {
        LOG_ERR("Cannot set style " << styleName
                                    << ". No such style found supported on this platform. Supported styles are: ["
                                    << styleList().join(",") << "]");
        return false;
    }

    LOG_INFO("Setting styleName: " << styleName);
    qApp->setStyleSheet(defaultStyleSheet);

    if (styleName == "default")
    {
        qApp->setStyle(defaultStyle);
    }
    else if (styleName == "light_fusion")
    {
        qApp->setStyle(QStyleFactory::create("Fusion"));
        qApp->setPalette(fusionLightPalette());
        setFusionLightTooltip();
    }
    else if (styleName == "dark_fusion")
    {
        qApp->setStyle(QStyleFactory::create("Fusion"));
        qApp->setPalette(fusionDarkPalette());
        setFusionDarkTooltip();
    }
#ifdef Q_OS_WIN
    else if (styleName == "auto_fusion")
    {
        qApp->setStyle(QStyleFactory::create("Fusion"));
        if (isWindowsDarkThemeforApps())
        {
            qApp->setPalette(fusionDarkPalette());
            setFusionDarkTooltip();
        }
        else
        {
            qApp->setPalette(fusionLightPalette());
            setFusionLightTooltip();
        }
    }
#endif
    else
    {
        qApp->setStyle(QStyleFactory::create(styleName));
    }

    return true;
}

QStringList StyleManager::styleList()
{
    auto list = QStyleFactory::keys();
    list.append("default");
    if (list.contains("Fusion"))
    {
        list.removeAll("Fusion");
        list.append("light_fusion");
        list.append("dark_fusion");

#ifdef Q_OS_WIN // In Windows we can check if we have dark theme enabled from system.
        list.append("auto_fusion");
#endif
    }

    return list;
}

bool StyleManager::isWindowsDarkThemeforApps()
{
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
                       QSettings::NativeFormat);
    return settings.value("AppsUseLightTheme") == 0;
}

QPalette StyleManager::fusionDarkPalette()
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

void StyleManager::setFusionDarkTooltip()
{
    qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
}

QPalette StyleManager::fusionLightPalette()
{
    QPalette lightPalette;
    return lightPalette;
}

void StyleManager::setFusionLightTooltip()
{
    qApp->setStyleSheet("QToolTip {}");
}
} // namespace Core
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

#include "Settings/AppearancePage.hpp"
#include "Core/Translator.hpp"
#include "Settings/SettingsInfo.hpp"
#include "Settings/SettingsManager.hpp"
#include "Settings/ValueWrapper.hpp"
#include <QMessageBox>

AppearancePage::AppearancePage(QWidget *parent)
    : PreferencesPageTemplate({"Locale", "UI Style", "Editor Theme", "Editor Font", "Test Cases Font",
                               "Message Logger Font", "Opacity", "Show Compile And Run Only", "Display EOLN In Diff",
                               "Extra Bottom Margin"},
                              true, parent)
{
}

void AppearancePage::makeSettingsTheSameAsUI()
{
    for (int i = 0; i < options.size(); ++i)
    {
        ValueWidget *widget = widgets[i];
        auto si = SettingsInfo::findSetting(options[i]);
        if (si.name == "Locale" && SettingsManager::get(si.name) != widget->getVariant())
        {
            Core::Translator::setLocale(widget->getVariant().toString());
            QMessageBox::warning(this, tr("Change Locale"),
                                 tr("You need to restart the application to completely apply the locale change."));
        }
        SettingsManager::set(si.name, widget->getVariant());
    }
}

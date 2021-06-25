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

#include "Settings/PreferencesTemplate.hpp"
#include "Core/EventLogger.hpp"
#include "Settings/SettingsInfo.hpp"
#include "Settings/SettingsManager.hpp"
#include "Util/Util.hpp"
#include <QCheckBox>

PreferencesTemplate::PreferencesTemplate(QStringList opts, const QString &path, const QString &trPath, bool alignTop,
                                         QWidget *parent)
    : PreferencesPage(parent), options(std::move(opts))
{
    PreferencesPage::setPath(path, trPath);

    QMap<QString, QVariant> var;
    var["filter"] = options;

    panel = new SettingsWrapper;
    panel->path = path;
    panel->trPath = trPath;
    panel->iter = SettingsInfo::fakeRootIter();
    panel->init(this, var);
    makeUITheSameAsSettings();
    panel->enable(true);

    registerWidget(path, panel);

    addWidget(panel->rootWidget());
}

QStringList PreferencesTemplate::content()
{
    auto ctt = panel->content();
    std::sort(ctt.begin(), ctt.end());
    ctt.erase(std::unique(ctt.begin(), ctt.end()), ctt.end());
    return ctt;
}

bool PreferencesTemplate::areSettingsChanged()
{
    return panel->changed();
}

void PreferencesTemplate::makeUITheSameAsDefault()
{
    panel->setdef();
}

void PreferencesTemplate::makeUITheSameAsSettings()
{
    panel->reset();
}

void PreferencesTemplate::makeSettingsTheSameAsUI()
{
    panel->apply();
}
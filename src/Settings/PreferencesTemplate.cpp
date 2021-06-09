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

    panel = new WIP::SettingsWrapper;
    panel->path = path;
    panel->trPath = trPath;
    panel->iter = SettingsInfo::fakeRootIter();
    panel->init(this, options);
    makeUITheSameAsSettings();
    panel->enable(true);

    registerWidget(path, panel);

    addWidget(panel->rootWidget());
}

QStringList PreferencesTemplate::content()
{
    QStringList ret = options;
    for (auto const &opt : options)
    {
        auto si = SettingsInfo::findSetting(opt);
        if (!si.desc.isEmpty())
        {
            ret += si.desc;
            ret += si.untrDesc;
        }
        if (!si.tip.isEmpty())
        {
            ret += si.tip;
            ret += si.untrTip;
        }
    }
    return ret;
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
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
#include <QHBoxLayout>
#include <QVBoxLayout>

PreferencesTemplate::PreferencesTemplate(const QStringList &opts, const QString &path, const QString &trPath,
                                         bool alignTop, QWidget *parent)
    : PreferencesPage(parent)
{
    PreferencesPage::setPath(path, trPath);

    QStringList temp;
    for (const auto &s : opts)
    {
        if (s == "===" && !temp.empty())
        {
            options.push_back(temp);
            temp.clear();
        }
        else
            temp.push_back(s);
    }
    if (!temp.empty())
        options.push_back(temp);

    for (const auto &os : options)
    {
        QMap<QString, QVariant> var;
        var["filter"] = os;

        auto *panel = new SettingsWrapper;
        panel->path = path;
        panel->trPath = trPath;
        panel->iter = SettingsInfo::fakeRootIter();
        panel->init(this, var);
        wraps.push_back(panel);
    }

    makeUITheSameAsSettings();

    QList<SettingBase *> tws;

    for (auto *w : wraps)
    {
        w->enable(true);
        tws.push_back(w);
    }

    registerWidgets(path, tws);

    auto *vlayout = new QVBoxLayout();
    vlayout->addSpacing(20);
    for (auto *w : wraps)
        vlayout->addWidget(w->rootWidget());
    vlayout->addStretch();
    vlayout->addSpacing(20);
    auto *layout = new QHBoxLayout();
    layout->addStretch();
    layout->addLayout(vlayout);
    layout->addStretch();
    addLayout(layout);
}

QStringList PreferencesTemplate::content()
{
    QStringList ctt;
    for (auto *w : wraps)
        ctt += w->content();
    std::sort(ctt.begin(), ctt.end());
    ctt.erase(std::unique(ctt.begin(), ctt.end()), ctt.end());
    return ctt;
}

bool PreferencesTemplate::areSettingsChanged()
{
    return std::any_of(wraps.begin(), wraps.end(), [](SettingsWrapper *w) { return w->changed(); });
}

void PreferencesTemplate::makeUITheSameAsDefault()
{
    std::for_each(wraps.begin(), wraps.end(), [](SettingsWrapper *w) { w->setdef(); });
}

void PreferencesTemplate::makeUITheSameAsSettings()
{
    std::for_each(wraps.begin(), wraps.end(), [](SettingsWrapper *w) { w->reset(); });
}

void PreferencesTemplate::makeSettingsTheSameAsUI()
{
    std::for_each(wraps.begin(), wraps.end(), [](SettingsWrapper *w) { w->apply(); });
}

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

#include "Settings/PreferencesPageTemplate.hpp"
#include "Settings/SettingsManager.hpp"
#include "Util.hpp"
#include <QDebug>
#include <generated/SettingsInfo.hpp>

static QString filter[] = {"Excutable (*" EXE_SUFFIX ")", "C++ Sources (*.cpp *.hpp *.h *.cc *.cxx *.c",
                           "Java Sources (*.java)", "Python Sources (*.py *.py3)"};

static QString titles[] = {
    "Choose Excutable",
    "Choose C++ Sources",
    "Choose Java Sources",
    "Choose Python Sources",
};

PreferencesPageTemplate::PreferencesPageTemplate(QStringList opts, bool alignTop, QWidget *parent)
    : PreferencesGridPage(alignTop, parent), options(opts)
{
    connect(this, &PreferencesPageTemplate::pathChanged, [this](const QString &path) {
        QStringList pl = path.split('/');
        if (pl.size() > 1 && pl.back() == "General")
            setTitle(QString("%1 %2").arg(pl[pl.size() - 2], pl.last()));
        else
            setTitle(pl.last());
    });

    for (const QString &name : options)
    {
        SettingInfo si = findSetting(name);
#ifdef QT_DEBUG
        if (name != si.name)
            qDebug() << "Unknown option" << name;
#endif
        if (si.type == "QString")
        {
            Wrapper<QString> *wrapper = createStringWrapper(si.ui);
            wrapper->init(this, si.param);
            addRow(wrapper, si.tip, si.help, si.desc);
            widgets.push_back(wrapper);
        }
        else if (si.type == "bool")
        {
            Wrapper<bool> *wrapper = createBoolWrapper(si.ui);
            wrapper->init(si.desc, this, si.param);
            addRow(wrapper, si.tip, si.help);
            widgets.push_back(wrapper);
        }
        else if (si.type == "int")
        {
            Wrapper<int> *wrapper = createIntWrapper(si.ui);
            wrapper->init(this, si.param);
            addRow(wrapper, si.tip, si.help, si.desc);
            widgets.push_back(wrapper);
        }
        else if (si.type == "QFont")
        {
            Wrapper<QFont> *wrapper = createFontWrapper(si.ui);
            wrapper->init(this, si.param);
            addRow(wrapper, si.tip, si.help, si.desc);
            widgets.push_back(wrapper);
        }
        else if (si.type == "QStringList")
        {
            Wrapper<QStringList> *wrapper = createStringListWrapper(si.ui);
            wrapper->init(this, si.param);
            addRow(wrapper, si.tip, si.help, si.desc);
            widgets.push_back(wrapper);
        }
    }
}

QStringList PreferencesPageTemplate::content()
{
    return options;
}

bool PreferencesPageTemplate::areSettingsChanged()
{
    for (int i = 0; i < options.size(); ++i)
    {
        ValueWidget *widget = widgets[i];
        SettingInfo si = findSetting(options[i]);
        if (widget->getVariant() != SettingsManager::get(si.name))
            return true;
    }
    return false;
}

void PreferencesPageTemplate::makeUITheSameAsDefault()
{
    for (int i = 0; i < options.size(); ++i)
    {
        ValueWidget *widget = widgets[i];
        SettingInfo si = findSetting(options[i]);
        widget->setVariant(SettingsManager::get(si.name, true));
    }
}

void PreferencesPageTemplate::makeUITheSameAsSettings()
{
    for (int i = 0; i < options.size(); ++i)
    {
        ValueWidget *widget = widgets[i];
        SettingInfo si = findSetting(options[i]);
        widget->setVariant(SettingsManager::get(si.name));
    }
}

void PreferencesPageTemplate::makeSettingsTheSameAsUI()
{
    for (int i = 0; i < options.size(); ++i)
    {
        ValueWidget *widget = widgets[i];
        SettingInfo si = findSetting(options[i]);
        SettingsManager::set(si.name, widget->getVariant());
    }
}

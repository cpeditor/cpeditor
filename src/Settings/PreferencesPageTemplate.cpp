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
#include "Settings/SettingsInfo.hpp"
#include "Settings/SettingsManager.hpp"
#include "Settings/ValueWrapper.hpp"
#include <QCheckBox>
#include <QDebug>

PreferencesPageTemplate::PreferencesPageTemplate(QStringList opts, bool alignTop, QWidget *parent)
    : PreferencesGridPage(alignTop, parent), options(opts)
{
    for (const QString &name : options)
    {
        auto si = SettingsInfo::findSetting(name);
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
        else if (si.type == "QVariantList")
        {
            Wrapper<QVariantList> *wrapper = createStringListsWrapper(si.ui);
            wrapper->init(this, si.param);
            addRow(wrapper, si.tip, si.help, si.desc);
            widgets.push_back(wrapper);
        }
        else
        {
            Q_UNREACHABLE();
        }

        if (si.immediatelyApply)
        {
            auto widget = widgets.back();
            connect(
                widget, &ValueWidget::valueChanged, this, [=] { SettingsManager::set(si.name, widget->getVariant()); },
                Qt::DirectConnection); // PreferencesPage::registerWidget uses Qt::QueuedConnection
        }
    }

    for (const QString &name : options)
    {
        auto si = SettingsInfo::findSetting(name);
        if (si.depends.isEmpty())
            continue;
        for (auto depend : si.depends)
        {
            if (!options.contains(depend.first))
            {
                qDebug() << name << " depends on unknown option " << depend.first;
                continue;
            }
            auto dependWidget = widgets[options.indexOf(depend.first)];
            connect(dependWidget, &ValueWidget::valueChanged, [this, name] { onDependencyUpdated(name); });
        }
        onDependencyUpdated(name);
    }
}

QStringList PreferencesPageTemplate::content()
{
    QStringList ret = options;
    for (auto opt : options)
    {
        auto si = SettingsInfo::findSetting(opt);
        if (!si.desc.isEmpty())
            ret += si.desc;
        if (!si.tip.isEmpty())
            ret += si.tip;
        if (!si.help.isEmpty())
            ret += si.help;
    }
    return ret;
}

void PreferencesPageTemplate::setPath(const QString &path)
{
    PreferencesPage::setPath(path);
    for (const QString &name : options)
        SettingsManager::setPath(name, path + "/" + SettingsInfo::findSetting(name).desc);
}

bool PreferencesPageTemplate::areSettingsChanged()
{
    for (int i = 0; i < options.size(); ++i)
    {
        ValueWidget *widget = widgets[i];
        auto si = SettingsInfo::findSetting(options[i]);
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
        auto si = SettingsInfo::findSetting(options[i]);
        widget->setVariant(SettingsManager::get(si.name, true));
    }
}

void PreferencesPageTemplate::makeUITheSameAsSettings()
{
    for (int i = 0; i < options.size(); ++i)
    {
        ValueWidget *widget = widgets[i];
        auto si = SettingsInfo::findSetting(options[i]);
        widget->setVariant(SettingsManager::get(si.name));
    }
}

void PreferencesPageTemplate::makeSettingsTheSameAsUI()
{
    for (int i = 0; i < options.size(); ++i)
    {
        ValueWidget *widget = widgets[i];
        auto si = SettingsInfo::findSetting(options[i]);
        if (SettingsManager::get(si.name) != widget->getVariant())
            si.onApply(&si, widget, this);
        SettingsManager::set(si.name, widget->getVariant());
    }
}

void PreferencesPageTemplate::onDependencyUpdated(const QString &settingName)
{
    auto si = SettingsInfo::findSetting(settingName);
    auto currentWidget = widgets[options.indexOf(settingName)]->coreWidget();
    for (auto depend : si.depends)
    {
        auto dependWidget = widgets[options.indexOf(depend.first)];
        if (depend.second(dependWidget->getVariant()))
        {
            if (!si.requireAllDepends)
            {
                currentWidget->setEnabled(true);
                return;
            }
        }
        else
        {
            if (si.requireAllDepends)
            {
                currentWidget->setEnabled(false);
                return;
            }
        }
    }
    if (si.requireAllDepends)
        currentWidget->setEnabled(true);
    else
        currentWidget->setEnabled(false);
}

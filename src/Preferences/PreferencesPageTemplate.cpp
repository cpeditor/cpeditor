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

#include "Preferences/PreferencesPageTemplate.hpp"
#include "Core/Settings.hpp"
#include "Core/SettingsInfo.hpp"
#include "Util.hpp"
#include <QDebug>

static QString filter[] = {"Excutable (*" EXE_SUFFIX ")", "C++ Sources (*.cpp *.hpp *.h *.cc *.cxx *.c",
                           "Java Sources (*.java)", "Python Sources (*.py *.py3)"};

static QString titles[] = {
    "Choose Excutable",
    "Choose C++ Sources",
    "Choose Java Sources",
    "Choose Python Sources",
};

PreferencesPageTemplate::PreferencesPageTemplate(QStringList opts, QWidget *parent)
    : PreferencesGridPage(parent), options(opts)
{
    for (const QString &desc : options)
    {
        SettingInfo si = findSetting(desc);
#ifdef QT_DEBUG
        if (desc != si.desc)
            qDebug() << "Unknown options" << desc;
#endif
        if (si.type == "QString")
        {
            if (!si.param1.isNull()) // Don't use QLineEdit
            {
                if (si.param1.toString() == "PathItem")
                {
                    PathItem *item = new PathItem(filter[si.param2.toInt()], titles[si.param2.toInt()], this);
                    registerAddRow(si.desc, item);
                    widgets.push_back(item);
                }
                else if (si.param1.toString() == "QComboBox")
                {
                    QComboBox *combo = new QComboBox(this);
                    combo->addItems(si.param2.toStringList());
                    registerAddRow(si.desc, combo);
                    widgets.push_back(combo);
                }
            }
            else
            {
                QLineEdit *edit = new QLineEdit(this);
                registerAddRow(si.desc, edit);
                widgets.push_back(edit);
            }
        }
        else if (si.type == "int")
        {
            QSpinBox *spin = new QSpinBox(this);
            spin->setRange(si.param1.toInt(), si.param2.toInt());
            registerAddRow(si.desc, spin);
            widgets.push_back(spin);
        }
        else if (si.type == "bool")
        {
            QCheckBox *check = new QCheckBox(si.desc, this);
            registerAddRow(check);
            widgets.push_back(check);
        }
    }
}

QStringList PreferencesPageTemplate::content()
{
    return options;
}

static QVariant getValue(const SettingInfo &si, QWidget *widget)
{
    if (si.type == "QString")
    {
        if (!si.param1.isNull())
        {
            if (si.param1.toString() == "PathItem")
            {
                return dynamic_cast<PathItem *>(widget)->getLineEdit()->text();
            }
            else if (si.param1.toString() == "QComboBox")
            {
                return dynamic_cast<QComboBox *>(widget)->currentText();
            }
        }
        else
        {
            return dynamic_cast<QLineEdit *>(widget)->text();
        }
    }
    else if (si.type == "int")
    {
        return dynamic_cast<QSpinBox *>(widget)->value();
    }
    else if (si.type == "bool")
    {
        return dynamic_cast<QCheckBox *>(widget)->isChecked();
    }
    return QVariant();
}

static void setValue(const SettingInfo &si, QWidget *widget, QVariant value)
{
    if (si.type == "QString")
    {
        if (!si.param1.isNull())
        {
            if (si.param1.toString() == "PathItem")
            {
                dynamic_cast<PathItem *>(widget)->getLineEdit()->setText(value.toString());
            }
            else if (si.param1.toString() == "QComboBox")
            {
                dynamic_cast<QComboBox *>(widget)->setCurrentText(value.toString());
            }
        }
        else
        {
            dynamic_cast<QLineEdit *>(widget)->setText(value.toString());
        }
    }
    else if (si.type == "int")
    {
        dynamic_cast<QSpinBox *>(widget)->setValue(value.toInt());
    }
    else if (si.type == "bool")
    {
        dynamic_cast<QCheckBox *>(widget)->setChecked(value.toBool());
    }
}

bool PreferencesPageTemplate::areSettingsChanged()
{
    for (int i = 0; i < options.size(); ++i)
    {
        QWidget *widget = widgets[i];
        SettingInfo si = findSetting(options[i]);
        if (getValue(si, widget) != SettingsManager::get(si.desc))
            return true;
    }
    return false;
}

void PreferencesPageTemplate::makeUITheSameAsDefault()
{
    for (int i = 0; i < options.size(); ++i)
    {
        QWidget *widget = widgets[i];
        SettingInfo si = findSetting(options[i]);
        setValue(si, widget, SettingsManager::get(si.desc, true));
    }
}

void PreferencesPageTemplate::makeUITheSameAsSettings()
{
    for (int i = 0; i < options.size(); ++i)
    {
        QWidget *widget = widgets[i];
        SettingInfo si = findSetting(options[i]);
        setValue(si, widget, SettingsManager::get(si.desc));
    }
}

void PreferencesPageTemplate::makeSettingsTheSameAsUI()
{
    for (int i = 0; i < options.size(); ++i)
    {
        QWidget *widget = widgets[i];
        SettingInfo si = findSetting(options[i]);
        SettingsManager::set(si.desc, getValue(si, widget));
    }
}

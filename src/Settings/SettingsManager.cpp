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

#include "Settings/SettingsManager.hpp"
#include "Core/EventLogger.hpp"
#include <QDebug>
#include <QFile>
#include <QFont>
#include <QRect>
#include <QSettings>
#include <QStandardPaths>
#include <generated/SettingsInfo.hpp>

Settings *SettingsManager::cur = nullptr;
Settings *SettingsManager::def = nullptr;

static QStringList configFileLocation = {"$AC/cp_editor_settings.ini", "$H/.cp_editor_settings.ini",
                                         "$H/cp_editor_settings.ini"};

static const QStringList noUnknownKeyWarning = {"C++/Run Command", "Python/Compile Command"};

void SettingsManager::init()
{
    for (QString &location : configFileLocation)
        location = location.replace("$AC", QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation))
                       .replace("$H", QStandardPaths::writableLocation(QStandardPaths::HomeLocation));

    // find exist config file
    QString loadPath;
    for (auto path : configFileLocation)
    {
        if (QFile::exists(path))
        {
            loadPath = path;
            break;
        }
    }

    cur = new Settings();
    def = new Settings();

    // default settings
    for (const SettingInfo &si : settingInfo)
        def->insert(si.name, si.def);

    if (!loadPath.isEmpty())
    {
        QSettings setting(loadPath, QSettings::IniFormat);

        // load most of settings
        for (const SettingInfo &si : settingInfo)
        {
            if (setting.contains(si.key()))
                set(si.name, setting.value(si.key()));
            else
                for (const QString &old : si.old)
                    if (setting.contains(old))
                    {
                        set(si.name, setting.value(old));
                        break;
                    }
        }

        // load snippets
        setting.beginGroup("snippets");
        QStringList langs = setting.childGroups();
        if (langs.contains("Cpp"))
        {
            setting.beginGroup("Cpp");
            QStringList keys = setting.allKeys();
            for (const QString &key : keys)
                set(QString("Snippets/C++/%1").arg(key), setting.value(key));
            setting.endGroup();
            langs.removeOne("Cpp");
        }
        for (const QString &lang : langs)
        {
            setting.beginGroup(lang);
            QStringList keys = setting.allKeys();
            for (const QString &key : keys)
                set(QString("Snippets/%1/%2").arg(lang, key), setting.value(key));
            setting.endGroup();
        }
        setting.endGroup();

        // load editor status
        setting.beginGroup("editor_status");
        for (const QString &index : setting.allKeys())
            set(QString("Editor Status/%1").arg(index), setting.value(index));
        setting.endGroup();

        // rename themes
        QString theme = get("Editor Theme")
                            .toString()
                            .replace("Monkai", "Monokai")
                            .replace("Drakula", "Dracula")
                            .replace("Solarised", "Solarized");
        set("Editor Theme", theme);
    }
}

void SettingsManager::deinit()
{
    QSettings setting(configFileLocation[0], QSettings::IniFormat);
    setting.clear(); // Otherwise SettingsManager::remove won't work

    // save most settings
    for (const SettingInfo &si : settingInfo)
        setting.setValue(si.key(), get(si.name));

    // save snippets
    for (QString key : keyStartsWith("Snippets/"))
        setting.setValue(key.replace("Snippets/", "snippets/"), get(key));

    // save editor status
    for (QString key : keyStartsWith("Editor Status/"))
        setting.setValue(key.replace("Editor Status/", "editor_status/"), get(key));

    setting.sync();
    delete cur;
    delete def;
    cur = def = nullptr;
}

QVariant SettingsManager::get(QString key, bool alwaysDefault)
{
    if (!alwaysDefault && cur->contains(key))
        return cur->value(key);
    else if (def->contains(key))
        return def->value(key);
    else
    {
#ifdef QT_DEBUG
        if (!noUnknownKeyWarning.contains(key))
            qDebug() << "Settings: getting unknown key: " << key;
#endif
        LOG_INFO("SettingsManager::getting unknown key: " << key);
        return QVariant();
    }
}

bool SettingsManager::contains(const QString &key)
{
    return cur->contains(key);
}

void SettingsManager::set(const QString &key, QVariant value)
{
    LOG_INFO(INFO_OF(key) << "," << INFO_OF(value.toString()));
    cur->insert(key, value);
}

void SettingsManager::remove(QStringList keys)
{
    for (const QString &key : keys)
        cur->remove(key);
}

void SettingsManager::reset()
{
    *cur = *def;
}

QStringList SettingsManager::keyStartsWith(const QString &head)
{
    QStringList keys = cur->keys();
    keys.erase(std::remove_if(keys.begin(), keys.end(), [head](QString s) { return !s.startsWith(head); }), keys.end());
    return keys;
}

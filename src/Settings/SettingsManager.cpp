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
#include "Settings/FileProblemBinder.hpp"
#include "Settings/SettingsUpdater.hpp"
#include "Util/FileUtil.hpp"
#include "generated/portable.hpp"
#include <QDebug>
#include <QFile>
#include <QFont>
#include <QRect>
#include <QSettings>

QVariantMap *SettingsManager::cur = nullptr;
QVariantMap *SettingsManager::def = nullptr;
QMap<QString, QString> *SettingsManager::settingPath = nullptr;

const static QStringList configFileLocations = {
#ifdef PORTABLE_VERSION
    "$BINARY/cp_editor_settings.ini",
#endif
    "$APPCONFIG/cp_editor_settings.ini", "$HOME/.cp_editor_settings.ini", "$HOME/cp_editor_settings.ini"};

static const QStringList noUnknownKeyWarning = {"C++/Run Command", "Python/Compile Command"};

void SettingsManager::load(QSettings &setting, const QString &prefix, const QList<SettingsInfo::SettingInfo> &infos)
{
    for (const auto &si : infos)
    {
        if (si.type == "Object")
        {
            setting.beginGroup(si.name);
            for (const QString &sub : setting.childGroups())
            {
                setting.beginGroup(sub);
                load(setting, QString("%1%2/%3/").arg(prefix, si.name, sub), si.child);
                setting.endGroup();
            }
            setting.endGroup();
        }
        else if (si.type.startsWith("QMap:"))
        {
            QString final = si.type.mid(5);
            setting.beginGroup(si.key());
            for (const QString &key : setting.childKeys())
            {
                set(QString("%1%2/%3").arg(prefix, si.name, key), setting.value(key));
            }
            setting.endGroup();
        }
        else if (setting.contains(si.key()) && setting.value(si.key()).isValid())
            set(si.name, setting.value(si.key()));
    }
}

void SettingsManager::save(QSettings &setting, const QString &prefix, const QList<SettingsInfo::SettingInfo> &infos)
{
    for (const auto &si : infos)
        if (si.type == "Object")
        {
            QString head = QString("%1%2/").arg(prefix, si.name);
            QStringList keys = itemUnder(head);
            for (const QString &k : keys)
            {
                save(setting, QString("%1%2/").arg(head, k), si.child);
            }
        }
        else if (si.type.startsWith("QMap:"))
            for (const QString &key : itemUnder(QString("%1%2/").arg(prefix, si.name)))
                setting.setValue(QString("%1%2/%3").arg(prefix, si.key(), key),
                                 get(QString("%1%2/%3").arg(prefix, si.name, key)));
        else
            setting.setValue(QString("%1%2").arg(prefix, si.key()), get(si.name));
}

void SettingsManager::init()
{
    if (cur)
        delete cur;
    if (def)
        delete def;
    if (settingPath)
        delete settingPath;

    cur = new QVariantMap();
    def = new QVariantMap();
    settingPath = new QMap<QString, QString>();

    generateDefaultSettings();

    QString path = Util::firstExistingConfigPath(configFileLocations);
    if (!path.isEmpty())
        loadSettings(path);
}

void SettingsManager::deinit()
{
    saveSettings(Util::configFilePath(configFileLocations[0]));

    delete cur;
    delete def;
    delete settingPath;
    cur = def = nullptr;
    settingPath = nullptr;
}

void SettingsManager::generateDefaultSettings()
{
    LOG_INFO("Generating default settings");

    for (const auto &si : SettingsInfo::getSettings())
        def->insert(si.name, si.def);

    LOG_INFO("Default settings are generated")
}

void SettingsManager::loadSettings(const QString &path)
{
    LOG_INFO("Start loading settings from " + path);

    QSettings setting(path, QSettings::IniFormat);
    load(setting, "", SettingsInfo::getSettings());
    SettingsUpdater::updateSetting(setting);

    // load file problem binding
    FileProblemBinder::fromVariant(setting.value("file_problem_binding"));

    LOG_INFO("Settings have been loaded from " + path);
}

void SettingsManager::saveSettings(const QString &path)
{
    LOG_INFO("Start saving settings to " + path);

    QSettings setting(path, QSettings::IniFormat);
    setting.clear(); // Otherwise SettingsManager::remove won't work
    save(setting, "", SettingsInfo::getSettings());

    // save file problem binding
    setting.setValue("file_problem_binding", FileProblemBinder::toVariant());

    setting.sync();

    LOG_INFO("Settings have been saved to " + path);
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
        LOG_WARN("SettingsManager::getting unknown key: " << key);
        return QVariant();
    }
}

bool SettingsManager::contains(const QString &key, bool includingDefault)
{
    return cur->contains(key) || (includingDefault && def->contains(key));
}

void SettingsManager::set(const QString &key, QVariant value)
{
    LOG_INFO_IF(!key.startsWith("Language Config/"), INFO_OF(key) << INFO_OF(value.toString()));
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

void SettingsManager::setPath(const QString &key, const QString &path)
{
    settingPath->insert(key, path);
}

QString SettingsManager::getPathText(const QString &key, bool parent)
{
    if (!settingPath->contains(key))
    {
#ifdef QT_DEBUG
        qDebug() << "SettingsManager: Getting unknown key path: " << key;
#endif
        LOG_WARN("Getting unknown key path: " << key);
        return "Unknown";
    }
    auto nodes = QStringList(QCoreApplication::translate("PreferencesWindow", "Preferences")) +
                 settingPath->value(key).split('/');
    for (int i = 0; i < nodes.count() - 1; ++i)
        nodes[i] = QCoreApplication::translate("PreferencesWindow", nodes[i].toStdString().c_str());
    if (parent)
        nodes.pop_back();
    else
        nodes.back() = QCoreApplication::translate("SettingsInfo", nodes.back().toStdString().c_str());
    return nodes.join("->");
}

QStringList SettingsManager::keyStartsWith(const QString &head)
{
    QStringList keys = cur->keys();
    keys.erase(std::remove_if(keys.begin(), keys.end(), [head](QString s) { return !s.startsWith(head); }), keys.end());
    return keys;
}

QStringList SettingsManager::itemUnder(const QString &head)
{
    QStringList temp = keyStartsWith(head);
    for (QString &k : temp)
    {
        k = k.mid(head.length());
        int pos = k.indexOf('/');
        if (pos != -1)
            k = k.left(pos);
    }
    std::sort(temp.begin(), temp.end());
    temp.erase(std::unique(temp.begin(), temp.end()), temp.end());
    return temp;
}

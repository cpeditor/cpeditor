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

#include "Settings/SettingsManager.hpp"
#include "Core/EventLogger.hpp"
#include "Settings/FileProblemBinder.hpp"
#include "Settings/SettingsUpdater.hpp"
#include "Util/FileUtil.hpp"
#include "generated/portable.hpp"
#include <QDateTime>
#include <QFile>
#include <QFont>
#include <QRect>
#include <QSettings>

QVariantMap *SettingsManager::cur = nullptr;
QMap<QString, QString> *SettingsManager::settingPath = nullptr;
QMap<QString, QString> *SettingsManager::settingTrPath = nullptr;
QMap<QString, QString> *SettingsManager::pathSetting = nullptr;
QMap<QString, QWidget *> *SettingsManager::settingWidget = nullptr;
long long SettingsManager::startTime = 0;

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
            if (!setting.contains(si.name + '@'))
                continue;
            auto keys = setting.value(si.name + '@', setting.childGroups()).toStringList();
            setting.beginGroup(si.name);
            for (const QString &sub : keys)
            {
                setting.beginGroup(sub);
                load(setting, QString("%1%2/%3/").arg(prefix, si.name, sub), si.child);
                setting.endGroup();
            }
            auto oldKeys = get(prefix + si.name + '@').toStringList();
            for (const auto &k : oldKeys)
            {
                if (keys.indexOf(k) == -1)
                    keys.push_back(k);
            }
            set(prefix + si.name + '@', keys);
            setting.endGroup();
        }
        else if (setting.contains(si.key()) && setting.value(si.key()).isValid())
            set(prefix + si.name, setting.value(si.key()));
    }
}

void SettingsManager::save(QSettings &setting, const QString &prefix, const QList<SettingsInfo::SettingInfo> &infos)
{
    for (const auto &si : infos)
        if (si.type == "Object")
        {
            QString head = QString("%1%2").arg(prefix, si.name);
            QStringList keys = get(head + '@').toStringList();
            setting.setValue(head + '@', keys);
            for (const QString &k : keys)
            {
                save(setting, QString("%1/%2/").arg(head, k), si.child);
            }
        }
        else
            setting.setValue(QString("%1%2").arg(prefix, si.key()), get(prefix + si.name));
}

void SettingsManager::init()
{
    delete cur;
    delete settingPath;
    delete settingTrPath;
    delete pathSetting;
    delete settingWidget;

    cur = new QVariantMap();
    settingPath = new QMap<QString, QString>();
    settingTrPath = new QMap<QString, QString>();
    pathSetting = new QMap<QString, QString>();
    settingWidget = new QMap<QString, QWidget *>();

    startTime = QDateTime::currentSecsSinceEpoch();

    QString path = Util::firstExistingConfigPath(configFileLocations);
    if (!path.isEmpty())
        loadSettings(path);

    generateDefaultSettings();
}

void SettingsManager::deinit()
{
    set("Total Usage Time", get("Total Usage Time").toInt() + QDateTime::currentSecsSinceEpoch() - startTime);

    saveSettings(QString());

    delete cur;
    delete settingPath;
    delete settingTrPath;
    delete pathSetting;
    delete settingWidget;
    cur = nullptr;
    settingPath = settingTrPath = pathSetting = nullptr;
    settingWidget = nullptr;
}

void SettingsManager::fillWithDefault(const SettingsInfo::SettingIter &pos)
{
    if (pos->type == "Object")
    {
        QStringList child;
        if (cur->contains(pos.key() + '@'))
            child = cur->value(pos.key() + '@').toStringList();
        else if (!pos.getDefault().isNull())
            child = pos.getDefault().toStringList();
        cur->insert(pos.key() + '@', child);
        if (pos->name == "@ROOT@")
        {
            for (const auto &sub : pos->child)
            {
                auto it = pos;
                it.child("", &sub);
                fillWithDefault(it);
            }
            return;
        }
        for (const auto &sub : child)
            for (const auto &it : pos.allVisibleChild(sub))
                fillWithDefault(it);
    }
    else
    {
        if (cur->contains(pos.key()))
            return;
        cur->insert(pos.key(), pos.getDefault());
    }
}

void SettingsManager::generateDefaultSettings()
{
    LOG_INFO("Generating default settings");

    fillWithDefault(SettingsInfo::fakeRootIter());

    LOG_INFO("Default settings are generated")
}

void SettingsManager::loadSettings(const QString &path)
{
    LOG_INFO("Start loading settings from " + path);

    QSettings setting(path, QSettings::IniFormat);
    SettingsUpdater::updateSetting(setting);
    load(setting, "", SettingsInfo::getSettings());
    SettingsUpdater::updateSettingFinal();

    // load file problem binding
    FileProblemBinder::fromVariant(setting.value("file_problem_binding"));

    LOG_INFO("Settings have been loaded from " + path);
}

void SettingsManager::saveSettings(const QString &path)
{
    const auto savePath = path.isEmpty() ? Util::configFilePath(configFileLocations[0]) : path;

    LOG_INFO("Start saving settings to " + savePath);

    QSettings setting(savePath, QSettings::IniFormat);
    setting.clear(); // Otherwise SettingsManager::remove won't work
    save(setting, "", SettingsInfo::getSettings());

    // save file problem binding
    setting.setValue("file_problem_binding", FileProblemBinder::toVariant());

    setting.sync();

    LOG_INFO("Settings have been saved to " + savePath);
}

QVariant SettingsManager::get(QString const &key)
{
    if (cur->contains(key))
        return cur->value(key);
    return QVariant();
}

bool SettingsManager::contains(const QString &key)
{
    return cur->contains(key);
}

void SettingsManager::set(const QString &key, QVariant const &value)
{
    cur->insert(key, value);
}

void SettingsManager::remove(QStringList const &keys)
{
    for (const QString &key : keys)
        cur->remove(key);
}

void SettingsManager::reset()
{
    cur->clear();
    generateDefaultSettings();
}

void SettingsManager::setPath(const QString &key, const QString &path, const QString &trPath)
{
    settingPath->insert(key, path);
    settingTrPath->insert(key, trPath);
    pathSetting->insert(path, key);
}

QString SettingsManager::getPathText(const QString &key, bool parent)
{
    if (!settingPath->contains(key))
    {
        LOG_DEV("Getting unknown path: " << key);
        return "Unknown";
    }
    auto path = settingPath->value(key);
    auto trPath = QCoreApplication::translate("PreferencesWindow", "Preferences") + "/" + settingTrPath->value(key);
    if (parent)
    {
        path = path.left(path.lastIndexOf('/'));
        trPath = trPath.left(trPath.lastIndexOf('/'));
    }
    return QString("<a href='#Preferences/%1'>%2</a>").arg(path).arg(trPath.replace('/', "->"));
}

QString SettingsManager::getKeyOfPath(const QString &path)
{
    if (!pathSetting->contains(path))
    {
        LOG_WARN("Getting unknown key of path: " << INFO_OF(path));
        return QString();
    }
    return pathSetting->value(path);
}

void SettingsManager::setWidget(const QString &key, QWidget *widget)
{
    settingWidget->insert(key, widget);
}

QWidget *SettingsManager::getWidget(const QString &key)
{
    if (!settingWidget->contains(key))
    {
        LOG_WARN("Getting unknown widget: " << INFO_OF(key));
        return nullptr;
    }
    return settingWidget->value(key);
}

QStringList SettingsManager::keyStartsWith(const QString &head)
{
    QStringList keys = cur->keys();
    keys.erase(std::remove_if(keys.begin(), keys.end(), [head](QString const &s) { return !s.startsWith(head); }),
               keys.end());
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

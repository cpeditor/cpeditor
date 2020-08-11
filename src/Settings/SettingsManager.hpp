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

#ifndef SETTINGSMANAGER_HPP
#define SETTINGSMANAGER_HPP

#include "Settings/SettingsInfo.hpp"

class QSettings;

class SettingsManager
{
  private:
    static void load(QSettings &setting, const QString &prefix, const QList<SettingsInfo::SettingInfo> &infos);
    static void save(QSettings &setting, const QString &prefix, const QList<SettingsInfo::SettingInfo> &infos);

  public:
    static void init();
    static void deinit();

    static void generateDefaultSettings();

    static void loadSettings(const QString &path);
    static void saveSettings(const QString &path);

    static QVariant get(QString key, bool alwaysDefault = false);
    static bool contains(const QString &key, bool includingDefault = false);
    static void set(const QString &key, QVariant value);
    static void remove(QStringList keys);
    static void reset();

    static void setPath(const QString &key, const QString &path);
    static QString getPathText(const QString &key, bool parent = false);

    static QStringList keyStartsWith(const QString &head);
    static QStringList itemUnder(const QString &head);

  private:
    static QVariantMap *cur;
    static QVariantMap *def;
    static QMap<QString, QString> *settingPath;
};

#endif // SETTINGSMANAGER_HPP

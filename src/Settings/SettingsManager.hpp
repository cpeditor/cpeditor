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

    /**
     * @brief save settings to the given path
     * @param path the path to save at; if empty, save at the default path
     */
    static void saveSettings(const QString &path);

    static QVariant get(QString const &key, bool alwaysDefault = false);
    static bool contains(const QString &key, bool includingDefault = false);
    static void set(const QString &key, QVariant const &value);
    static void remove(QStringList const &keys);
    static void reset();

    /**
     * @brief set the path of a setting in the preferences window
     * @param key the name of the setting
     * @param path The path to the setting, in the form "X/Y/Z/<desc>". The <desc> shouldn't contain slashes.
     * @param trPath the translation of the path, in the form "X->Y->Z-><trdesc>"
     */
    static void setPath(const QString &key, const QString &path, const QString &trPath);
    /**
     * @brief get the path of a setting which can be shown in the UI
     * @param key the name of the setting
     * @param parent get the path to the parent directory instead of the setting itself
     */
    static QString getPathText(const QString &key, bool parent = false);
    static QString getKeyOfPath(const QString &path);

    static void setWidget(const QString &key, QWidget *widget);
    static QWidget *getWidget(const QString &key);

    static QStringList keyStartsWith(const QString &head);
    static QStringList itemUnder(const QString &head);

  private:
    static QVariantMap *cur;
    static QVariantMap *def;
    static QMap<QString, QString> *settingPath;
    static QMap<QString, QString> *settingTrPath;
    static QMap<QString, QString> *pathSetting;
    static QMap<QString, QWidget *> *settingWidget;
};

#endif // SETTINGSMANAGER_HPP

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

#ifndef SETTINGSINFO_HPP
#define SETTINGSINFO_HPP

#include <QCoreApplication>
#include <QVariant>
#include <functional>

class ValueWidget;

class SettingsInfo
{
    Q_DECLARE_TR_FUNCTIONS(SettingsInfo)

  public:
    class SettingInfo
    {
      public:
        QString name;             // key
        QString desc;             // translated description
        QString untrDesc;         // untranslated description
        QString type;             // int, bool, QString, etc.
        QString ui;               // type of the widget
        QString tip;              // translated tooltips
        QString untrTip;          // untranslated tooltips
        QString docAnchor;        // the anchor of the documentation
        bool noDoc;               // this setting is very simple so it has no documentation on the website
        bool requireAllDepends{}; // false for one of the depends, true for all depends
        bool immediatelyApply{};
        std::function<void(SettingInfo *, ValueWidget *, QWidget *)> onApply;
        QList<QPair<QString, std::function<bool(const QVariant &)>>> depends;
        QList<QString> old; // the old keys of this setting
        QVariant def;
        QVariant param;
        QList<SettingInfo> child;

        QString key() const
        {
            return name.toLower().replace("c++", "cpp").replace(' ', '_');
        }
    };

    static void updateSettingInfo();

    static SettingInfo findSetting(const QString &name, const QList<SettingInfo> &infos = settings)
    {
        for (const SettingInfo &si : infos)
            if (si.name == name)
                return si;
        return SettingInfo();
    }

    static QList<SettingInfo> getSettings()
    {
        return settings;
    }

  private:
    static QList<SettingInfo> settings;

    friend class SettingsUpdater;
};

#endif // SETTINGSINFO_HPP

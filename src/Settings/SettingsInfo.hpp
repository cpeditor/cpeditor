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

class SettingsInfo
{
    Q_DECLARE_TR_FUNCTIONS(SettingsInfo)

  public:
    class SettingInfo;

    struct SettingIter
    {
        const SettingInfo *info;
        QStringList pre, trPre;

        SettingIter() = default;
        SettingIter(const SettingInfo *i) : info(i)
        {
        }
        SettingIter &child(const QString &key, const QString &name)
        {
            return child(key, key, name);
        }
        SettingIter &child(const QString &key, const SettingInfo *ch)
        {
            return child(key, key, ch);
        }
        SettingIter &child(const QString &key, const QString &trKey, const QString &name);
        SettingIter &child(const QString &key, const QString &trKey, const SettingInfo *ch);
        QList<SettingIter> allVisibleChild(const QString &key) const;
        QString key() const
        {
            auto p = pre;
            p.append(info->name);
            return p.join("/");
        }
        const SettingInfo *operator->() const
        {
            return info;
        }
        QString format(bool needtr, const QString &fmt) const
        {
            return needtr ? format(fmt) : formatuntr(fmt);
        }
        QString format(const QString &fmt) const
        {
            return _format(fmt, trPre);
        }
        QString formatuntr(const QString &fmt) const
        {
            return _format(fmt, pre);
        }
        QString getDesc(bool needtr = true) const
        {
            if (!info)
                return "";
            return format(needtr, needtr ? info->desc : info->untrDesc);
        }
        QVariant getDefault() const
        {
            if (!info)
                return QVariant();
            if (info->methods.contains("getDefault"))
                return info->call("getDefault", "pre", pre, "param", getParam(), "rawparam", info->param, "def",
                                  info->def);
            return info->def;
        }
        QVariant getParam() const
        {
            if (!info)
                return QVariant();
            if (info->methods.contains("getParam"))
                return info->call("getParam", "pre", pre, "param", info->param);
            return info->param;
        }
        QString getTip(bool needtr = true) const
        {
            if (!info)
                return "";
            if (info->methods.contains("getTip"))
                return format(needtr, info->call("getTip", "pre", pre, "param", getParam(), "rawparam", info->param,
                                                 "needtr", needtr, "tip", needtr ? info->tip : info->untrTip)
                                          .toString());
            return needtr ? format(info->tip) : formatuntr(info->untrTip);
        }
        QVariant buildChildDefault(const QString &key) const;

      private:
        QString _format(const QString &fmt, const QStringList &p) const;
    };

    class SettingInfo
    {
      public:
        QString name;           // key
        QString desc;           // translated description
        QString untrDesc;       // untranslated description
        QString type;           // int, bool, QString, etc.
        QString ui;             // type of the widget
        QString tip;            // translated tooltips
        QString untrTip;        // untranslated tooltips
        QString docAnchor;      // the anchor of the documentation
        bool requireAllDepends; // false for one of the depends, true for all depends
        bool immediatelyApply;
        QMap<QString, std::function<QVariant(QVariant)>> methods;
        QList<QPair<QString, std::function<bool(const QVariant &)>>> depends;
        QList<QString> old; // the old keys of this setting
        QVariant def;
        QVariant param;
        QList<SettingInfo> child;

        QString key() const
        {
            return name.toLower().replace("c++", "cpp").replace(' ', '_');
        }
        int findChild(const QString &name) const;
        template <typename... Extra> QVariant call(const QString &name, const Extra &... extra) const
        {
            if (!methods.contains(name))
            {
                return QVariant();
            }
            QMap<QString, QVariant> arg;
            return _call(methods[name], arg, extra...);
        }

      private:
        template <typename... Extra>
        QVariant _call(std::function<QVariant(QVariant)> func, QMap<QString, QVariant> &arg, const QString &key,
                       const QVariant &value, const Extra &... extra) const
        {
            arg[key] = value;
            return _call(func, arg, extra...);
        }
        QVariant _call(std::function<QVariant(QVariant)> func, QMap<QString, QVariant> &arg, const QString &key,
                       const QVariant &value) const
        {
            arg[key] = value;
            return func(arg);
        }
        QVariant _call(std::function<QVariant(QVariant)> func, QMap<QString, QVariant> &arg) const
        {
            return func(arg);
        }
    };

    static void updateSettingInfo();

    static const SettingInfo &findSetting(const QString &name)
    {
        return findSetting(name, getSettings());
    }

    static const SettingInfo &findSetting(const QString &name, const QList<SettingInfo> &infos)
    {
        static SettingInfo empty;
        for (const SettingInfo &si : infos)
            if (si.name == name)
                return si;
        return empty;
    }

    static const QList<SettingInfo> &getSettings()
    {
        return fakeRoot.child;
    }

    static SettingIter beginIter(const QString &name)
    {
        return SettingIter(&findSetting(name, getSettings()));
    }

    static SettingIter fakeRootIter()
    { // This fake root has no type. Only used for it's child.
        return SettingIter(&fakeRoot);
    }

  private:
    static SettingInfo fakeRoot;
    static QList<SettingInfo> &settings;

    friend class SettingsUpdater;
};

#endif // SETTINGSINFO_HPP

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
#include <QFileInfo>
#include <QFont>
#include <QRect>
#include <QSettings>
#include <yaml-cpp/yaml.h>

namespace YAML
{

template <> struct convert<QString>
{
    static Node encode(const QString &s)
    {
        Node node;
        node = s.toUtf8().toStdString();
        return node;
    }

    static bool decode(const Node &node, QString &s)
    {
        if (!node.IsScalar())
        {
            return false;
        }
        s = QString::fromUtf8(QByteArray::fromStdString(node.as<std::string>()));
        return true;
    }
};

template <> struct convert<QFont>
{
    static Node encode(const QFont &f)
    {
        Node node;
        node = f.toString();
        return node;
    }

    static bool decode(const Node &node, QFont &f)
    {
        if (!node.IsScalar())
        {
            return false;
        }
        f.fromString(node.as<QString>());
        return true;
    }
};

template <> struct convert<QRect>
{
    static Node encode(const QRect &r)
    {
        Node node;
        node.push_back(r.left());
        node.push_back(r.top());
        node.push_back(r.right());
        node.push_back(r.bottom());
        return node;
    }

    static bool decode(const Node &node, QRect &r)
    {
        if (!node.IsSequence() || node.size() != 4)
        {
            return false;
        }
        r = QRect(node[0].as<int>(), node[1].as<int>(), node[2].as<int>(), node[3].as<int>());
        return true;
    }
};

template <> struct convert<QByteArray>
{
    static Node encode(const QByteArray &d)
    {
        Node node;
        node = d.toHex().toStdString();
        return node;
    }

    static bool decode(const Node &node, QByteArray &d)
    {
        if (!node.IsScalar())
        {
            return false;
        }
        d = QByteArray::fromHex(QByteArray::fromStdString(node.as<std::string>()));
        return true;
    }
};

template <typename Type> struct convert<QList<Type>>
{
    static Node encode(const QList<Type> &l)
    {
        Node node(YAML::NodeType::Sequence);
        for (const auto &v : l)
            node.push_back(v);
        return node;
    }

    static bool decode(const Node &node, QList<Type> &l)
    {
        if (!node.IsSequence())
        {
            return false;
        }
        l.clear();
        for (auto it = node.begin(); it != node.end(); it++)
            l.push_back(it->as<Type>());
        return true;
    }
};

template <> struct convert<QStringList>
{
    static Node encode(const QStringList &l)
    {
        Node node(YAML::NodeType::Sequence);
        for (const auto &v : l)
            node.push_back(v);
        return node;
    }

    static bool decode(const Node &node, QStringList &l)
    {
        if (!node.IsSequence())
        {
            return false;
        }
        l.clear();
        for (auto it = node.begin(); it != node.end(); it++)
            l.push_back(it->as<QString>());
        return true;
    }
};
} // namespace YAML

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
    "$APPCONFIG/cp_editor_settings.yml", "$APPCONFIG/cp_editor_settings.ini", "$HOME/.cp_editor_settings.ini",
    "$HOME/cp_editor_settings.ini"};

void SettingsManager::load(const YAML::Node &setting, const QString &prefix,
                           const QList<SettingsInfo::SettingInfo> &infos)
{
    if (!setting.IsMap())
        return;
    for (const auto &si : infos)
    {
        if (!setting[si.name])
            continue;
        const auto &node = setting[si.name];
        if (si.type == "Object")
        {
            if (!node.IsMap() || !setting[si.name + '@'] || !setting[si.name + '@'].IsSequence())
                continue;
            QStringList keys = setting[si.name + '@'].as<QStringList>();
            for (const QString &sub : keys)
            {
                if (!node[sub])
                    continue;
                load(node[sub], QString("%1%2/%3/").arg(prefix, si.name, sub), si.child);
            }
            auto oldKeys = get(prefix + si.name + '@').toStringList();
            for (const auto &k : oldKeys)
            {
                if (keys.indexOf(k) == -1)
                    keys.push_back(k);
            }
            set(prefix + si.name + '@', keys);
        }
        else
        {
            QVariant ret(QVariant::Invalid);
            switch (node.Type())
            {
            case YAML::NodeType::Scalar:
                if (si.type == "bool")
                    ret = node.as<bool>();
                else if (si.type == "int")
                    ret = node.as<int>();
                else if (si.type == "QString")
                    ret = node.as<QString>();
                else if (si.type == "QByteArray")
                    ret = node.as<QByteArray>();
                else if (si.type == "QFont")
                    ret = node.as<QFont>();
                else
                    Q_UNREACHABLE();
                break;
            case YAML::NodeType::Sequence:
                if (si.type == "QVariantList")
                {
                    auto lsl = node.as<QList<QStringList>>();
                    QVariantList vl;
                    for (const auto &sl : lsl)
                        vl.push_back(sl);
                    ret = vl;
                }
                else if (si.type == "QStringList")
                    ret = node.as<QStringList>();
                else if (si.type == "QRect")
                    ret = node.as<QRect>();
                else
                    Q_UNREACHABLE();
            default:
                break;
            }
            set(prefix + si.name, ret);
        }
    }
}
void SettingsManager::load_INI(QSettings &setting, const QString &prefix, const QList<SettingsInfo::SettingInfo> &infos)
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
                load_INI(setting, QString("%1%2/%3/").arg(prefix, si.name, sub), si.child);
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

void SettingsManager::save(YAML::Emitter &setting, const QString &prefix, const QList<SettingsInfo::SettingInfo> &infos)
{
    for (const auto &si : infos)
        if (si.type == "Object")
        {
            QString head = prefix + si.name;
            QStringList keys = get(head + '@').toStringList();
            setting << YAML::Key << YAML::Node(si.name + '@') << YAML::Value << YAML::Node(keys);
            setting << YAML::Key << YAML::Node(si.name) << YAML::BeginMap;
            for (const QString &k : keys)
            {
                setting << YAML::Key << YAML::Node(k) << YAML::Value << YAML::BeginMap;
                save(setting, QString("%1/%2/").arg(head, k), si.child);
                setting << YAML::EndMap;
            }
            setting << YAML::EndMap;
        }
        else
        {
            setting << YAML::Key << YAML::Node(si.name) << YAML::Value;
            auto v = get(prefix + si.name);
            if (si.type == "bool")
                setting << v.toBool();
            else if (si.type == "int")
                setting << v.toInt();
            else if (si.type == "QRect")
                setting << YAML::Node(v.toRect());
            else if (si.type == "QString")
                setting << YAML::Node(v.toString());
            else if (si.type == "QStringList")
                setting << YAML::Node(v.toStringList());
            else if (si.type == "QByteArray")
                setting << YAML::Node(v.toByteArray());
            else if (si.type == "QFont")
                setting << YAML::Node(v.value<QFont>());
            else if (si.type == "QVariantList")
            {
                setting << YAML::BeginSeq;
                for (const auto &l : v.toList())
                    setting << YAML::Node(l.toStringList());
                setting << YAML::EndSeq;
            }
            else
                Q_UNREACHABLE();
        }
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
    {
        QFileInfo i(path);
        auto sfx = i.suffix();
        if (sfx == "yml")
            loadSettings(path);
        else if (sfx == "ini")
            loadSettings_INI(path);
    }

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

    YAML::Node setting = YAML::LoadFile(path.toUtf8().toStdString());
    SettingsUpdater::updateSetting();
    load(setting, "", SettingsInfo::getSettings());
    SettingsUpdater::updateSettingFinal();

    // load file problem binding
    if (setting["File Problem Binding"] && !setting["File Problem Binding"].IsNull())
    {
        qDebug() << setting["File Problem Binding"].Type();
        FileProblemBinder::fromList(setting["File Problem Binding"].as<QStringList>()); // TODO: solve this
    }

    LOG_INFO("Settings have been loaded from " + path);
}

void SettingsManager::loadSettings_INI(const QString &path)
{
    LOG_INFO("Start loading settings from " + path);

    QSettings setting(path, QSettings::IniFormat);
    SettingsUpdater::updateSetting_INI(setting);
    load_INI(setting, "", SettingsInfo::getSettings());
    SettingsUpdater::updateSettingFinal();

    // load file problem binding
    FileProblemBinder::fromList(setting.value("file_problem_binding").toStringList()); // TODO: solve this

    LOG_INFO("Settings have been loaded from " + path);
}

void SettingsManager::saveSettings(const QString &path)
{
    const auto savePath = path.isEmpty() ? Util::configFilePath(configFileLocations[0]) : path;

    LOG_INFO("Start saving settings to " + savePath);

    YAML::Emitter setting;
    setting << YAML::BeginMap;
    save(setting, "", SettingsInfo::getSettings());

    // save file problem binding
    setting << YAML::Key << "File Problem Binding" << YAML::Value << YAML::Node(FileProblemBinder::toList());

    setting << YAML::EndMap;

    QFile file(savePath);
    file.open(QIODevice::WriteOnly);
    file.write(setting.c_str());
    file.close();

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

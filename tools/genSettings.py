#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys
import json

def writeHelper(f, obj, pre, indent):
    ids = "    " * indent
    for t in obj:
        name = t["name"]
        key = name.replace(" ", "").replace("/", "").replace("+", "p")
        typename = t["type"]
        if typename == "config":
            f.write(f"{ids}struct {key} {{\n")
            f.write(f"{ids}    QString pre;\n")
            f.write(f"{ids}    {key}(QString p) : pre(p) {{}}\n")
            writeHelper(f, t["sub"], "pre, ", indent + 1)
            f.write(f"{ids}}};\n")
            f.write(f"{ids}inline {key} get{key}(QString key) {{ return {key}(QStringList {{{pre} {json.dumps(name)}, key}}.join('/')); }}\n")
            f.write(f"{ids}inline void remove{key}(QString key) {{ SettingsManager::remove(SettingsManager::keyStartsWith(QStringList {{{pre} {json.dumps(name)}, key}}.join('/'))); }}\n")
            f.write(f"{ids}inline QStringList query{key}() {{ return SettingsManager::itemUnder(QStringList {{{pre} {json.dumps(name)}}}.join('/') + '/'); }}\n")
        elif typename == "QMap":
            final = t["final"]
            f.write(f"{ids}inline void set{key}(QString key, {final} data) {{ SettingsManager::set(QStringList {{{pre} {json.dumps(name)}, key}}.join('/'), data); }}\n")
            f.write(f"{ids}inline void remove{key}(QString key) {{ SettingsManager::remove({{QStringList {{{pre} {json.dumps(name)}, key}}.join('/')}}); }}\n")
            f.write(f"{ids}inline {final} get{key}(QString key) {{ return SettingsManager::get(QStringList {{{pre} {json.dumps(name)}, key}}.join('/')).value<{final}>(); }}\n")
            f.write(f"{ids}inline QStringList query{key}() {{ return SettingsManager::itemUnder(QStringList {{{pre} {json.dumps(name)}}}.join('/') + '/'); }}\n")
        else:
            f.write(
                f"{ids}inline void set{key}({typename} value) {{ SettingsManager::set({json.dumps(name)}, value); }}\n")
            if typename == "bool":
                f.write(
                    f"{ids}inline bool is{key}() {{ return SettingsManager::get({json.dumps(name)}).toBool(); }}\n")
            else:
                f.write(
                    f"{ids}inline {typename} get{key}() {{ return SettingsManager::get({json.dumps(name)}).value<{typename}>(); }}\n")

def writeInfo(f, obj, lst):
    for t in obj:
        name = t["name"]
        key = name.replace(" ", "").replace("/", "").replace("+", "p")
        typename = t["type"]
        if "desc" in t:
            desc = t["desc"]
        else:
            desc = name.replace('/', ' ')
        if "ui" in t:
            ui = t["ui"]
        else:
            ui = ""
        if "tip" in t:
            tip = t["tip"]
        else:
            tip = ""
        if "help" in t:
            hlp = t["hlp"]
        else:
            hlp = ""
        if typename == "config":
            f.write(f"    QList<SettingInfo> LIST{key};\n")
            writeInfo(f, t["sub"], f"LIST{key}")
        f.write(f"    {lst}.append(SettingInfo {{{json.dumps(name)}, ")
        if "C++" in json.dumps(desc):
            f.write(f"QCoreApplication::translate(\"Setting\", {json.dumps(desc).replace('C++', '%1')}).arg(\"C++\")")
        elif "Java" in json.dumps(desc):
            f.write(f"QCoreApplication::translate(\"Setting\", {json.dumps(desc).replace('Java', '%1')}).arg(\"Java\")")
        elif "Python" in json.dumps(desc):
            f.write(f"QCoreApplication::translate(\"Setting\", {json.dumps(desc).replace('Python', '%1')}).arg(\"Python\")")
        else:
            f.write(f"QCoreApplication::translate(\"Setting\", {json.dumps(desc)})")
        tempname = typename
        if typename == "QMap":
            final = t["final"]
            tempname = f"QMap:{final}"
        f.write(f", \"{tempname}\", \"{ui}\", QCoreApplication::translate(\"Setting\", {json.dumps(tip)}), QCoreApplication::translate(\"Setting\", {json.dumps(hlp)}), {{")

        if "old" in t:
            olds = ""
            for s in t["old"]:
                olds = olds + '"' + s + '", '
            f.write(olds)
        f.write("}, ")
        if typename != "config":
            if "default" in t:
                if typename == "QString":
                    f.write(json.dumps(t["default"]))
                else:
                    if isinstance(t["default"], bool):
                        f.write(str(t["default"]).lower())
                    else:
                        f.write(str(t["default"]))
            else:
                defs = {
                    'QString': '""',
                    'int': '0',
                    'bool': 'false',
                    'QRect': 'QRect()',
                    'QByteArray': 'QByteArray()',
                    'QVariantList': 'QVariantList()',
                    'QMap': 'QVariantMap()'
                }
                f.write(defs[typename])
        else:
            f.write(f'QVariant()')
        if "param" in t:
            f.write(f', {t["param"]}')
        else:
            f.write(f", QVariant()")
        if typename == "config":
            f.write(f", LIST{key}")
        f.write("});\n")

if __name__ == "__main__":
    obj = json.load(open(sys.argv[1], mode="r", encoding="utf-8"))
    head = """/*
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

/*
 * This file is auto generated by tools/genSettings.py
 */

"""
    setting_helper = open("generated/SettingsHelper.hpp", mode="w", encoding="utf-8")
    setting_helper.write(head)
    setting_helper.write("""#ifndef SETTINGSHELPER_HPP
#define SETTINGSHELPER_HPP

#include "Settings/SettingsManager.hpp"
#include <QFont>
#include <QRect>
#include <QVariant>

namespace SettingsHelper
{
""")
    writeHelper(setting_helper, obj, "", 1)
    setting_helper.write("""}

#endif // SETTINGSHELPER_HPP""")
    setting_helper.close()
    setting_info = open("generated/SettingsInfo.hpp", mode="w", encoding="utf-8")
    setting_info.write(head)
    setting_info.write("""#ifndef SETTINGSINFO_HPP
#define SETTINGSINFO_HPP

#include <QCoreApplication>
#include <QFont>
#include <QRect>
#include <QVariant>
#include "Core/StyleManager.hpp"

struct SettingInfo
{
    QString name, desc, type, ui, tip, help;
    QStringList old;
    QVariant def;
    QVariant param;
    QList<SettingInfo> child;

    QString key() const
    {
        return name.toLower().replace('+', 'p').replace(' ', '_');
    }
};

extern QList<SettingInfo> settingInfo;

void updateSettingInfo();

inline SettingInfo findSetting(const QString &name, const QList<SettingInfo> &infos = settingInfo)
{
    for (const SettingInfo &si: infos)
        if (si.name == name)
            return si;
    return SettingInfo();
}

#endif // SETTINGSINFO_HPP""")
    setting_info.close()

    setting_info = open("generated/SettingsInfo.cpp", mode="w", encoding="utf-8")
    setting_info.write(head)
    setting_info.write("""#include "SettingsInfo.hpp"

QList<SettingInfo> settingInfo;

void updateSettingInfo()
{
    settingInfo.clear();
""")
    writeInfo(setting_info, obj, "settingInfo")
    setting_info.write("};\n")
    setting_info.close()

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
        if typename == "Object":
            f = f + f"{ids}struct {key} {{\n"
            f = f + f"{ids}    QString pre;\n"
            f = f + f"{ids}    {key}(QString p) : pre(p) {{}}\n"
            f = writeHelper(f, t["sub"], "pre, ", indent + 1)
            f = f + f"{ids}}};\n"
            f = f + f"{ids}inline {key} get{key}(QString key) {{ return {key}(QStringList {{{pre} {json.dumps(name)}, key}}.join('/')); }}\n"
            f = f + f"{ids}inline void remove{key}(QString key) {{ SettingsManager::remove(SettingsManager::keyStartsWith(QStringList {{{pre} {json.dumps(name)}, key}}.join('/'))); }}\n"
            f = f + f"{ids}inline QStringList query{key}() {{ return SettingsManager::itemUnder(QStringList {{{pre} {json.dumps(name)}}}.join('/') + '/'); }}\n"
        elif typename == "QMap":
            final = t["final"]
            pname = t.get("plural", name)
            pkey = pname.replace(" ", "").replace("/", "").replace("+", "p")
            f = f + f"{ids}inline void set{key}(QString key, {final} data) {{ SettingsManager::set(QStringList {{{pre} {json.dumps(name)}, key}}.join('/'), data); }}\n"
            f = f + f"{ids}inline void remove{key}(QString key) {{ SettingsManager::remove({{QStringList {{{pre} {json.dumps(name)}, key}}.join('/')}}); }}\n"
            f = f + f"{ids}inline {final} get{key}(QString key) {{ return SettingsManager::get(QStringList {{{pre} {json.dumps(name)}, key}}.join('/')).value<{final}>(); }}\n"
            f = f + f"{ids}inline QStringList get{pkey}() {{ return SettingsManager::itemUnder(QStringList {{{pre} {json.dumps(name)}}}.join('/') + '/'); }}\n"
        else:
            f = f + f"{ids}inline void set{key}({typename} value) {{ SettingsManager::set({json.dumps(name)}, value); }}\n"
            if typename == "bool":
                f = f + f"{ids}inline bool is{key}() {{ return SettingsManager::get({json.dumps(name)}).toBool(); }}\n"
            else:
                f = f + f"{ids}inline {typename} get{key}() {{ return SettingsManager::get({json.dumps(name)}).value<{typename}>(); }}\n"
    return f

def writeInfo(f, obj, lst):
    for t in obj:
        name = t["name"]
        key = name.replace(" ", "").replace("/", "").replace("+", "p")
        typename = t["type"]
        desc = t.get("desc", name.replace('/', ' '))
        trdesc = t.get("trdesc", f"tr({json.dumps(desc)})")
        ui = t.get("ui", "")
        tip = t.get("tip", "")
        trtip = t.get("trtip", f"tr({json.dumps(tip)})")
        hlp = t.get("help", "")
        requireAllDepends = t.get("requireAllDepends", True)
        depends = t.get("depends", [])
        if typename == "Object":
            f = f + f"    QList<SettingInfo> LIST{key};\n"
            f = writeInfo(f, t["sub"], f"LIST{key}")
        f = f + f"    {lst}.append(SettingInfo {{{json.dumps(name)}, "
        if "trdesc" not in t:
            for lang in ["C++", "Java", "Python"]:
                if lang in desc:
                    trdesc = f"tr({json.dumps(desc.replace(lang, '%1'))}).arg(\"{lang}\")"
                    break
        f = f + trdesc
        tempname = typename
        if typename == "QMap":
            final = t["final"]
            tempname = f"QMap:{final}"
        dependsString = "{"
        for depend in depends:
            dependsString += f"{{{json.dumps(depend.get('name', ''))}, [](const QVariant &var) {{ {depend.get('check', 'return var.toBool();')} }}}}, "
        dependsString += "}"
        f = f + f", \"{tempname}\", \"{ui}\", {trtip}, tr({json.dumps(hlp)}), {json.dumps(requireAllDepends)}, {dependsString}, "
        if typename != "Object":
            if "default" in t:
                if typename == "QString":
                    f = f + json.dumps(t["default"])
                else:
                    if isinstance(t["default"], bool):
                        f = f + str(t["default"]).lower()
                    else:
                        f = f + str(t["default"])
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
                f = f + defs[typename]
        else:
            f = f + "QVariant()"
        f = f + f', {t.get("param", "QVariant()")}'
        if typename == "Object":
            f = f + f", LIST{key}"
        f = f + "});\n"
    return f

def addDefaultPaths(obj):
    actions = [
        ("Open File", "${file}", "file, testcase, checker"),
        ("Save File", "${file}", "file, testcase, checker"),
        ("Open Contest", "${contest}", "contest, file, testcase, checker"),
        ("Load Single Test Case", "${testcase}", "testcase"),
        ("Add Pairs Of Test Cases", "${testcase}", "testcase"),
        ("Custom Checker", "${checker}", "checker"),
        ("Export And Import Settings", "${settings}", "settings"),
        ("Export And Load Session", "${session}", "session"),
        ("Extract And Load Snippets", "${snippets}", "snippets"),
    ]

    for action in actions:
        obj.append({
            "name": f"Default Path/Action/{action[0]}/Uses",
            "trdesc": f'tr("Default path used for %1").arg(tr("{action[0]}"))',
            "type": "QString",
            "default": action[1],
            "trtip": f'tr("The default path used when choosing a path for %1.\\nYou can use ${{<default path name>}} as a place holder.").arg(tr("{action[0]}"))'
        })
        if action[0] == "Save File":
            obj[-1]["trtip"] += ' + "\\n" + tr("It can be overridden by %1.").arg(tr("Default File Paths For Problem URLs"))'
        obj.append({
            "name": f"Default Path/Action/{action[0]}/Changes",
            "trdesc": f'tr("Default paths changed by %1").arg(tr("{action[0]}"))',
            "type": "QString",
            "default": action[2],
            "trtip": f'tr("The default paths changed after choosing a path for %1.\\nIt is a list of <default path name>s, separated by commas, and can be empty.").arg(tr("{action[0]}"))'
        })

def updateIfNeeded(path, text):
    file = open(path, mode="r", encoding="utf-8")
    buf = file.read()
    file.close()
    if buf == text:
        print(f"Skip update {path} as no changes applied")
        return
    file = open(path, mode="w", encoding="utf-8")
    file.write(text)
    file.close()

if __name__ == "__main__":
    obj = json.load(open(sys.argv[1], mode="r", encoding="utf-8"))

    addDefaultPaths(obj)

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
    buf = head
    buf = buf + """#ifndef SETTINGSHELPER_HPP
#define SETTINGSHELPER_HPP

#include "Settings/SettingsManager.hpp"
#include <QFont>
#include <QRect>
#include <QVariant>

namespace SettingsHelper
{
"""
    buf = writeHelper(buf, obj, "", 1)
    buf = buf + """}

#endif // SETTINGSHELPER_HPP"""
    updateIfNeeded("generated/SettingsHelper.hpp", buf)

    buf = head
    buf = buf + """#include "Settings/SettingsInfo.hpp"
#include "Core/StyleManager.hpp"
#include "Settings/PathItem.hpp"
#include <QFontDatabase>
#include <QRect>

QList<SettingsInfo::SettingInfo> SettingsInfo::settings;

void SettingsInfo::updateSettingInfo()
{
    settings.clear();
"""
    buf = writeInfo(buf, obj, "settings")
    buf = buf + "};\n"
    updateIfNeeded("generated/SettingsInfo.cpp", buf)

    file = open("generated/GENSETTING", mode="w")
    file.close()
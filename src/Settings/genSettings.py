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
            f.write(f"{ids}struct {key} {{\n")
            f.write(f"{ids}    QString pre;\n")
            f.write(f"{ids}    {key}(QString p) : pre(p) {{}}\n")
            writeHelper(f, t["sub"], "pre, ", indent + 1)
            f.write(f"{ids}}};\n")
            f.write(
                f"{ids}inline {key} get{key}(QString key) {{ return {key}(QStringList {{{pre} {json.dumps(name)}, key}}.join('/')); }}\n")
            f.write(f"{ids}inline void remove{key}(QString key) {{ SettingsManager::remove(SettingsManager::keyStartsWith(QStringList {{{pre} {json.dumps(name)}, key}}.join('/'))); }}\n")
            f.write(f"{ids}inline QStringList query{key}() {{ return SettingsManager::itemUnder(QStringList {{{pre} {json.dumps(name)}}}.join('/') + '/'); }}\n")
        else:
            f.write(
                f"{ids}inline void set{key}({typename} value) {{ SettingsManager::set(QStringList {{{pre} {json.dumps(name)}}}.join('/'), value); }}\n")
            if typename == "bool":
                f.write(
                    f"{ids}inline bool is{key}() {{ return SettingsManager::get(QStringList {{{pre} {json.dumps(name)}}}.join('/')).toBool(); }}\n")
            else:
                f.write(
                    f"{ids}inline {typename} get{key}() {{ return SettingsManager::get(QStringList {{{pre} {json.dumps(name)}}}.join('/')).value<{typename}>(); }}\n")
        f.write(
            f"{ids}inline QString pathOf{key}(bool parent = false) {{ return SettingsManager::getPathText(QStringList {{{pre} {json.dumps(name)}}}.join('/'), parent); }}\n")

# "hello" -> "hello"
# tr("123") -> "@tr(\"123\")"
# QFont() -> "#QFont()"
# "#hello" -> "%#hello"

def parseParam(obj):
    if type(obj) == str:
        if len(obj) > 0:
            if obj[0] == "@":
                return obj[1:], True
            elif obj[0] == "#":
                return obj[1:], False
            elif obj[0] == "%":
                return json.dumps(obj[1:]), True
        return json.dumps(obj), True
    elif type(obj) == dict:
        ret = "QMap<QString, QVariant>{"
        for k in obj:
            ret = ret + f"{{{json.dumps(k)},{parseParam(obj[k])[0]}}},"
        ret = ret + "}"
        return ret, False
    elif type(obj) == list:
        allstr = True
        ret = "{"
        for k in obj:
            r, t = parseParam(k)
            ret = ret + f"{r},"
            if not t:
                allstr = False
        if allstr:
            ret = "QStringList" + ret + "}"
        else:
            ret = "QList<QVariant>" + ret + "}"
        return ret, False
    elif type(obj) == bool:
        return str.lower(str(obj)), False
    return str(obj), False

def writeInfo(f, obj, lst):
    for t in obj:
        name = t["name"]
        key = name.replace(" ", "").replace("/", "").replace("+", "p")
        typename = t["type"]
        desc = t.get("desc", name.replace('/', ' '))
        trdesc = t.get("trdesc", f"tr({json.dumps(desc)})" if not t.get(
            "notr", False) else json.dumps(desc))
        ui = t.get("ui", "")
        tip = t.get("tip", "")
        trtip = t.get("trtip", f"tr({json.dumps(tip)})")
        docAnchor = t.get("docAnchor", "")
        if docAnchor == "":
            docAnchor = json.dumps("")
        else:
            docAnchor = f"tr({json.dumps(docAnchor)}, {json.dumps(f'the anchor of {desc} on the corresponding page of https://cpeditor.org/docs/preferences')})"
        requireAllDepends = t.get("requireAllDepends", True)
        immediatelyApply = t.get("immediatelyApply", False)
        methods = "QMap<QString, std::function<QVariant(QVariant)>>{"
        for fn in t.get("methods", []):
            func = t["methods"][fn]
            predef = ""
            for v in func.get("param", {}):
                vt = func["param"][v].strip()
                at = "auto "
                if vt[-1] == "*":
                    at = "auto *"
                predef = predef + f"{at}{v}(_.toMap()[{json.dumps(v)}].value<{vt}>());"
            methods = methods + f"{{{json.dumps(fn)},[](const QVariant &_)->QVariant{{{predef}do{{{func['code']}}}while(false);return QVariant();}}}},"
        methods = methods + "}"
        depends = t.get("depends", [])
        if typename == "Object":
            f.write(f"    QList<SettingInfo> LIST{key};\n")
            writeInfo(f, t["sub"], f"LIST{key}")
        if "trdesc" not in t and "notr" not in t:
            for lang in ["C++", "Java", "Python"]:
                if lang in desc:
                    trdesc = f"tr({json.dumps(desc.replace(lang, '%1'))}).arg(\"{lang}\")"
                    break
        tempname = typename
        dependsString = "{"
        for depend in depends:
            dependsString += f"{{{json.dumps(depend.get('name', ''))}, [](const QVariant &var) {{ {depend.get('check', 'return var.toBool();')} }}}}, "
        dependsString += "}"
        old = t.get("old", [])
        f.write(
            f"    {lst}.append(SettingInfo {{{json.dumps(name)}, {trdesc}, {json.dumps(desc)}, \"{tempname}\", \"{ui}\", {trtip}, {json.dumps(tip)}, {docAnchor}, {json.dumps(requireAllDepends)}, {json.dumps(immediatelyApply)}, {methods}, {dependsString}, {{{json.dumps(old)[1:-1]}}}, ")
        if typename != "Object":
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
                    'QMap': 'QVariantMap()'
                }
                if typename in defs:
                    f.write(defs[typename])
                else:
                    f.write(typename + '()')
        else:
            f.write('QVariant()')
        f.write(f', {parseParam(t.get("param", "#QVariant()"))[0]}')
        if typename == "Object":
            f.write(f", LIST{key}")
        f.write("});\n")


def addDefaultPaths(obj):
    actions = [
        ("Open File", "${file}", "file, testcase, checker"),
        ("Save File", "${file}", "file, testcase, checker"),
        ("Open Contest", "${contest}", "contest, file, testcase, checker"),
        ("Load Single Test Case", "${testcase}", "testcase"),
        ("Add Pairs Of Test Cases", "${testcase}", "testcase"),
        ("Save Test Case To A File", "${testcase}", "testcase"),
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


if __name__ == "__main__":
    obj = json.load(open(sys.argv[1], mode="r", encoding="utf-8"))

    addDefaultPaths(obj)

    head = """/*
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

/*
 * This file is auto generated by src/Settings/genSettings.py
 */

"""
    setting_helper = open("generated/SettingsHelper.hpp",
                          mode="w", encoding="utf-8")
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

    setting_info = open("generated/SettingsInfo.cpp",
                        mode="w", encoding="utf-8")
    setting_info.write(head)
    setting_info.write("""#include "Settings/SettingsInfo.hpp"
#include "Core/StyleManager.hpp"
#include "Core/Translator.hpp"
#include "Settings/PathItem.hpp"
#include "Settings/SettingWidget.hpp"
#include "Settings/SettingsMethods.hpp"
#include <QFontDatabase>
#include <QMessageBox>
#include <QRect>

SettingsInfo::SettingInfo SettingsInfo::fakeRoot = { "@ROOT@", "", "", "Object" };
QList<SettingsInfo::SettingInfo> &SettingsInfo::settings = SettingsInfo::fakeRoot.child;

void SettingsInfo::updateSettingInfo()
{
    settings.clear();
""")
    writeInfo(setting_info, obj, "settings")
    setting_info.write("};\n")
    setting_info.close()

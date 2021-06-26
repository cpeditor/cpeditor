#include <algorithm>
#include <fstream>
#include <iterator>
#include <map>
#include <regex>
#include <yaml-cpp/yaml.h>
using namespace std;

string dump(string s)
{
    string ret = "\"";
    for (char c : s)
    {
        switch (c)
        {
        case '"':
            ret += "\\\"";
            break;
        case '\\':
            ret += "\\\\";
            break;
        case '\t':
            ret += "\\t";
            break;
        case '\n':
            ret += "\\n";
            break;
        default:
            ret += c;
        }
    }
    return ret + '"';
}

void writeHelper(ofstream &f, const YAML::Node &obj, const string &pre, int indent)
{
    if (!obj.IsSequence())
        return;
    string ids(4 * indent, ' ');
    string cst = " const";
    if (indent == 1)
        cst = "";
    for (const auto &t : obj)
    {
        if (!t.IsMap())
            continue;
        auto name = t["name"].as<string>();
        string key;
        transform(name.begin(), name.end(), back_insert_iterator<string>(key),
                  [](char c) { return c == '+' ? 'p' : c; });
        key.erase(remove_if(key.begin(), key.end(), [](char c) { return c == ' ' || c == '/'; }), key.end());
        auto type = t["type"].as<string>();
        if (type == "Object")
        {
            f << ids << "struct " << key << " {" << endl;
            f << ids << "    QString pre;" << endl;
            f << ids << "    " << key << "(QString  p) : pre(p) {}" << endl;
            writeHelper(f, t["sub"], "pre, ", indent + 1);
            f << ids << "};" << endl;
            f << ids << "inline " << key << " get" << key << "(QString key)" << cst << " { return " << key
              << "(QStringList {" << pre << " " << dump(name) << ", key}.join('/')); }" << endl;
            f << ids << "inline QStringList query" << key << "()" << cst
              << " { return SettingsManager::get(QStringList {" << pre << " " << dump(name)
              << "}.join('/') + '@').toStringList(); }" << endl;
            f << ids << "inline void set" << key << "(const QStringList &items)" << cst
              << " { SettingsManager::set(QStringList {" << pre << " " << dump(name) << "}.join('/') + '@', items); }"
              << endl;
            if (t["param"] && t["param"].IsMap() && t["param"]["restrict"] && t["param"]["restrict"].IsSequence())
            {
                for (const auto &kt : t["param"]["restrict"])
                {
                    auto k = kt.as<string>();
                    if (k[0] == '!')
                        k = k.substr(1);
                    string ks;
                    transform(k.begin(), k.end(), back_insert_iterator<string>(ks),
                              [](char c) { return c == '+' ? 'p' : (c == ' ' ? '_' : c); });
                    f << ids << "inline " << key << " get" << key << "_" << ks << "()" << cst << " { return get" << key
                      << "(" << dump(k) << "); }" << endl;
                }
            }
        }
        else
        {
            f << ids << "inline void set" << key << "(" << type << " value)" << cst
              << " { SettingsManager::set(QStringList {" << pre << " " << dump(name) << "}.join('/'), value); }"
              << endl;
            f << ids << "inline " << type << " " << (type == "bool" ? "is" : "get") << key << "()" << cst
              << " { return SettingsManager::get(QStringList {" << pre << " " << dump(name) << "}.join('/')).value<"
              << type << ">(); }" << endl;
        }
        f << ids << "inline QString pathOf" << key << "(bool parent = false)" << cst
          << " { return SettingsManager::getPathText(QStringList {" << pre << " " << dump(name)
          << "}.join('/'), parent); }" << endl;
    }
}

bool isNumber(const string &s)
{
    auto b = s.begin();
    if (*b == '+' || *b == '-')
        b++;
    return b != s.end() && !any_of(b, s.end(), [](char c) { return !isdigit(c); });
}

string parseParam(const YAML::Node &obj, bool &isStr)
{
    if (obj.IsScalar())
    {
        auto s = obj.as<string>();
        if (s.length() >= 0)
        {
            auto ds = dump(s.substr(1));
            switch (s[0])
            {
            case '@':
                isStr = true;
                return s.substr(1);
            case '#':
                isStr = false;
                return s.substr(1);
            case '%':
                isStr = true;
                return ds;
            case '*':
                isStr = true;
                return "tr(" + ds + ")";
            case '!':
                isStr = false;
                return "QStringList{" + ds + ", tr(" + ds + ")}";
            }
        }
        // yaml-cpp cannot distinguish between "123" and 123
        // Luckily we have % to solve it
        if (s == "true" || s == "false" || isNumber(s))
        {
            isStr = false;
            return s;
        }
        else
        {
            isStr = true;
            return dump(s);
        }
    }
    else if (obj.IsMap())
    {
        string ret = "QMap<QString, QVariant>{";
        for (const auto &pr : obj)
            ret += "{" + dump(pr.first.as<string>()) + "," + parseParam(pr.second, isStr) + "},";
        isStr = false;
        return ret + "}";
    }
    else if (obj.IsSequence())
    {
        bool allStr = true;
        string ret = "{";
        for (const auto &t : obj)
        {
            ret += parseParam(t, isStr) + ",";
            if (!isStr)
                allStr = false;
        }
        if (allStr)
            ret = "QStringList" + ret + "}";
        else
            ret = "QList<QVariant>" + ret + "}";
        isStr = false;
        return ret;
    }
    isStr = false;
    return obj.as<string>();
}

string buildMethod(const YAML::Node &func)
{
    string predef;
    if (func["param"] && func["param"].IsMap())
    {
        for (const auto &pr : func["param"])
        {
            string vname = pr.first.as<string>();
            string vtype = pr.second.as<string>();
            vtype = vtype.substr(vtype.find_first_not_of(' '));
            vtype = vtype.substr(0, vtype.find_last_not_of(' ') + 1);
            string a = "auto ";
            if (vtype.back() == '*')
                a = "auto *";
            predef += a + vname + "(_.toMap()[" + dump(vname) + "].value<" + vtype + ">());";
        }
    }
    return "[](const QVariant &_)->QVariant{" + predef + "do{" + func["code"].as<string>() +
           "}while(false);return QVariant();}";
}

void writeInfo(ofstream &f, const YAML::Node &obj, const string &lst)
{
    if (!obj.IsSequence())
        return;
    for (const auto &t : obj)
    {
        if (!t.IsMap())
            continue;
        auto name = t["name"].as<string>();
        string key;
        transform(name.begin(), name.end(), back_insert_iterator<string>(key),
                  [](char c) { return c == '+' ? 'p' : c; });
        key.erase(remove_if(key.begin(), key.end(), [](char c) { return c == ' ' || c == '/'; }), key.end());
        auto type = t["type"].as<string>();
        string desc;
        if (t["desc"])
            desc = t["desc"].as<string>();
        else
        {
            transform(name.begin(), name.end(), back_insert_iterator<string>(desc),
                      [](char c) { return c == '/' ? ' ' : c; });
        }
        string trdesc;
        if (t["trdesc"])
            trdesc = t["trdesc"].as<string>();
        else if (t["notr"] && t["notr"].as<bool>())
            trdesc = dump(desc);
        else
            trdesc = "tr(" + dump(desc) + ")";
#define GET(k, d)                                                                                                      \
    string k(d);                                                                                                       \
    if (t[#k])                                                                                                         \
        k = t[#k].as<string>();
        GET(ui, "")
        GET(tip, "")
        GET(trtip, "tr(" + dump(tip) + ")")
        GET(docAnchor, "")
        if (docAnchor == "")
            docAnchor = "\"\"";
        else
            docAnchor =
                "tr(" + dump(docAnchor) + ", " +
                dump("the anchor of " + desc + " on the corresponding page of https://cpeditor.org/docs/preferences") +
                ")";
#undef GET
#define GET(k, d)                                                                                                      \
    bool k(d);                                                                                                         \
    if (t[#k])                                                                                                         \
        k = t[#k].as<bool>();
        GET(requireAllDepends, true)
        GET(immediatelyApply, false)
#undef GET
        string methods = "QMap<QString, std::function<QVariant(QVariant)>>{";
        if (t["methods"] && t["methods"].IsMap())
        {
            for (const auto &_pr : t["methods"])
            {
                auto fn = _pr.first.as<string>();
                const YAML::Node *pfunc = &_pr.second;
                if (pfunc->IsScalar())
                    methods += "{" + dump(fn) + ",_M_" + pfunc->as<string>() + "},";
                else
                    methods += "{" + dump(fn) + "," + buildMethod(*pfunc) + "},";
            }
        }
        methods += "}";
        if (type == "Object")
        {
            f << "    QList<SettingInfo> LIST" << key << ";" << endl;
            writeInfo(f, t["sub"], "LIST" + key);
        }
        string depend = "{";
        if (t["depends"] && t["depends"].IsSequence())
        {
            for (const auto &d : t["depends"])
            {
                if (!d.IsMap())
                    continue;
                depend += "{" + dump(d["name"].as<string>()) + ", [](const QVariant &var){";
                if (d["check"])
                    depend += d["check"].as<string>();
                else
                    depend += "return var.toBool();";
                depend += "}},";
            }
        }
        depend += "}";
        f << "    " << lst << ".append(SettingInfo {" << dump(name) << "," << trdesc << "," << dump(desc) << ","
          << dump(type) << "," << dump(ui) << "," << trtip << "," << dump(tip) << "," << docAnchor << ","
          << requireAllDepends << "," << immediatelyApply << "," << methods << "," << depend << ",";
        if (type != "Object" && !t["default"])
        {
            static map<string, string> defs = {{"QString", "\"\""}, {"int", "0"}, {"bool", "false"}};
            if (defs.count(type) > 0)
                f << defs[type];
            else
                f << type << "{}";
        }
        else
        {
            bool _;
            if (t["default"])
                f << parseParam(t["default"], _);
            else
                f << "QStringList()";
        }
        bool _;
        if (t["param"])
            f << "," << parseParam(t["param"], _);
        else
            f << ",QVariant()";
        f << "});" << endl;
    }
}

regex re_os(R"(<(.+)>(.*))");

void writePage(ofstream &f, const YAML::Node &obj, int indent)
{
    string ids(4 * indent, ' ');
    if (!obj.IsMap())
        return;
    for (const auto &pr : obj)
    {
        string key = pr.first.as<string>();
        cmatch m;
        string rk = key;
        int os = 7;
        if (regex_match(key.c_str(), m, re_os))
        {
            os = 0;
            string osstr = m[1];
            for (char c : osstr)
            {
                switch (c)
                {
                case 'w':
                case 'W':
                    os |= 1;
                    break;
                case 'u':
                case 'U':
                    os |= 2;
                    break;
                case 'm':
                case 'M':
                    os |= 4;
                    break;
                }
            }
            rk = m[2];
        }
        if (os != 7)
            f << "#if PW_OS & " << os << " != 0" << endl;
        if (pr.second.Type() == YAML::NodeType::Sequence)
        {
            f << ids << ".page(" << dump(rk) << ", tr(" << dump(rk) << "), {";
            for (const auto &s : pr.second)
                f << dump(s.as<string>()) << ",";
            f << "})" << endl;
        }
        else
        {
            f << ids << ".dir(" << dump(rk) << ", tr(" << dump(rk) << "))" << endl;
            writePage(f, pr.second, indent + 1);
            f << ids << ".end()" << endl;
        }
        if (os != 7)
            f << "#endif" << endl;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
        return 1;
    auto root = YAML::LoadFile(argv[1]);
    string head = R"(/*
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
 * This file is auto generated by src/Settings/genSettings.cpp
 */
    
)";
    ofstream helper("generated/SettingsHelper.hpp");
    helper << head;
    helper << R"(#ifndef SETTINGSHELPER_HPP
#define SETTINGSHELPER_HPP

#include "Settings/SettingsManager.hpp"
#include <QFont>
#include <QRect>
#include <QVariant>

namespace SettingsHelper
{
)";
    writeHelper(helper, root["data"], "", 1);
    helper << R"(}

#endif // SETTINGSHELPER_HPP)";
    helper.close();

    ofstream info("generated/SettingsInfo.cpp");
    info << head;
    info << R"(#include "Settings/SettingsInfo.hpp"
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
)";
    if (root["presetMethod"] && root["presetMethod"].IsMap())
    {
        for (const auto &pr : root["presetMethod"])
        {
            info << "    auto _M_" << pr.first.as<string>() << " = " << buildMethod(pr.second) << ";" << endl;
        }
    }
    writeInfo(info, root["data"], "settings");
    info << "}" << endl;
    info.close();

    ofstream pw("generated/PreferencesWindow.cpp");
    pw << head;
    pw << R"(#include "Settings/PreferencesWindow.hpp"

#if defined(Q_OS_WIN)
#define PW_OS 1
#elif defined(Q_OS_UNIX) && !defined(Q_OS_MACOS)
#define PW_OS 2
#elif defined(Q_OS_MACOS)
#define PW_OS 4
#else
#define PW_OS 0
#endif

void PreferencesWindow::addPages()
{
    AddPageHelper(this)
)";
    writePage(pw, root["pageInfo"], 2);
    pw << R"(    .ensureAtTop();
})";
    pw.close();

    return 0;
}
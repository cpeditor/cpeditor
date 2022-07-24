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

#include "Core/Translator.hpp"
#include "Core/EventLogger.hpp"
#include "generated/SettingsHelper.hpp"
#include <QMap>
#include <QTranslator>

namespace Core
{
const static QMap<QString, QString> locales = {{"Νέα Ελληνικά", "el_GR"},
                                               {"Português brasileiro", "pt_BR"},
                                               {"Русский", "ru_RU"},
                                               {"简体中文", "zh_CN"},
                                               {"正體中文", "zh_TW"}};

const static QMap<QString, QString> suffixes = {
    {"el_GR", ""}, {"pt_BR", ""}, {"ru_RU", "_ru-RU"}, {"zh_CN", "_zh-CN"}, {"zh_TW", "_zh-TW"}};

const static QMap<QString, QString> code = {
    {"el_GR", ""}, {"pt_BR", ""}, {"ru_RU", "ru"}, {"zh_CN", "zh"}, {"zh_TW", "zh_TW"}};

QTranslator *Translator::translator = nullptr;

void Translator::setLocale()
{
    const auto language = SettingsHelper::getLocale();
    LOG_INFO(INFO_OF(language));
    if (translator)
    {
        LOG_ERR_IF(!qApp->removeTranslator(translator), "Failed to remove the translator " << translator);
        delete translator;
        translator = nullptr;
    }
    QString locale;
    if (language == "system")
    {
        auto name = QLocale::system().name();
        LOG_INFO("System locale: " << INFO_OF(name));
        if (locales.values().contains(name))
            locale = name;
    }
    else if (locales.keys().contains(language))
    {
        locale = locales[language];
    }
    if (!locale.isEmpty())
    {
        translator = new QTranslator(qApp);
        translator->load(QString(":/translations/%1.qm").arg(locale));
        LOG_ERR_IF(!qApp->installTranslator(translator), "Failed to load the translator " << translator);
        SettingsInfo::updateSettingInfo();
    }
}

QString Translator::langSuffix()
{
    return suffixes[langName()];
}

QString Translator::langCode()
{
    return code[langName()];
}

QString Translator::langName()
{
    const auto language = SettingsHelper::getLocale();

    if (language == "system")
        return QLocale::system().name();
    return locales[language];
}
} // namespace Core

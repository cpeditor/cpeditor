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

#include "Extensions/EditorTheme.hpp"
#include "Core/EventLogger.hpp"

#include <QFile>
#include <QIODevice>

namespace Themes
{

QSyntaxStyle *EditorTheme::light = nullptr;
QSyntaxStyle *EditorTheme::drakula = nullptr;
QSyntaxStyle *EditorTheme::monkai = nullptr;
QSyntaxStyle *EditorTheme::solarised = nullptr;
QSyntaxStyle *EditorTheme::solarisedDark = nullptr;

QSyntaxStyle *EditorTheme::getLightTheme()
{
    if (light != nullptr)
        return light;
    else
        return QSyntaxStyle::defaultStyle();
}
QSyntaxStyle *EditorTheme::getMonkaiTheme()
{
    if (monkai != nullptr)
    {
        Core::Log::i("editorTheme/getMonkaiTheme", "Reuse existing resource");
        return monkai;
    }
    else
    {
        QFile f(":/styles/monkai.xml");
        f.open(QIODevice::ReadOnly);
        if (!f.isOpen())
        {
            Core::Log::e("editorTheme/getMonkaiTheme", "Unable to access resources, returning nullptr");
            return nullptr;
        }
        monkai = new QSyntaxStyle();
        monkai->load(f.readAll());
        return monkai;
    }
}
QSyntaxStyle *EditorTheme::getDrakulaTheme()
{
    if (drakula != nullptr)
    {
        Core::Log::i("editorTheme/getDrakulaTheme", "Reuse existing resource");
        return drakula;
    }
    else
    {
        QFile f(":/styles/drakula.xml");
        f.open(QIODevice::ReadOnly);
        if (!f.isOpen())
        {
            Core::Log::e("editorTheme/getDrakulaTheme", "Unable to access resources, returning nullptr");
            return nullptr;
        }
        drakula = new QSyntaxStyle();
        drakula->load(f.readAll());
        return drakula;
    }
}
QSyntaxStyle *EditorTheme::getSolarisedTheme()
{
    if (solarised != nullptr)
    {
        Core::Log::i("editorTheme/getSolarisedTheme", "Reuse existing resource");
        return solarised;
    }
    else
    {
        QFile f(":/styles/solarised.xml");
        f.open(QIODevice::ReadOnly);
        if (!f.isOpen())
        {
            Core::Log::e("editorTheme/getsolarisedTheme", "Unable to access resources, returning nullptr");
            return nullptr;
        }
        solarised = new QSyntaxStyle();
        solarised->load(f.readAll());
        return solarised;
    }
}
QSyntaxStyle *EditorTheme::getSolarisedDarkTheme()
{
    if (solarisedDark != nullptr)
    {
        Core::Log::i("editorTheme/getSolarisedDarkTheme", "Reuse existing resource");
        return solarisedDark;
    }
    else
    {
        QFile f(":/styles/solarisedDark.xml");
        f.open(QIODevice::ReadOnly);
        if (!f.isOpen())
        {
            Core::Log::e("editorTheme/getsolarisedDarkTheme", "Unable to access resources, returning nullptr");
            return nullptr;
        }
        solarisedDark = new QSyntaxStyle();
        solarisedDark->load(f.readAll());
        return solarisedDark;
    }
}

void EditorTheme::release()
{
    Core::Log::i("editorTheme/release", "Releasing theme resources");
    if (light != nullptr)
    {
        Core::Log::i("editorTheme/release", "Deleting light theme");
        delete light;
    }
    if (drakula != nullptr)
    {
        Core::Log::i("editorTheme/release", "Deleting drakula theme");
        delete drakula;
    }
    if (monkai != nullptr)
    {
        Core::Log::i("editorTheme/release", "Deleting monkai theme");
        delete monkai;
    }
    if (solarised != nullptr)
    {
        Core::Log::i("editorTheme/release", "Deleting solarised theme");
        delete solarised;
    }
    if (solarisedDark != nullptr)
    {
        Core::Log::i("editorTheme/release", "Deleting solarisedDark theme");
        delete solarisedDark;
    }
}
} // namespace Themes

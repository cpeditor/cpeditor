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
#include "Util.hpp"
#include <QFile>
#include <QIODevice>

namespace Extensions
{

QSyntaxStyle *EditorTheme::light = nullptr;
QSyntaxStyle *EditorTheme::dracula = nullptr;
QSyntaxStyle *EditorTheme::monokai = nullptr;
QSyntaxStyle *EditorTheme::solarized = nullptr;
QSyntaxStyle *EditorTheme::solarizedDark = nullptr;

QSyntaxStyle *EditorTheme::getLightTheme()
{
    if (light != nullptr)
        return light;
    else
        return QSyntaxStyle::defaultStyle();
}
QSyntaxStyle *EditorTheme::getMonokaiTheme()
{
    if (monokai != nullptr)
        return monokai;
    else
    {
        auto content = Util::readFile(":/styles/monokai.xml", "Read Style");
        if (content.isNull())
            return nullptr;
        monokai = new QSyntaxStyle();
        monokai->load(content);
        return monokai;
    }
}
QSyntaxStyle *EditorTheme::getDraculaTheme()
{
    if (dracula != nullptr)
        return dracula;
    else
    {
        auto content = Util::readFile(":/styles/dracula.xml", "Read Style");
        if (content.isNull())
            return nullptr;
        dracula = new QSyntaxStyle();
        dracula->load(content);
        return dracula;
    }
}
QSyntaxStyle *EditorTheme::getSolarizedTheme()
{
    if (solarized != nullptr)
        return solarized;
    else
    {
        auto content = Util::readFile(":/styles/solarized.xml", "Read Style");
        if (content.isNull())
            return nullptr;
        solarized = new QSyntaxStyle();
        solarized->load(content);
        return solarized;
    }
}
QSyntaxStyle *EditorTheme::getSolarizedDarkTheme()
{
    if (solarizedDark != nullptr)
        return solarizedDark;
    else
    {
        auto content = Util::readFile(":/styles/solarizedDark.xml", "Read Style");
        if (content.isNull())
            return nullptr;
        solarizedDark = new QSyntaxStyle();
        solarizedDark->load(content);
        return solarizedDark;
    }
}

void EditorTheme::release()
{
    if (light != nullptr)
        delete light;
    if (dracula != nullptr)
        delete dracula;
    if (monokai != nullptr)
        delete monokai;
    if (solarized != nullptr)
        delete solarized;
    if (solarizedDark != nullptr)
        delete solarizedDark;
}
} // namespace Extensions

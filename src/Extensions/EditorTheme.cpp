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
    Core::Log::e("editorTheme/getLightTheme", "Invoked");
    if (light != nullptr)
        return light;
    else
        return QSyntaxStyle::defaultStyle();
}
QSyntaxStyle *EditorTheme::getMonokaiTheme()
{
    Core::Log::i("editorTheme/getMonokaiTheme", "Invoked");
    if (monokai != nullptr)
    {
        Core::Log::i("editorTheme/getMonokaiTheme", "reuse existing resource");
        return monokai;
    }
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
    Core::Log::i("editorTheme/getDraculaTheme", "Invoked");
    if (dracula != nullptr)
    {
        Core::Log::i("editorTheme/getDraculaTheme", "reuse existing resource");
        return dracula;
    }
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
    Core::Log::i("editorTheme/getSolarizedTheme", "Invoked");
    if (solarized != nullptr)
    {
        Core::Log::i("editorTheme/getSolarizedTheme", "reuse existing resource");
        return solarized;
    }
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
    Core::Log::i("editorTheme/getSolarizedDarkTheme", "Invoked");
    if (solarizedDark != nullptr)
    {
        Core::Log::i("editorTheme/getSolarizedDarkTheme", "reuse existing resource");
        return solarizedDark;
    }
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
    Core::Log::i("editorTheme/release", "releasing theme resources");
    if (light != nullptr)
    {
        Core::Log::i("editorTheme/release", "deleting light theme");
        delete light;
    }
    if (dracula != nullptr)
    {
        Core::Log::i("editorTheme/release", "deleting dracula theme");
        delete dracula;
    }
    if (monokai != nullptr)
    {
        Core::Log::i("editorTheme/release", "deleting monokai theme");
        delete monokai;
    }
    if (solarized != nullptr)
    {
        Core::Log::i("editorTheme/release", "deleting solarized theme");
        delete solarized;
    }
    if (solarizedDark != nullptr)
    {
        Core::Log::i("editorTheme/release", "deleting solarizedDark theme");
        delete solarizedDark;
    }
}
} // namespace Extensions

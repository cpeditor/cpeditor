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
#include "Util/FileUtil.hpp"
#include <QFile>
#include <QSyntaxStyle>

namespace Extensions
{

static const QMap<QString, QString> resources = {{"Dracula", ":/styles/dracula.xml"},
                                                 {"Monokai", ":/styles/monokai.xml"},
                                                 {"Solarized", ":/styles/solarized.xml"},
                                                 {"Solarized Dark", ":/styles/solarizedDark.xml"}};

QMap<QString, QSyntaxStyle *>
    EditorTheme::styles; // NOLINT: variable 'styles' is non-const and globally accessible, consider making it const

QSyntaxStyle *EditorTheme::query(const QString &name)
{
    if (styles.find(name) != styles.end())
        return styles[name];
    if (name == "Light")
        return styles["Light"] = QSyntaxStyle::defaultStyle();
    if (resources.find(name) == resources.end())
        return nullptr;
    auto content = Util::readFile(resources[name], "Read Style");
    if (content.isNull())
        return nullptr;
    auto *style = new QSyntaxStyle();
    style->load(content);
    return styles[name] = style;
}

void EditorTheme::release()
{
    for (auto *style : styles.values())
        if (style != QSyntaxStyle::defaultStyle())
            delete style;
    styles.clear();
}

} // namespace Extensions

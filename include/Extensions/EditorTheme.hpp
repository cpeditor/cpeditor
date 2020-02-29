/*
 * Copyright (C) 2019-2020 Ashar Khan <ashar786khan@gmail.com>
 *
 * This file is part of cpeditor.
 *
 * cpeditor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * I will not be responsible if cpeditor behaves in unexpected way and
 * causes your ratings to go down and or lose any important contest.
 *
 * Believe Software is "Software" and it isn't immune to bugs.
 *
 */

#ifndef EDITORTHEME_H
#define EDITORTHEME_H

#include <QSyntaxStyle>

namespace Themes
{
class EditorTheme
{
  public:
    EditorTheme() = delete;

  private:
    static QSyntaxStyle *light;
    static QSyntaxStyle *drakula;
    static QSyntaxStyle *monkai;
    static QSyntaxStyle *solarised;
    static QSyntaxStyle *solarisedDark;

  public:
    static QSyntaxStyle *getLightTheme();
    static QSyntaxStyle *getDrakulaTheme();
    static QSyntaxStyle *getMonkaiTheme();
    static QSyntaxStyle *getSolarisedTheme();
    static QSyntaxStyle *getSolarisedDarkTheme();

    static void release();
};
} // namespace Themes

#endif // EDITORTHEME_H

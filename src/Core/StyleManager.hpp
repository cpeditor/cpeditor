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

#ifndef STYLE_MANAGER_HPP
#define STYLE_MANAGER_HPP

class QString;
class QStringList;
class QPalette;

namespace Core
{
class StyleManager
{
  public:
    static void setDefault();
    static bool setStyle(const QString &styleName);
    static QStringList styleList();

  private:
    static bool isWindowsDarkThemeForApps();

    static void setPalette(const QString &styleName);

    static QPalette darkFusionPalette();

    static QPalette lightFusionPalette();

  private:
    static QPalette defaultPalette;
    static QString defaultStyle;
    static QString currentStyle;
};

} // namespace Core
#endif
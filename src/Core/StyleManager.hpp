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

#ifndef STYLE_MANAGER_HPP
#define STYLE_MANAGER_HPP

class QString;
class QStringList;
class QPalette;
class QStyle;

namespace Core
{
class StyleManager
{
  public:
    static bool initiateStyle(QString);
    static QStringList styleList();

  private:
    static bool setStyle(QString styleName);
    static bool isWindowsDarkThemeforApps();
    static QPalette fusionDarkPalette();
    static void setFusionDarkTooltip();
    static QPalette fusionLightPalette();
    static void setFusionLightTooltip();

};

} // namespace Core
#endif
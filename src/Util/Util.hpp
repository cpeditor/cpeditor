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

#ifndef UTIL_HPP
#define UTIL_HPP

#include <QMetaType>
#include <QWidget>

namespace Util
{

void showWidgetOnTop(QWidget *widget);

QString sanitizeAnchorName(const QString &str);

QString websiteLink(const QString &path = QString());

} // namespace Util

#endif // UTIL_HPP

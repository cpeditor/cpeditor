/*
 * Copyright (C) 2019-2020 Ashar Khan <ashar786khan@gmail.com>
 *
 * This file is part of CPEditor.
 *
 * CPEditor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * I will not be responsible if CPEditor behaves in an unexpected way and
 * causes your bad performance and / or lose any contest.
 *
 * Believe Software is "Software" and it isn't immune to bugs.
 *
 */

#ifndef UTIL_HPP
#define UTIL_HPP

#include <QString>

inline QString bool2string(const bool &value)
{
    if (value)
        return QString::fromStdString("true");
    else
        return QString::fromStdString("false");
}

#endif
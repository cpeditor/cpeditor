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

#include "Settings/SettingsUpdater.hpp"
#include "Core/SessionManager.hpp"
#include "Settings/SettingsManager.hpp"
#include "generated/SettingsHelper.hpp"
#include <QSet>
#include <QSettings>

void SettingsUpdater::updateSetting()
{
}

void SettingsUpdater::updateSetting_INI(QSettings &setting)
{
#ifdef QT_DEBUG
    // Check for key conflicts
    QSet<QString> keys;

    auto addKey = [&](const QString &key) {
        if (keys.contains(key))
            qFatal("Duplicate key in the settings: %s", // NOLINT: C-Style varargs should not be called.
                   key.toStdString().c_str());
        keys.insert(key);
    };

    for (const auto &si : qAsConst(SettingsInfo::settings))
    {
        addKey(si.name);
    }
#endif
    // Remove all fixes.
}

void SettingsUpdater::updateSettingFinal()
{
}

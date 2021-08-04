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

#ifndef PREFERENCESTEMPLATE_HPP
#define PREFERENCESTEMPLATE_HPP

#include "Settings/PreferencesPage.hpp"
#include "Settings/SettingWidget.hpp"

struct SettingsWrapper;

class PreferencesTemplate : public PreferencesPage
{
    Q_OBJECT

  public:
    explicit PreferencesTemplate(const QStringList &opts, const QString &path, const QString &trPath,
                                 bool alignTop = true, QWidget *parent = nullptr);

    QStringList content() override;

  private:
    bool areSettingsChanged() override;
    void makeUITheSameAsDefault() override;
    void makeUITheSameAsSettings() override;
    void makeSettingsTheSameAsUI() override;

  private:
    QList<QStringList> options;
    QList<SettingsWrapper *> wraps;
};

#endif // PREFERENCESPAGETEMPLATE_HPP

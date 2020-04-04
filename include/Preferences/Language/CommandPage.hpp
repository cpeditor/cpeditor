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

#ifndef PREFERENCESCOMMANDPAGE_HPP
#define PREFERENCESCOMMANDPAGE_HPP

#include "Preferences/PathItem.hpp"
#include "Preferences/PreferencesGridPage.hpp"

class CommandPage : public PreferencesGridPage
{
    Q_OBJECT
  public:
    explicit CommandPage(QWidget *parent);

  private:
    bool areSettingsChanged() override;

    void makeUITheSameAsDefault() override;

    void makeUITheSameAsSettings() override;

    void makeSettingsTheSameAsUI() override;

  private:
    QLineEdit *cppCompile = nullptr;
    QLineEdit *cppRunArgument = nullptr;
    QLineEdit *javaCompile = nullptr;
    QLineEdit *javaRunArgument = nullptr;
    PathItem *javaStartPath = nullptr;
    QLineEdit *pythonRunArgument = nullptr;
    PathItem *pythonStartPath = nullptr;
};

#endif
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

/*
 * This is a convenience class for creating a PreferencesPage with QGridLayout.
 *
 * In fact, this class uses QGridLayout to simulate QFormLayout, because there
 * is an alignment problem with QFormLayout.
 */

#ifndef PREFERENCESGRIDPAGE_HPP
#define PREFERENCESGRIDPAGE_HPP

#include "Settings/PreferencesPage.hpp"

class QGridLayout;

class PreferencesGridPage : public PreferencesPage
{
    Q_OBJECT

  public:
    explicit PreferencesGridPage(bool alignTop = true, QWidget *parent = nullptr);

    /**
     * @brief add a row
     * @param widget the widget to add
     * @param tip the tool tip of this item
     * @param help the help (what's this) of this item
     * @param labelText (optional) the label of the row
     */
    void addRow(ValueWidget *widget, const QString &tip, const QString &help, const QString &labelText = QString());

  private:
    QVBoxLayout *VLayout = nullptr;
    QHBoxLayout *HLayout = nullptr;
    QGridLayout *gridLayout = nullptr;
};

#endif
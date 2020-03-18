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

#include "Preferences/PreferencesPage.hpp"
#include <QApplication>
#include <QMessageBox>
#include <QStyle>

PreferencesPage::PreferencesPage(QWidget *parent) : QWidget(parent)
{
    // construct widgets
    mainLayout = new QVBoxLayout(this);
    scrollArea = new QScrollArea();
    scrollAreaWidget = new QWidget();
    settingsLayout = new QVBoxLayout(scrollAreaWidget);
    buttonsLayout = new QHBoxLayout();
    defaultButton = new QPushButton(QApplication::style()->standardIcon(QStyle::SP_DialogDiscardButton), "Default");
    resetButton = new QPushButton(QApplication::style()->standardIcon(QStyle::SP_DialogResetButton), "Reset");
    buttonsSpacer = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Expanding);
    applyButton = new QPushButton(QApplication::style()->standardIcon(QStyle::SP_DialogApplyButton), "Apply");

    // set up the UI
    buttonsLayout->addWidget(defaultButton);
    buttonsLayout->addWidget(resetButton);
    buttonsLayout->addItem(buttonsSpacer);
    buttonsLayout->addWidget(applyButton);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(scrollAreaWidget);
    mainLayout->addWidget(scrollArea);
    mainLayout->addLayout(buttonsLayout);

    // connect the signals and slots
    connect(defaultButton, SIGNAL(clicked()), this, SLOT(loadDefault()));
    connect(resetButton, SIGNAL(clicked()), this, SLOT(loadSettings()));
    connect(applyButton, SIGNAL(clicked()), this, SLOT(applySettings()));
}

bool PreferencesPage::aboutToExit()
{
    // exit if there is no unsaved change
    if (!areSettingsChanged())
        return true;

    // otherwise, ask the user whether to save, discard or cancel
    auto response = QMessageBox::warning(this, "Unsaved Settings",
                                         "The settings are changed. Do you want to save the settings or discard them?",
                                         QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

    switch (response)
    {
    case QMessageBox::Save:
        applySettings();
        return true;
    case QMessageBox::Discard:
        loadSettings();
        return true;
    case QMessageBox::Cancel:
        return false;
    default:
        Q_UNREACHABLE();
        break;
    }
}

void PreferencesPage::loadSettings()
{
    makeUITheSameAsSettings();
    updateButtons();
}

void PreferencesPage::addLayout(QLayout *layout)
{
    settingsLayout->addLayout(layout);
}

void PreferencesPage::addWidget(QWidget *widget)
{
    settingsLayout->addWidget(widget);
}

void PreferencesPage::addItem(QLayoutItem *item)
{
    settingsLayout->addItem(item);
}

void PreferencesPage::loadDefault()
{
    makeUITheSameAsDefault();
    updateButtons();
}

void PreferencesPage::applySettings()
{
    makeSettingsTheSameAsUI();
    updateButtons();
    emit settingsApplied();
}

void PreferencesPage::updateButtons()
{
    bool changed = areSettingsChanged();
    resetButton->setEnabled(changed);
    applyButton->setEnabled(changed);
}

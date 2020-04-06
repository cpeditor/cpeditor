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
#include <QFormLayout>
#include <QMessageBox>
#include <QStyle>

PreferencesPage::PreferencesPage(QWidget *parent) : QWidget(parent)
{
    // construct widgets
    mainLayout = new QVBoxLayout(this);
    titleLabel = new QLabel();
    scrollArea = new QScrollArea();
    scrollAreaWidget = new QWidget();
    VStretchLayout = new QVBoxLayout(scrollAreaWidget);
    HStretchLayout = new QHBoxLayout();
    settingsLayout = new QVBoxLayout();
    buttonsLayout = new QHBoxLayout();
    defaultButton = new QPushButton(QApplication::style()->standardIcon(QStyle::SP_FileDialogDetailedView), "Default");
    resetButton = new QPushButton(QApplication::style()->standardIcon(QStyle::SP_DialogResetButton), "Reset");
    applyButton = new QPushButton(QApplication::style()->standardIcon(QStyle::SP_DialogApplyButton), "Apply");

    // set up the UI
    buttonsLayout->addWidget(defaultButton);
    buttonsLayout->addWidget(resetButton);
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(applyButton);
    HStretchLayout->addStretch();
    HStretchLayout->addLayout(settingsLayout);
    HStretchLayout->addStretch();
    VStretchLayout->addSpacing(30);
    VStretchLayout->addLayout(HStretchLayout);
    VStretchLayout->addStretch();
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(scrollAreaWidget);
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(scrollArea);
    mainLayout->addLayout(buttonsLayout);

    // set the font for title
    auto labelFont = font();
    labelFont.setPointSizeF(font().pointSizeF() * 1.6);
    titleLabel->setFont(labelFont);

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

QString PreferencesPage::path() const
{
    return m_path;
}

void PreferencesPage::setPath(const QString &path)
{
    m_path = path;
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

void PreferencesPage::registerWidget(QCheckBox *checkBox)
{
    connect(checkBox, SIGNAL(toggled(bool)), this, SLOT(updateButtons()));
}

void PreferencesPage::registerWidget(QLineEdit *lineEdit)
{
    connect(lineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(updateButtons()));
}

void PreferencesPage::registerWidget(QSpinBox *spinBox)
{
    connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(updateButtons()));
}

void PreferencesPage::registerWidget(QScrollBar *scrollBar)
{
    connect(scrollBar, SIGNAL(valueChanged(int)), this, SLOT(updateButtons()));
}

void PreferencesPage::registerWidget(QSlider *slider)
{
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(updateButtons()));
}

void PreferencesPage::registerWidget(QComboBox *comboBox)
{
    connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateButtons()));
}

void PreferencesPage::registerWidget(FontItem *fontItem)
{
    connect(fontItem, SIGNAL(fontChanged(QFont)), this, SLOT(updateButtons()));
}

void PreferencesPage::registerWidget(PathItem *pathItem)
{
    registerWidget(pathItem->getLineEdit());
}

void PreferencesPage::setTitle(const QString &title)
{
    titleLabel->setText(title);
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
    emit settingsApplied(m_path);
}

void PreferencesPage::updateButtons()
{
    bool changed = areSettingsChanged();
    resetButton->setEnabled(changed);
    applyButton->setEnabled(changed);
}

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

#include "Preferences/Edit/EditPage.hpp"

EditPage::EditPage(QWidget *parent) : PreferencesPage(parent)
{
    tabWidthLayout = new QFormLayout();
    addLayout(tabWidthLayout);
    tabWidthSpinBox = new QSpinBox(this);
    tabWidthSpinBox->setRange(1, 20);
    connect(tabWidthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(updateButtons()));
    tabWidthLayout->addRow("Tab Width", tabWidthSpinBox);

    indentCheckBox = new QCheckBox("Auto Indentation", this);
    addCheckBox(indentCheckBox);

    textWrapCheckBox = new QCheckBox("Text Wrap", this);
    addCheckBox(textWrapCheckBox);

    replaceTabsWithSpacesCheckBox = new QCheckBox("Replace tabs with spaces", this);
    addCheckBox(replaceTabsWithSpacesCheckBox);

    parenthesesCompletionCheckBox = new QCheckBox("Auto Parentheses Completion", this);
    addCheckBox(parenthesesCompletionCheckBox);

    parenthesesRemovalCheckBox = new QCheckBox("Auto Parentheses Removal", this);
    addCheckBox(parenthesesRemovalCheckBox);

    addStretch();
}

bool EditPage::areSettingsChanged()
{
    if (Settings::SettingsManager::getTabStop() != tabWidthSpinBox->value())
        return true;
    if (Settings::SettingsManager::isAutoIndent() != indentCheckBox->isChecked())
        return true;
    if (Settings::SettingsManager::isWrapText() != textWrapCheckBox->isChecked())
        return true;
    if (Settings::SettingsManager::isTabsReplaced() != replaceTabsWithSpacesCheckBox->isChecked())
        return true;
    if (Settings::SettingsManager::isAutoParentheses() != parenthesesCompletionCheckBox->isChecked())
        return true;
    if (Settings::SettingsManager::isAutoRemoveParentheses() != parenthesesRemovalCheckBox->isChecked())
        return true;
    return false;
}

void EditPage::makeUITheSameAsDefault()
{
    tabWidthSpinBox->setValue(Settings::Default::tabStopWidth);
    indentCheckBox->setChecked(Settings::Default::autoIndent);
    textWrapCheckBox->setChecked(Settings::Default::textWrap);
    replaceTabsWithSpacesCheckBox->setChecked(Settings::Default::tabsReplaced);
    parenthesesCompletionCheckBox->setChecked(Settings::Default::autoParentheses);
    parenthesesRemovalCheckBox->setChecked(Settings::Default::autoRemoveParentheses);
}

void EditPage::makeUITheSameAsSettings()
{
    tabWidthSpinBox->setValue(Settings::SettingsManager::getTabStop());
    indentCheckBox->setChecked(Settings::SettingsManager::isAutoIndent());
    textWrapCheckBox->setChecked(Settings::SettingsManager::isWrapText());
    replaceTabsWithSpacesCheckBox->setChecked(Settings::SettingsManager::isTabsReplaced());
    parenthesesCompletionCheckBox->setChecked(Settings::SettingsManager::isAutoParentheses());
    parenthesesRemovalCheckBox->setChecked(Settings::SettingsManager::isAutoRemoveParentheses());
}

void EditPage::makeSettingsTheSameAsUI()
{
    Settings::SettingsManager::setTabStop(tabWidthSpinBox->value());
    Settings::SettingsManager::setAutoIndent(indentCheckBox->isChecked());
    Settings::SettingsManager::setWrapText(textWrapCheckBox->isChecked());
    Settings::SettingsManager::setTabsReplaced(replaceTabsWithSpacesCheckBox->isChecked());
    Settings::SettingsManager::setAutoParentheses(parenthesesCompletionCheckBox->isChecked());
    Settings::SettingsManager::setAutoRemoveParentheses(parenthesesRemovalCheckBox->isChecked());
}

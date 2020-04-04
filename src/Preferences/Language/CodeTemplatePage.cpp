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

#include "Preferences/Language/CodeTemplatePage.hpp"
#include "Util.hpp"

CodeTemplatePage::CodeTemplatePage(QWidget *parent) : PreferencesGridPage(parent)
{
    setTitle("Code Template");

    cppTemplate = new PathItem(Util::fileNameFilter(true, false, false), "Choose Code Template for C++");
    registerAddRow("C++ Template", cppTemplate, cppTemplate->getLineEdit());

    javaTemplate = new PathItem(Util::fileNameFilter(false, true, false), "Choose Code Template for Java");
    registerAddRow("Java Template", javaTemplate, javaTemplate->getLineEdit());

    pythonTemplate = new PathItem(Util::fileNameFilter(false, false, true), "Choose Code Template for Python");
    registerAddRow("Python Template", pythonTemplate, pythonTemplate->getLineEdit());
}

bool CodeTemplatePage::areSettingsChanged()
{
    if (cppTemplate->getLineEdit()->text() != Settings::SettingsManager::getTemplatePath("C++"))
        return true;
    if (javaTemplate->getLineEdit()->text() != Settings::SettingsManager::getTemplatePath("Java"))
        return true;
    if (pythonTemplate->getLineEdit()->text() != Settings::SettingsManager::getTemplatePath("Python"))
        return true;
    return false;
}

void CodeTemplatePage::makeUITheSameAsDefault()
{
    cppTemplate->getLineEdit()->clear();
    javaTemplate->getLineEdit()->clear();
    pythonTemplate->getLineEdit()->clear();
}

void CodeTemplatePage::makeUITheSameAsSettings()
{
    cppTemplate->getLineEdit()->setText(Settings::SettingsManager::getTemplatePath("C++"));
    javaTemplate->getLineEdit()->setText(Settings::SettingsManager::getTemplatePath("Java"));
    pythonTemplate->getLineEdit()->setText(Settings::SettingsManager::getTemplatePath("Python"));
}

void CodeTemplatePage::makeSettingsTheSameAsUI()
{
    Settings::SettingsManager::setTemplatePath("C++", cppTemplate->getLineEdit()->text());
    Settings::SettingsManager::setTemplatePath("Java", javaTemplate->getLineEdit()->text());
    Settings::SettingsManager::setTemplatePath("Python", pythonTemplate->getLineEdit()->text());
}

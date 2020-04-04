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

#include "Preferences/Language/CommandPage.hpp"
#include "Util.hpp"

CommandPage::CommandPage(QWidget *parent) : PreferencesGridPage(parent)
{
    setTitle("Compile & Run Commands");

    cppCompile = new QLineEdit(this);
    registerAddRow("C++ Compile Command", cppCompile);

    cppRunArgument = new QLineEdit(this);
    registerAddRow("C++ Runtime Arguments", cppRunArgument);

    javaCompile = new QLineEdit(this);
    registerAddRow("Java Compile Command", javaCompile);

    javaRunArgument = new QLineEdit(this);
    registerAddRow("Java Runtime Arguments", javaRunArgument);

    javaStartPath = new PathItem("Excutable (*" EXE_SUFFIX ")", "Choose Java Excutable");
    registerAddRow("Java Start Program", javaStartPath, javaStartPath->getLineEdit());

    pythonRunArgument = new QLineEdit(this);
    registerAddRow("Python Runtime Arguments", pythonRunArgument);

    pythonStartPath = new PathItem("Excutable (*" EXE_SUFFIX ")", "Choose Python Excutable");
    registerAddRow("Python Start Program", pythonStartPath, pythonStartPath->getLineEdit());
}

bool CommandPage::areSettingsChanged()
{
    if (cppCompile->text() != Settings::SettingsManager::getCompileCommand("C++"))
        return true;
    if (cppRunArgument->text() != Settings::SettingsManager::getRuntimeArguments("C++"))
        return true;
    if (javaCompile->text() != Settings::SettingsManager::getCompileCommand("Java"))
        return true;
    if (javaRunArgument->text() != Settings::SettingsManager::getRuntimeArguments("Java"))
        return true;
    if (javaStartPath->getLineEdit()->text() != Settings::SettingsManager::getRunCommand("Java"))
        return true;
    if (pythonRunArgument->text() != Settings::SettingsManager::getRuntimeArguments("Python"))
        return true;
    if (pythonStartPath->getLineEdit()->text() != Settings::SettingsManager::getRunCommand("Python"))
        return true;
    return false;
}

void CommandPage::makeUITheSameAsDefault()
{
    cppCompile->setText(Settings::Default::cppCompileCommand);
    cppRunArgument->setText("");
    javaCompile->setText(Settings::Default::javaCompileCommand);
    javaRunArgument->setText("");
    javaStartPath->getLineEdit()->setText(Settings::Default::javaRunCommand);
    pythonRunArgument->setText("");
    pythonStartPath->getLineEdit()->setText(Settings::Default::pythonRunCommand);
}

void CommandPage::makeUITheSameAsSettings()
{
    cppCompile->setText(Settings::SettingsManager::getCompileCommand("C++"));
    cppRunArgument->setText(Settings::SettingsManager::getRuntimeArguments("C++"));
    javaCompile->setText(Settings::SettingsManager::getCompileCommand("Java"));
    javaRunArgument->setText(Settings::SettingsManager::getRuntimeArguments("Java"));
    javaStartPath->getLineEdit()->setText(Settings::SettingsManager::getRunCommand("Java"));
    pythonRunArgument->setText(Settings::SettingsManager::getRuntimeArguments("Python"));
    pythonStartPath->getLineEdit()->setText(Settings::SettingsManager::getRunCommand("Python"));
}

void CommandPage::makeSettingsTheSameAsUI()
{
    Settings::SettingsManager::setCompileCommand("C++", cppCompile->text());
    Settings::SettingsManager::setRuntimeArguments("C++", cppRunArgument->text());
    Settings::SettingsManager::setCompileCommand("Java", javaCompile->text());
    Settings::SettingsManager::setRuntimeArguments("Java", javaRunArgument->text());
    Settings::SettingsManager::setRunCommand("Java", javaStartPath->getLineEdit()->text());
    Settings::SettingsManager::setRuntimeArguments("Python", pythonRunArgument->text());
    Settings::SettingsManager::setRunCommand("Python", pythonStartPath->getLineEdit()->text());
}
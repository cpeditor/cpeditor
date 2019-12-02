/*
 * Copyright (C) 2019 Ashar Khan <ashar786khan@gmail.com>
 *
 * This file is part of CPEditor.
 *
 * CPEditor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * I will not be responsible if CPEditor behaves in unexpected way and
 * causes your ratings to go down and or loose any important contest.
 *
 * Believe Software is "Software" and it isn't not immune to bugs.
 *
 */

#include <MessageLogger.hpp>
#include <QApplication>
#include <QStandardPaths>
#include <SettingsManager.hpp>

namespace Settings {
SettingManager::SettingManager() {
  mSettingsFile =
      QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/" +
      SETTINGS_FILE;
  mSettings = new QSettings(mSettingsFile, QSettings::IniFormat);
  // Log::MessageLogger::info("Settings","saved under " +
  // mSettingsFile.toStdString());
}

bool SettingManager::isDarkTheme() {
  return mSettings->value("dark_theme", "false").toBool();
}

bool SettingManager::isWrapText() {
  return mSettings->value("wrap_text", "false").toBool();
}

bool SettingManager::isAutoIndent() {
  return mSettings->value("auto_indent", "true").toBool();
}

bool SettingManager::isAutoParenthesis() {
  return mSettings->value("auto_parenthesis", "true").toBool();
}
bool SettingManager::isAutoSave(){
    return mSettings->value("autosave", "false").toBool();
}

std::string SettingManager::getRunCommand() {
  return mSettings->value("run", "").toString().toStdString();
}
std::string SettingManager::getCompileCommand() {
  return mSettings->value("compile", "g++ -Wall").toString().toStdString();
}
std::string SettingManager::getFormatCommand() {
  return mSettings->value("format", "clang-format -i").toString().toStdString();
}
std::string SettingManager::getPrependRunCommand() {
  return mSettings->value("prepend_run", "").toString().toStdString();
}
std::string SettingManager::getDefaultLang() {
  return mSettings->value("lang", "Cpp").toString().toStdString();
}
std::string SettingManager::getTemplatePath() {
  return mSettings->value("template", "").toString().toStdString();
}
std::string SettingManager::getFont(){
    return mSettings->value("font", "").toString().toStdString();
}

void SettingManager::setDarkTheme(bool value) {
  if (value)
    mSettings->setValue("dark_theme", QString::fromStdString("true"));
  else
    mSettings->setValue("dark_theme", QString::fromStdString("false"));
}

void SettingManager::setAutoIndent(bool value) {
  if (value)
    mSettings->setValue("auto_indent", QString::fromStdString("true"));
  else
    mSettings->setValue("auto_indent", QString::fromStdString("false"));
}

void SettingManager::setWrapText(bool value) {
  if (value)
    mSettings->setValue("wrap_text", QString::fromStdString("true"));
  else
    mSettings->setValue("wrap_text", QString::fromStdString("false"));
}

void SettingManager::setAutoParenthesis(bool value) {
  if (value)
    mSettings->setValue("auto_parenthesis", QString::fromStdString("true"));
  else
    mSettings->setValue("auto_parenthesis", QString::fromStdString("false"));
}

void SettingManager::setAutoSave(bool value){
    if(value)
        mSettings->setValue("autosave", QString::fromStdString("true"));
    else
        mSettings->setValue("autosave", QString::fromStdString("false"));
}

void SettingManager::setRunCommand(std::string command) {
  mSettings->setValue("run", QString::fromStdString(command));
}
void SettingManager::setCompileCommands(std::string command) {
  mSettings->setValue("compile", QString::fromStdString(command));
}
void SettingManager::setFormatCommand(std::string value) {
  mSettings->setValue("format", QString::fromStdString(value));
}
void SettingManager::setTemplatePath(std::string path) {
  mSettings->setValue("template", QString::fromStdString(path));
}
void SettingManager::setPrependRunCommand(std::string command) {
  mSettings->setValue("prepend_run", QString::fromStdString(command));
}
void SettingManager::setDefaultLanguage(std::string lang) {
  mSettings->setValue("lang", QString::fromStdString(lang));
}
void SettingManager::setFont(std::string font){
    mSettings->setValue("font", QString::fromStdString(font));
}

SettingManager::~SettingManager() {
  mSettings->sync();
  delete mSettings;
}

}  // namespace Settings

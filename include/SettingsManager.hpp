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


#ifndef SETTINGSMANAGER_HPP
#define SETTINGSMANAGER_HPP

#include <QSettings>
#include <QString>
#include <string>

#define SETTINGS_FILE "cp_editor_settings.ini"

namespace Settings {
class SettingManager {
 public:
  SettingManager();

  bool isDarkTheme();
  bool isAutoIndent();
  bool isAutoParenthesis();
  bool isWrapText();
  bool isAutoSave();
  bool isBeta();
  bool isTabs();

  std::string getFormatCommand();
  std::string getCompileCommand();
  std::string getRunCommand();
  std::string getPrependRunCommand();
  std::string getTemplatePath();
  std::string getDefaultLang();
  std::string getFont();
  QRect getGeometry();

  void setDarkTheme(bool value);
  void setAutoParenthesis(bool value);
  void setAutoIndent(bool value);
  void setWrapText(bool value);
  void setAutoSave(bool value);
  void setBeta(bool value);
  void setTabs(bool value);

  void setCompileCommands(std::string command);
  void setRunCommand(std::string command);
  void setFormatCommand(std::string command);
  void setPrependRunCommand(std::string command);
  void setTemplatePath(std::string path);
  void setDefaultLanguage(std::string lang);
  void setFont(std::string font);
  void setGeometry(QRect);

  ~SettingManager();

 private:
  QString mSettingsFile;
  QSettings* mSettings;
};
}  // namespace Settings

#endif  // SETTINGSMANAGER_HPP

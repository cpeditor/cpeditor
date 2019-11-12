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

  std::string getFormatCommand();
  std::string getCompileCommand();
  std::string getRunCommand();

  void setDarkTheme(bool value);
  void setAutoParenthesis(bool value);
  void setAutoIndent(bool value);
  void setWrapText(bool value);

  void setCompileCommands(std::string command);
  void setRunCommand(std::string command);
  void setFormatCommand(std::string command);

  ~SettingManager();

 private:
  QString mSettingsFile;
  QSettings* mSettings;
};
}  // namespace Settings

#endif  // SETTINGSMANAGER_HPP

#include <MessageLogger.hpp>
#include <QApplication>
#include <QStandardPaths>
#include <SettingsManager.hpp>

namespace Settings {
SettingManager::SettingManager() {
  mSettingsFile =
      QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/" +
      SETTINGS_FILE;
  mSettings = new QSettings(mSettingsFile, QSettings::NativeFormat);
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

std::string SettingManager::getRunCommand() {
  return mSettings->value("run", "").toString().toStdString();
}
std::string SettingManager::getCompileCommand() {
  return mSettings->value("compile", "g++ -Wall").toString().toStdString();
}
std::string SettingManager::getFormatCommand() {
  return mSettings->value("format", "clang-format -i").toString().toStdString();
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

void SettingManager::setRunCommand(std::string command) {
  mSettings->setValue("run", QString::fromStdString(command));
}
void SettingManager::setCompileCommands(std::string command) {
  mSettings->setValue("compile", QString::fromStdString(command));
}
void SettingManager::setFormatCommand(std::string value) {
  mSettings->setValue("format", QString::fromStdString(value));
}

SettingManager::~SettingManager() {
  delete mSettings;
}

}  // namespace Settings

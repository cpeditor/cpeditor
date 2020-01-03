#ifndef PREFERENCEWINDOW_HPP
#define PREFERENCEWINDOW_HPP

#include "SettingsManager.hpp"
#include <QListWidget>
#include <QMainWindow>

namespace Ui
{
class PreferenceWindow;
}

class PreferenceWindow : public QMainWindow
{
    Q_OBJECT

  signals:
    void settingsApplied();

  public:
    explicit PreferenceWindow(Settings::SettingManager *manager, QWidget *parent = nullptr);
    void resetSettings();
    void updateShow();
    ~PreferenceWindow();

  private slots:

    void on_exit_clicked();

    void on_apply_clicked();

    void on_hotkeys_clicked(bool checked);

    void on_font_button_clicked();

    void on_cpp_template_clicked();

    void on_py_template_clicked();

    void on_java_template_clicked();

  private:
    Ui::PreferenceWindow *ui;
    QFont currentFont;
    QString cppTemplatePath, pythonTemplatePath, javaTemplatePath;
    Settings::SettingManager *manager;

    void extractSettingsFromUi();
    void applySettingsToui();
    void setConstraints();
};

#endif // PREFERENCEWINDOW_HPP

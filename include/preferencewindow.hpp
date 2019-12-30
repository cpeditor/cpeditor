#ifndef PREFERENCEWINDOW_HPP
#define PREFERENCEWINDOW_HPP

#include <QMainWindow>
#include "SettingsManager.hpp"

namespace Ui {
class PreferenceWindow;
}

class PreferenceWindow : public QMainWindow
{
    Q_OBJECT

signals:
    void settingsApplied();
public:
    explicit PreferenceWindow(Settings::SettingManager* manager, QWidget *parent = nullptr);
    void resetSettings();
    ~PreferenceWindow();

private slots:

    void on_exit_clicked();

    void on_apply_clicked();

private:
    Ui::PreferenceWindow *ui;
    Settings::SettingManager* manager;
};

#endif // PREFERENCEWINDOW_HPP

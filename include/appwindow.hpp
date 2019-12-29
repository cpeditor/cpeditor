#ifndef APPWINDOW_HPP
#define APPWINDOW_HPP

#include <QMainWindow>

#include "mainwindow.hpp"
#include "SettingsManager.hpp"
#include "UpdateNotifier.hpp"
#include "preferencewindow.hpp"

namespace Ui {
class AppWindow;
}

class AppWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AppWindow(QWidget *parent = nullptr);
    explicit AppWindow(QVector<MainWindow*> tabs, QWidget* parent = nullptr);
    ~AppWindow() override;

    void closeEvent(QCloseEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;

private slots:
    void on_actionSupport_me_triggered();

    void on_actionAbout_triggered();

    void on_actionClose_All_triggered();

    void on_actionAutosave_triggered(bool checked);

    void on_actionQuit_triggered();

    void on_actionNew_Tab_triggered();

    void on_actionOpen_triggered();

    void on_actionRestore_Settings_triggered();

    void on_actionSettings_triggered();

    void on_actionSave_triggered();

    void on_actionSave_as_triggered();

    void onTabCloseRequested(int);
    void onTabChanged(int);
    void onEditorTextChanged(bool);
    void onSaveTimerElapsed();
    void onSettingsApplied();

private:
    Ui::AppWindow *ui;
    MessageLogger* activeLogger = nullptr;
    QTimer* timer = nullptr;
    QMetaObject::Connection activeTextChangeConnections;
    Settings::SettingManager *settingManager = nullptr;
    Telemetry::UpdateNotifier *updater = nullptr;
    PreferenceWindow* preferenceWindow = nullptr;;

    void setConnections();
    void allocate();
    void applySettings();

};

#endif // APPWINDOW_HPP

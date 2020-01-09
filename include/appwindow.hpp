#ifndef APPWINDOW_HPP
#define APPWINDOW_HPP

#include <QMainWindow>

#include "SettingsManager.hpp"
#include "UpdateNotifier.hpp"
#include "mainwindow.hpp"
#include "preferencewindow.hpp"

namespace Ui
{
class AppWindow;
}

class AppWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit AppWindow(QWidget *parent = nullptr);
    explicit AppWindow(QStringList args, QWidget *parent = nullptr);
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

    void on_actionSave_As_triggered();

    void on_actionSave_All_triggered();

    void on_actionCheck_for_updates_triggered();

    void onTabCloseRequested(int);
    void onTabChanged(int);
    void onEditorTextChanged(bool, int);
    void onSaveTimerElapsed();
    void onSettingsApplied();
    void onSplitterMoved(int, int);
    void onIncomingCompanionRequest(Network::CompanionData);
    void onViewModeToggle();

    void on_actionCompile_triggered();

    void on_actionCompile_Run_triggered();

    void on_actionRun_triggered();

    void on_actionFormat_code_triggered();

    void on_actionRun_Detached_triggered();

    void on_actionKill_Processes_triggered();

    void on_actionEditor_Mode_triggered();

    void on_actionIO_Mode_triggered();

    void on_actionSplit_Mode_triggered();

    void on_confirmTriggered(int index);

  private:
    Ui::AppWindow *ui;
    MessageLogger *activeLogger = nullptr;
    QTimer *timer = nullptr;
    QMetaObject::Connection activeSplitterMoveConnections;
    QMetaObject::Connection companionEditorConnections;
    Settings::SettingManager *settingManager = nullptr;
    Telemetry::UpdateNotifier *updater = nullptr;
    PreferenceWindow *preferenceWindow = nullptr;
    QByteArray splitterState;
    Network::CompanionServer *server;

    void setConnections();
    void allocate();
    void applySettings();
    void saveSettings();
    QVector<QShortcut *> hotkeyObjects;
    void maybeSetHotkeys();
    void updateIndexes();
    void closeAll();
    void openFile(QString fileName);
};

#endif // APPWINDOW_HPP

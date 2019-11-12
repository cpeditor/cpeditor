#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QCodeEditor>
#include <QFile>
#include <QMainWindow>
#include <SettingsManager.hpp>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow() override;
  void closeEvent(QCloseEvent* event) override;
 private slots:
  void on_actionQuit_triggered();

  void on_actionDark_Theme_triggered(bool checked);

  void on_actionWrap_Text_triggered(bool checked);

  void on_actionAuto_Indentation_triggered(bool checked);

  void on_actionAuto_Parenthesis_triggered(bool checked);

  void on_actionOpen_triggered();

  void on_actionSave_triggered();

  void on_actionSave_as_triggered();

  void on_actionAbout_triggered();

  void on_actionAbout_Qt_triggered();

  void on_actionFormat_triggered();

  void on_actionRun_triggered();

  void on_actionCompile_triggered();

  void on_compile_clicked();

  void on_run_clicked();

 private:
  Ui::MainWindow* ui;
  QCodeEditor* editor;
  QFile* openFile = nullptr;
  Settings::SettingManager* setting = nullptr;
  void setEditor();
  void setLogger();
  void setSettingsManager();
  void saveSettings();
  void restoreSettings();
};
#endif  // MAINWINDOW_HPP

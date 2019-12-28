#include "appwindow.hpp"
#include "../ui/ui_appwindow.h"
#include <QDesktopServices>
#include <QFileDialog>
#include <QMessageBox>
#include <QMimeData>
#include <QUrl>

AppWindow::AppWindow(QVector<MainWindow *> tabs, QWidget *parent)
    : AppWindow(parent) {

  if (tabs.size() > 0) {
    ui->tabWidget->clear();
    for (auto e : tabs)
      ui->tabWidget->addTab(e, e->fileName());
  }
}

AppWindow::AppWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::AppWindow) {
  ui->setupUi(this);
  ui->tabWidget->clear();
  setAcceptDrops(true);

  allocate();
  applySettings();
  setConnections();

  auto windowTemp = new MainWindow(0, "");
  ui->tabWidget->addTab(windowTemp, windowTemp->fileName());

  updater->checkUpdate();
}

AppWindow::~AppWindow() {
  delete settingManager;
  delete ui;
}

/******************* PUBLIC METHODS ***********************/

void AppWindow::closeEvent(QCloseEvent *event) {}

void AppWindow::dragEnterEvent(QDragEnterEvent *event) {
  if (event->mimeData()->hasUrls()) {
    event->acceptProposedAction();
  }
}

void AppWindow::dropEvent(QDropEvent *event) {
  auto files = event->mimeData()->urls();
  for (auto e : files) {
    auto fsp = new MainWindow(ui->tabWidget->count(), e.toLocalFile());
    ui->tabWidget->addTab(fsp, fsp->fileName());
  }
}

/******************** PRIVATE METHODS ********************/
void AppWindow::setConnections() {
  connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this,
          SLOT(onTabCloseRequested(int)));
  connect(ui->tabWidget, SIGNAL(currentChanged(int)), this,
          SLOT(onTabChanged(int)));
}

void AppWindow::allocate() {
  settingManager = new Settings::SettingManager();
  updater = new Telemetry::UpdateNotifier(settingManager->isBeta());
}

void AppWindow::applySettings() {
  ui->actionAutosave->setChecked(settingManager->isAutoSave());
  // Start auto-save daemon now.
}

/***************** ABOUT SECTION ***************************/

void AppWindow::on_actionSupport_me_triggered() {
  QDesktopServices::openUrl(
      QUrl("https://paypal.me/coder3101", QUrl::TolerantMode));
}

void AppWindow::on_actionAbout_triggered() {
  QMessageBox::about(
      this,
      QString::fromStdString(std::string("About CP Editor ") +
                             APP_VERSION_MAJOR + "." + APP_VERSION_MINOR + "." +
                             APP_VERSION_PATCH),

      "<p>The <b>CP Editor</b> is a competitive programmer's editor "
      "which can ease the task of compiling, testing and running a program "
      "so that you (a great programmer) can focus fully on your algorithm "
      "designs. </p>"
      "<p>The code repository: <a "
      "href=\"https://github.com/coder3101/cp-editor2\">"
      "https://github.com/coder3101/cp-editor2</a></p>");
}

/******************* FILES SECTION *************************/

void AppWindow::on_actionClose_Current_triggered() {
  if (ui->tabWidget->count() == 0)
    return;
  int currentIndex = ui->tabWidget->currentIndex();

  if (dynamic_cast<MainWindow *>(ui->tabWidget->widget(currentIndex))
          ->closeChangedConfirm())
    ui->tabWidget->removeTab(currentIndex);
}

void AppWindow::on_actionClose_All_triggered() {
  QVector<int> removed;
  for (int t = 0; t < ui->tabWidget->count(); t++) {
    auto tmp = dynamic_cast<MainWindow *>(ui->tabWidget->widget(t));
    if (tmp->closeChangedConfirm())
      removed.push_back(t);
  }
  for (auto idx : removed)
    ui->tabWidget->removeTab(idx);
}

void AppWindow::on_actionAutosave_triggered(bool checked) {
  settingManager->setAutoSave(checked);
  // toggleAutoSave Daemon;
}

void AppWindow::on_actionQuit_triggered() {
  QVector<int> removed;
  for (int t = 0; t < ui->tabWidget->count(); t++) {
    auto tmp = dynamic_cast<MainWindow *>(ui->tabWidget->widget(t));
    if (tmp->closeChangedConfirm())
      removed.push_back(t);
  }
  for (auto idx : removed)
    ui->tabWidget->removeTab(idx);
  if (ui->tabWidget->count() == 0)
    QApplication::exit();
}

void AppWindow::on_actionNew_Tab_triggered() {
  auto temp = new MainWindow(ui->tabWidget->count(), "");
  ui->tabWidget->addTab(temp, temp->fileName());
}

void AppWindow::on_actionOpen_triggered() {
  auto fileName = QFileDialog::getOpenFileName(
      this, tr("Open File"), "",
      "Source Files (*.cpp *.hpp *.h *.cc *.cxx *.c *.py *.py3 *.java)");
  if (fileName.isEmpty())
    return;
  auto tmp = new MainWindow(ui->tabWidget->count(), fileName);
  ui->tabWidget->addTab(tmp, tmp->fileName());
}

void AppWindow::on_actionSave_triggered() {
  int currentIdx = ui->tabWidget->currentIndex();
  auto tmp = dynamic_cast<MainWindow *>(ui->tabWidget->widget(currentIdx));
  tmp->save();
}

void AppWindow::on_actionSave_as_triggered() {
  int currentIdx = ui->tabWidget->currentIndex();
  auto tmp = dynamic_cast<MainWindow *>(ui->tabWidget->widget(currentIdx));
  tmp->saveAs();
}

/************************ PREFERENCES SECTION **********************/

void AppWindow::on_actionRestore_Settings_triggered() {
  auto res = QMessageBox::question(this, "Reset preferences?",
                                   "Are you sure you want to reset the"
                                   " all preferences to default?",
                                   QMessageBox::Yes | QMessageBox::No);
  if (res == QMessageBox::Yes) {
  }
}

void AppWindow::on_actionSettings_triggered() {}

/************************** SLOTS *********************************/

void AppWindow::onTabCloseRequested(int index) {
  auto tmp = dynamic_cast<MainWindow *>(ui->tabWidget->widget(index));
  if (tmp->closeChangedConfirm())
    ui->tabWidget->removeTab(index);
}

void AppWindow::onTabChanged(int index){
    auto tmp = dynamic_cast<MainWindow*>(ui->tabWidget->widget(index));
    // use this to put astrick on the tab name for unsaved changes.
}

// Todo : Make Logger unstatic
// Implement all actions accordingly
// Make the Preferences window
// Add other themes
// Make companion, parse contests

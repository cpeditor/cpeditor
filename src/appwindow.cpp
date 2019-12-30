#include "appwindow.hpp"
#include "../ui/ui_appwindow.h"
#include <QDesktopServices>
#include <QFileDialog>
#include <QMessageBox>
#include <QMetaMethod>
#include <QMimeData>
#include <QTimer>
#include <QUrl>

AppWindow::AppWindow(QVector<MainWindow *> tabs, QWidget *parent) : AppWindow(parent)
{

    if (tabs.size() > 0)
    {
        ui->tabWidget->clear();
        for (auto e : tabs)
            ui->tabWidget->addTab(e, e->fileName());
    }
}

AppWindow::AppWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::AppWindow)
{
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

AppWindow::~AppWindow()
{
    saveSettings();
    delete settingManager;
    delete ui;
}

/******************* PUBLIC METHODS ***********************/

void AppWindow::closeEvent(QCloseEvent *event)
{
    for (int t = 0; t < ui->tabWidget->count(); t++)
    {
        auto tmp = dynamic_cast<MainWindow *>(ui->tabWidget->widget(t));
        if (tmp->closeChangedConfirm())
        {
            ui->tabWidget->removeTab(t);
            t--;
        }
    }
    if (ui->tabWidget->count() == 0)
        event->accept();
    else
        event->ignore();
}

void AppWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
    }
}

void AppWindow::dropEvent(QDropEvent *event)
{
    auto files = event->mimeData()->urls();
    for (auto e : files)
    {
        auto fileName = e.toLocalFile();
        auto fsp = new MainWindow(ui->tabWidget->count(), fileName);
        QString lang = "Cpp";
        if(fileName.endsWith(".java")) lang = "Java";
        else if(fileName.endsWith(".py") || fileName.endsWith(".py3")) lang = "Python";
        ui->tabWidget->addTab(fsp, fsp->fileName());
        fsp->setLanguage(lang);
    }
}

/******************** PRIVATE METHODS ********************/
void AppWindow::setConnections()
{
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(onTabCloseRequested(int)));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(onTabChanged(int)));
    connect(timer, SIGNAL(timeout()), this, SLOT(onSaveTimerElapsed()));

    connect(preferenceWindow, SIGNAL(settingsApplied()), this, SLOT(onSettingsApplied()));
}

void AppWindow::allocate()
{
    settingManager = new Settings::SettingManager();
    timer = new QTimer();
    updater = new Telemetry::UpdateNotifier(settingManager->isBeta());
    preferenceWindow = new PreferenceWindow(settingManager, this);

    timer->setInterval(3000);
    timer->setSingleShot(false);
}

void AppWindow::applySettings()
{
    ui->actionAutosave->setChecked(settingManager->isAutoSave());
    if (settingManager->isAutoSave())
        timer->start();

    if (!settingManager->getGeometry().isEmpty() &&
        !settingManager->getGeometry().isNull() &&
        settingManager->getGeometry().isValid() &&
        !settingManager->isMaximizedWindow())
     {
         this->setGeometry(settingManager->getGeometry());
     }

     if (settingManager->isMaximizedWindow())
     {
         this->showMaximized();
     }

}

void AppWindow::saveSettings()
{
    if (!this->isMaximized())
            settingManager->setGeometry(this->geometry());
    settingManager->setMaximizedWindow(this->isMaximized());

}

/***************** ABOUT SECTION ***************************/

void AppWindow::on_actionSupport_me_triggered()
{
    QDesktopServices::openUrl(QUrl("https://paypal.me/coder3101", QUrl::TolerantMode));
}

void AppWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this,
                       QString::fromStdString(std::string("About CP Editor ") + APP_VERSION_MAJOR + "." +
                                              APP_VERSION_MINOR + "." + APP_VERSION_PATCH),

                       "<p>The <b>CP Editor</b> is a competitive programmer's editor "
                       "which can ease the task of compiling, testing and running a program "
                       "so that you (a great programmer) can focus fully on your algorithm "
                       "designs. </p>"
                       "<p>The code repository: <a "
                       "href=\"https://github.com/coder3101/cp-editor2\">"
                       "https://github.com/coder3101/cp-editor2</a></p>");
}

/******************* FILES SECTION *************************/

void AppWindow::on_actionClose_All_triggered()
{
    for (int t = 0; t < ui->tabWidget->count(); t++)
    {
        auto tmp = dynamic_cast<MainWindow *>(ui->tabWidget->widget(t));
        if (tmp->closeChangedConfirm())
        {
            ui->tabWidget->removeTab(t);
            t--;
        }
    }
}

void AppWindow::on_actionAutosave_triggered(bool checked)
{
    settingManager->setAutoSave(checked);
    if (checked)
        timer->start();
    else
        timer->stop();
}

void AppWindow::on_actionQuit_triggered()
{
    for (int t = 0; t < ui->tabWidget->count(); t++)
    {
        auto tmp = dynamic_cast<MainWindow *>(ui->tabWidget->widget(t));
        if (tmp->closeChangedConfirm())
        {
            ui->tabWidget->removeTab(t);
            t--;
        }
    }
    if (ui->tabWidget->count() == 0)
        QApplication::exit();
}

void AppWindow::on_actionNew_Tab_triggered()
{
    auto temp = new MainWindow(ui->tabWidget->count(), "");
    ui->tabWidget->addTab(temp, temp->fileName());
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
}

void AppWindow::on_actionOpen_triggered()
{
    auto fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
                                                 "Source Files (*.cpp *.hpp *.h *.cc *.cxx *.c *.py *.py3 *.java)");
    if (fileName.isEmpty())
        return;

    QString lang = "Cpp";
    if(fileName.endsWith(".java")) lang = "Java";
    else if(fileName.endsWith(".py") || fileName.endsWith(".py3")) lang = "Python";

    for (int t = 0; t < ui->tabWidget->count(); t++)
    {
        auto tmp = dynamic_cast<MainWindow *>(ui->tabWidget->widget(t));
        if (fileName == tmp->filePath())
        {
            ui->tabWidget->setCurrentIndex(t);
            return;
        }
    }

    auto tmp = new MainWindow(ui->tabWidget->count(), fileName);
    ui->tabWidget->addTab(tmp, tmp->fileName());
    tmp->setLanguage(lang);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
}

void AppWindow::on_actionSave_triggered()
{
    int currentIdx = ui->tabWidget->currentIndex();
    auto tmp = dynamic_cast<MainWindow *>(ui->tabWidget->widget(currentIdx));
    tmp->save(true);
    onEditorTextChanged(false);
}

void AppWindow::on_actionSave_as_triggered()
{
    int currentIdx = ui->tabWidget->currentIndex();
    auto tmp = dynamic_cast<MainWindow *>(ui->tabWidget->widget(currentIdx));
    tmp->saveAs();
    onEditorTextChanged(false);
}

/************************ PREFERENCES SECTION **********************/

void AppWindow::on_actionRestore_Settings_triggered()
{
    auto res = QMessageBox::question(this, "Reset preferences?",
                                     "Are you sure you want to reset the"
                                     " all preferences to default?",
                                     QMessageBox::Yes | QMessageBox::No);
    if (res == QMessageBox::Yes)
    {
        preferenceWindow->resetSettings();
        onSettingsApplied();
    }
}

void AppWindow::on_actionSettings_triggered()
{
    preferenceWindow->show();
}

/************************** SLOTS *********************************/

void AppWindow::onTabCloseRequested(int index)
{
    //splitterState.clear();
    auto tmp = dynamic_cast<MainWindow *>(ui->tabWidget->widget(index));
    if (tmp->closeChangedConfirm())
        ui->tabWidget->removeTab(index);
}

void AppWindow::onTabChanged(int index)
{
    if (index == -1)
    {
        activeLogger = nullptr;
        return;
    }

    disconnect(activeTextChangeConnections);
    disconnect(activeSplitterMoveConnections);

    auto tmp = dynamic_cast<MainWindow *>(ui->tabWidget->widget(index));
    activeLogger = tmp->getLogger();
    tmp->setSettingsData(settingManager->toData());
    tmp->maybeLoadTemplate();

    if (!splitterState.isEmpty())
        tmp->getSplitter()->restoreState(splitterState);

    activeTextChangeConnections = connect(tmp, SIGNAL(editorTextChanged(bool)), this, SLOT(onEditorTextChanged(bool)));
    activeSplitterMoveConnections = connect(tmp->getSplitter(), SIGNAL(splitterMoved(int,int)), this, SLOT(onSplitterMoved(int,int)));
}

void AppWindow::onEditorTextChanged(bool isUnsaved)
{
    auto current = ui->tabWidget->currentIndex();
    if (isUnsaved)
    {
        if (!ui->tabWidget->tabText(current).endsWith("*"))
            ui->tabWidget->setTabText(current, ui->tabWidget->tabText(current) + "*");
    }
    else
    {
        if (ui->tabWidget->tabText(current).endsWith("*"))
        {
            auto name = dynamic_cast<MainWindow *>(ui->tabWidget->widget(current))->fileName();
            ui->tabWidget->setTabText(current, name);
        }
    }
}

void AppWindow::onSaveTimerElapsed()
{
    for (int t = 0; t < ui->tabWidget->count(); t++)
    {
        auto tmp = dynamic_cast<MainWindow *>(ui->tabWidget->widget(t));
        if (tmp->getOpenFile() != nullptr && tmp->getOpenFile()->isOpen())
        {
            ui->tabWidget->setTabText(t, tmp->fileName());
            tmp->save(false);
        }
    }
}

void AppWindow::onSettingsApplied()
{
}

void AppWindow::onSplitterMoved(int _,int __){
    int current = ui->tabWidget->currentIndex();
    auto splitter = dynamic_cast<MainWindow*>(ui->tabWidget->widget(current))->getSplitter();
    splitterState = splitter->saveState();
}

// Implement all actions accordingly
// Make the Preferences window
// Add other themes
// Make companion, parse contests

void AppWindow::on_actionCheck_for_updates_triggered()
{
    updater->checkUpdate(true);
}

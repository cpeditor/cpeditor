#include "appwindow.hpp"
#include "../ui/ui_appwindow.h"
#include <EditorTheme.hpp>
#include <QDesktopServices>
#include <QFileDialog>
#include <QMessageBox>
#include <QMetaMethod>
#include <QMimeData>
#include <QTimer>
#include <QUrl>

AppWindow::AppWindow(QStringList args, QWidget *parent) : AppWindow(parent)
{
    ui->tabWidget->clear();
    if (args.size() > 1)
        for (int i = 1; i < args.size(); ++i)
            openFile(args[i]);
    else
        openFile("");
}

AppWindow::AppWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::AppWindow)
{
    ui->setupUi(this);
    ui->tabWidget->clear();
    ui->tabWidget->setMovable(true);
    setAcceptDrops(true);

    allocate();
    setConnections();

    auto windowTemp = new MainWindow(0, "");
    ui->tabWidget->addTab(windowTemp, windowTemp->fileName());

    if (settingManager->isCheckUpdateOnStartup())
        updater->checkUpdate();

    applySettings();
    onSettingsApplied();
}

AppWindow::~AppWindow()
{
    saveSettings();
    Themes::EditorTheme::release();
    delete settingManager;
    delete ui;
    delete preferenceWindow;
    delete timer;
    delete updater;
    delete server;
}

/******************* PUBLIC METHODS ***********************/

void AppWindow::closeEvent(QCloseEvent *event)
{
    closeAll();
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
        openFile(fileName);
    }
}

/******************** PRIVATE METHODS ********************/
void AppWindow::setConnections()
{
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(onTabCloseRequested(int)));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(onTabChanged(int)));
    connect(timer, SIGNAL(timeout()), this, SLOT(onSaveTimerElapsed()));

    connect(preferenceWindow, SIGNAL(settingsApplied()), this, SLOT(onSettingsApplied()));

    if (settingManager->isCompetitiveCompanionActive())
        companionEditorConnections =
            connect(server, &Network::CompanionServer::onRequestArrived, this, &AppWindow::onIncomingCompanionRequest);
}

void AppWindow::allocate()
{
    settingManager = new Settings::SettingManager();
    timer = new QTimer();
    updater = new Telemetry::UpdateNotifier(settingManager->isBeta());
    preferenceWindow = new PreferenceWindow(settingManager, this);
    server = new Network::CompanionServer(settingManager->getConnectionPort());

    timer->setInterval(3000);
    timer->setSingleShot(false);
}

void AppWindow::applySettings()
{
    ui->actionAutosave->setChecked(settingManager->isAutoSave());
    Settings::ViewMode mode = settingManager->getViewMode();

    switch (mode)
    {
    case Settings::ViewMode::FULL_EDITOR:
        on_actionEditor_Mode_triggered();
        break;
    case Settings::ViewMode::FULL_IO:
        on_actionIO_Mode_triggered();
        break;
    case Settings::ViewMode::SPLIT:
        on_actionSplit_Mode_triggered();
    }

    if (settingManager->isAutoSave())
        timer->start();

    if (!settingManager->getGeometry().isEmpty() && !settingManager->getGeometry().isNull() &&
        settingManager->getGeometry().isValid() && !settingManager->isMaximizedWindow())
    {
        setGeometry(settingManager->getGeometry());
    }

    if (settingManager->isMaximizedWindow())
    {
        this->showMaximized();
    }

    maybeSetHotkeys();
}

void AppWindow::maybeSetHotkeys()
{
    for (auto e : hotkeyObjects)
        delete e;
    hotkeyObjects.clear();

    if (!settingManager->isHotkeyInUse())
        return;

    if (!settingManager->getHotkeyRun().isEmpty())
    {
        hotkeyObjects.push_back(new QShortcut(settingManager->getHotkeyRun(), this, SLOT(on_actionRun_triggered())));
    }
    if (!settingManager->getHotkeyCompile().isEmpty())
    {
        hotkeyObjects.push_back(
            new QShortcut(settingManager->getHotkeyCompile(), this, SLOT(on_actionCompile_triggered())));
    }
    if (!settingManager->getHotkeyCompileRun().isEmpty())
    {
        hotkeyObjects.push_back(
            new QShortcut(settingManager->getHotkeyCompileRun(), this, SLOT(on_actionCompile_Run_triggered())));
    }
    if (!settingManager->getHotkeyFormat().isEmpty())
    {
        hotkeyObjects.push_back(
            new QShortcut(settingManager->getHotkeyFormat(), this, SLOT(on_actionFormat_code_triggered())));
    }
    if (!settingManager->getHotkeyKill().isEmpty())
    {
        hotkeyObjects.push_back(
            new QShortcut(settingManager->getHotkeyKill(), this, SLOT(on_actionKill_Processes_triggered())));
    }
    if (!settingManager->getHotkeyViewModeToggler().isEmpty())
    {
        hotkeyObjects.push_back(
            new QShortcut(settingManager->getHotkeyViewModeToggler(), this, SLOT(onViewModeToggle())));
    }
}

void AppWindow::closeAll()
{
    for (int t = 0; t < ui->tabWidget->count(); t++)
        if (closeTab(t))
            --t;
}

bool AppWindow::closeTab(int index)
{
    auto tmp = dynamic_cast<MainWindow *>(ui->tabWidget->widget(index));
    if (tmp->closeConfirm())
    {
        ui->tabWidget->removeTab(index);
        return true;
    }
    return false;
}

void AppWindow::saveSettings()
{
    if (!this->isMaximized())
        settingManager->setGeometry(this->geometry());
    settingManager->setMaximizedWindow(this->isMaximized());
}

void AppWindow::openFile(QString fileName)
{
    if (!fileName.isEmpty())
    {
        for (int t = 0; t < ui->tabWidget->count(); t++)
        {
            auto tmp = dynamic_cast<MainWindow *>(ui->tabWidget->widget(t));
            if (fileName == tmp->filePath())
            {
                ui->tabWidget->setCurrentIndex(t);
                return;
            }
        }
    }

    int t = ui->tabWidget->count();
    auto fsp = new MainWindow(t, fileName);
    connect(fsp, SIGNAL(confirmTriggered(MainWindow *)), this, SLOT(on_confirmTriggered(MainWindow *)));
    connect(fsp, SIGNAL(editorTextChanged(bool, MainWindow *)), this, SLOT(onEditorTextChanged(bool, MainWindow *)));
    QString lang = "Cpp";
    if (fileName.endsWith(".java"))
        lang = "Java";
    else if (fileName.endsWith(".py") || fileName.endsWith(".py3"))
        lang = "Python";
    ui->tabWidget->addTab(fsp, fsp->fileName());
    fsp->setLanguage(lang);
    ui->tabWidget->setCurrentIndex(t);
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
    closeAll();
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
    closeAll();
    if (ui->tabWidget->count() == 0)
        QApplication::exit();
}

void AppWindow::on_actionNew_Tab_triggered()
{
    openFile("");
}

void AppWindow::on_actionOpen_triggered()
{
    auto fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
                                                 "Source Files (*.cpp *.hpp *.h *.cc *.cxx *.c *.py *.py3 *.java)");
    if (fileName.isEmpty())
        return;

    openFile(fileName);
}

void AppWindow::on_actionSave_triggered()
{
    int currentIdx = ui->tabWidget->currentIndex();
    auto tmp = dynamic_cast<MainWindow *>(ui->tabWidget->widget(currentIdx));
    tmp->save(true);
}

void AppWindow::on_actionSave_As_triggered()
{
    int currentIdx = ui->tabWidget->currentIndex();
    auto tmp = dynamic_cast<MainWindow *>(ui->tabWidget->widget(currentIdx));
    tmp->saveAs();
}

void AppWindow::on_actionSave_All_triggered()
{
    for (int t = 0; t < ui->tabWidget->count(); ++t)
    {
        auto tmp = dynamic_cast<MainWindow *>(ui->tabWidget->widget(t));
        tmp->save(true);
    }
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
    preferenceWindow->updateShow();
}

/************************** SLOTS *********************************/

void AppWindow::onTabCloseRequested(int index)
{
    closeTab(index);
}

void AppWindow::onTabChanged(int index)
{
    if (index == -1)
    {
        activeLogger = nullptr;
        server->setMessageLogger(nullptr);
        setWindowTitle("CP Editor: Competitive Programmers Editor");
        return;
    }

    disconnect(activeSplitterMoveConnections);

    auto tmp = dynamic_cast<MainWindow *>(ui->tabWidget->widget(index));

    if (tmp->getOpenFile() == nullptr)
        setWindowTitle("CP Editor: " + tmp->fileName());
    else
        setWindowTitle("CP Editor: " + tmp->filePath());

    activeLogger = tmp->getLogger();
    server->setMessageLogger(activeLogger);

    if(settingManager->isCompetitiveCompanionActive())
        server->checkServer();

    tmp->setSettingsData(settingManager->toData(), diagonistics);
    diagonistics = false;
    tmp->maybeLoadTemplate();

    if (!splitterState.isEmpty())
        tmp->getSplitter()->restoreState(splitterState);

    activeSplitterMoveConnections =
        connect(tmp->getSplitter(), SIGNAL(splitterMoved(int, int)), this, SLOT(onSplitterMoved(int, int)));
}

void AppWindow::onEditorTextChanged(bool isUnsaved, MainWindow *widget)
{
    int index = ui->tabWidget->indexOf(widget);
    if (index == -1)
        return;
    auto name = dynamic_cast<MainWindow *>(ui->tabWidget->widget(index))->fileName();
    if (isUnsaved)
        ui->tabWidget->setTabText(index, name + " *");
    else
        ui->tabWidget->setTabText(index, name);
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
    if (settingManager->isSystemThemeDark())
    {
        QMessageBox::warning(this, "Don't use dark theme",
                             "System-wide dark theme is not officially supported by Qt or the developer"
                             ". It has some serious UI/UX issues and should not be used. Instead you can"
                             " turn system theme dark on your PC settings.");

        settingManager->setSystemThemeDark(false);
    }

    updater->setBeta(settingManager->isBeta());
    maybeSetHotkeys();

    disconnect(companionEditorConnections);

    server->updatePort(settingManager->getConnectionPort());

    if (settingManager->isCompetitiveCompanionActive())
        companionEditorConnections =
            connect(server, &Network::CompanionServer::onRequestArrived, this, &AppWindow::onIncomingCompanionRequest);
    diagonistics = true;
    onTabChanged(ui->tabWidget->currentIndex());
}

void AppWindow::onIncomingCompanionRequest(Network::CompanionData data)
{
<<<<<<< HEAD
    auto newTab = new MainWindow(ui->tabWidget->currentIndex(), "");
    newTab->setSettingsData(settingManager->toData(), true);
    newTab->maybeLoadTemplate();
    newTab->applyCompanion(data);
    ui->tabWidget->addTab(newTab, newTab->fileName());
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);
=======
    auto current = ui->tabWidget->currentIndex();
    if (current == -1)
    {
        openFile("");
        current = 0;
    }
    auto tmp = dynamic_cast<MainWindow *>(ui->tabWidget->widget(current));
    tmp->applyCompanion(data);
>>>>>>> 3fa4eb0dfc10c8ec8626611469623afbb7eb7db1
}

void AppWindow::onViewModeToggle()
{
    if (ui->actionEditor_Mode->isChecked())
    {
        on_actionIO_Mode_triggered();
        return;
    }
    if (ui->actionSplit_Mode->isChecked())
    {
        on_actionEditor_Mode_triggered();
        return;
    }
    if (ui->actionIO_Mode->isChecked())
    {
        on_actionSplit_Mode_triggered();
        return;
    }
}

void AppWindow::onSplitterMoved(int _, int __)
{
    int current = ui->tabWidget->currentIndex();
    auto splitter = dynamic_cast<MainWindow *>(ui->tabWidget->widget(current))->getSplitter();
    splitterState = splitter->saveState();
}

/************************* ACTIONS ************************/
void AppWindow::on_actionCheck_for_updates_triggered()
{
    updater->checkUpdate(true);
}

void AppWindow::on_actionCompile_triggered()
{
    int current = ui->tabWidget->currentIndex();
    auto tmp = dynamic_cast<MainWindow *>(ui->tabWidget->widget(current));
    tmp->compile();
}

void AppWindow::on_actionCompile_Run_triggered()
{
    int current = ui->tabWidget->currentIndex();
    auto tmp = dynamic_cast<MainWindow *>(ui->tabWidget->widget(current));
    tmp->runAndCompile();
}

void AppWindow::on_actionRun_triggered()
{
    int current = ui->tabWidget->currentIndex();
    auto tmp = dynamic_cast<MainWindow *>(ui->tabWidget->widget(current));
    tmp->run();
}

void AppWindow::on_actionFormat_code_triggered()
{
    int current = ui->tabWidget->currentIndex();
    auto tmp = dynamic_cast<MainWindow *>(ui->tabWidget->widget(current));
    tmp->formatSource();
}

void AppWindow::on_actionRun_Detached_triggered()
{
    int current = ui->tabWidget->currentIndex();
    auto tmp = dynamic_cast<MainWindow *>(ui->tabWidget->widget(current));
    tmp->detachedExecution();
}

void AppWindow::on_actionKill_Processes_triggered()
{
    int current = ui->tabWidget->currentIndex();
    auto tmp = dynamic_cast<MainWindow *>(ui->tabWidget->widget(current));
    tmp->killProcesses();
}

void AppWindow::on_actionEditor_Mode_triggered()
{
    settingManager->setViewMode(Settings::ViewMode::FULL_EDITOR);
    ui->actionEditor_Mode->setChecked(true);
    ui->actionIO_Mode->setChecked(false);
    ui->actionSplit_Mode->setChecked(false);
    onTabChanged(ui->tabWidget->currentIndex());
}

void AppWindow::on_actionIO_Mode_triggered()
{
    settingManager->setViewMode(Settings::ViewMode::FULL_IO);
    ui->actionEditor_Mode->setChecked(false);
    ui->actionIO_Mode->setChecked(true);
    ui->actionSplit_Mode->setChecked(false);
    onTabChanged(ui->tabWidget->currentIndex());
}

void AppWindow::on_actionSplit_Mode_triggered()
{
    settingManager->setViewMode(Settings::ViewMode::SPLIT);
    ui->actionEditor_Mode->setChecked(false);
    ui->actionIO_Mode->setChecked(false);
    ui->actionSplit_Mode->setChecked(true);
    onTabChanged(ui->tabWidget->currentIndex());
}

void AppWindow::on_confirmTriggered(MainWindow *widget)
{
    int index = ui->tabWidget->indexOf(widget);
    if (index != -1)
        ui->tabWidget->setCurrentIndex(index);
}

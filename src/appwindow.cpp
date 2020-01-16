/*
 * Copyright (C) 2019-2020 Ashar Khan <ashar786khan@gmail.com>
 *
 * This file is part of CPEditor.
 *
 * CPEditor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * I will not be responsible if CPEditor behaves in unexpected way and
 * causes your ratings to go down and or loose any important contest.
 *
 * Believe Software is "Software" and it isn't immune to bugs.
 *
 */

#include "appwindow.hpp"
#include "../ui/ui_appwindow.h"
#include <EditorTheme.hpp>
#include <QDesktopServices>
#include <QFileDialog>
#include <QInputDialog>
#include <QMap>
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
            openTab(args[i]);
    else
        openTab("");
}

AppWindow::AppWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::AppWindow)
{
    ui->setupUi(this);
    ui->tabWidget->clear();
    setAcceptDrops(true);

    allocate();
    setConnections();

    auto windowTemp = new MainWindow(0, "", settingManager->toData());
    ui->tabWidget->addTab(windowTemp, windowTemp->getFileName());

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
        openTab(fileName);
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
    if (!settingManager->getHotkeySnippets().isEmpty())
    {
        hotkeyObjects.push_back(
            new QShortcut(settingManager->getHotkeySnippets(), this, SLOT(on_actionUse_Snippets_triggered())));
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
    auto tmp = windowIndex(index);
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

void AppWindow::openTab(QString fileName, bool iscompanionOpenedTab)
{
    if (!iscompanionOpenedTab)
    {
        if (!fileName.isEmpty())
        {
            auto fileInfo = QFileInfo(fileName);
            for (int t = 0; t < ui->tabWidget->count(); t++)
            {
                auto tmp = dynamic_cast<MainWindow *>(ui->tabWidget->widget(t));
                if (fileInfo == QFileInfo(tmp->getFilePath()))
                {
                    ui->tabWidget->setCurrentIndex(t);
                    return;
                }
            }
        }

        int t = ui->tabWidget->count();
        auto fsp = new MainWindow(t, fileName, settingManager->toData());
        connect(fsp, SIGNAL(confirmTriggered(MainWindow *)), this, SLOT(on_confirmTriggered(MainWindow *)));
        connect(fsp, SIGNAL(editorChanged(MainWindow *)), this, SLOT(onEditorChanged(MainWindow *)));
        QString lang = settingManager->getDefaultLang();

        if (fileName.endsWith(".java"))
            lang = "Java";
        else if (fileName.endsWith(".py") || fileName.endsWith(".py3"))
            lang = "Python";
        else if (fileName.endsWith(".cpp") || fileName.endsWith(".cxx") || fileName.endsWith(".c") ||
                 fileName.endsWith(".cc") || fileName.endsWith(".hpp") || fileName.endsWith(".h"))
            lang = "Cpp";

        ui->tabWidget->addTab(fsp, fsp->getFileName());
        fsp->setLanguage(lang);
        ui->tabWidget->setCurrentIndex(t);
    }
    else
    {
        for (int t = 0; t < ui->tabWidget->count(); t++)
        {
            auto tmp = windowIndex(t);
            if (fileName == tmp->getProblemURL())
            {
                ui->tabWidget->setCurrentIndex(t);
                return;
            }
        }

        int t = ui->tabWidget->count();
        auto fsp = new MainWindow(t, "", settingManager->toData());
        connect(fsp, SIGNAL(confirmTriggered(MainWindow *)), this, SLOT(on_confirmTriggered(MainWindow *)));
        connect(fsp, SIGNAL(editorChanged(MainWindow *)), this, SLOT(onEditorChanged(MainWindow *)));
        ui->tabWidget->addTab(fsp, fsp->getFileName());
        ui->tabWidget->setCurrentIndex(t);
    }
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
                       "<p><b>CP Editor</b> is a native Qt-based code editor written in C++. It's specially designed "
                       "for competitive programming, unlike other editors/IDEs which are mainly for developers. It "
                       "helps you focus on your coding and automates the compilation, executing and testing. It even "
                       "fetches test cases for you from webpages and submits codes on Codeforces!</p>"
                       "<p>Copyright (C) 2019-2020 Ashar Khan &lt;ashar786khan@gmail.com&gt;</p>"
                       "<p>This is free software; see the source for copying conditions. There is NO warranty; not "
                       "even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. The source code for CP Editor is "
                       "available at <a href=\"https://github.com/coder3101/cp-editor\"> "
                       "https://github.com/coder3101/cp-editor</a>.</p>");
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
    openTab("");
}

void AppWindow::on_actionOpen_triggered()
{
    auto fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",
                                                 "Source Files (*.cpp *.hpp *.h *.cc *.cxx *.c *.py *.py3 *.java)");
    if (fileName.isEmpty())
        return;

    openTab(fileName);
}

void AppWindow::on_actionSave_triggered()
{
    currentWindow()->save(true);
}

void AppWindow::on_actionSave_As_triggered()
{
    currentWindow()->saveAs();
}

void AppWindow::on_actionSave_All_triggered()
{
    for (int t = 0; t < ui->tabWidget->count(); ++t)
    {
        auto tmp = windowIndex(t);
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
        settingManager->resetSettings();
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
        setWindowTitle("CP Editor: An editor specially designed for competitive programming");
        return;
    }

    disconnect(activeSplitterMoveConnections);

    auto tmp = windowIndex(index);

    if (tmp->isUntitled())
        setWindowTitle("CP Editor: " + tmp->getFileName());
    else
        setWindowTitle("CP Editor: " + tmp->getFilePath());

    activeLogger = tmp->getLogger();
    server->setMessageLogger(activeLogger);

    if (settingManager->isCompetitiveCompanionActive() && diagonistics)
        server->checkServer();

    tmp->setSettingsData(settingManager->toData(), diagonistics);
    diagonistics = false;

    if (ui->actionEditor_Mode->isChecked())
        on_actionEditor_Mode_triggered();
    else if (ui->actionIO_Mode->isChecked())
        on_actionIO_Mode_triggered();
    else if (ui->actionSplit_Mode->isChecked())
        on_actionSplit_Mode_triggered();

    activeSplitterMoveConnections =
        connect(tmp->getSplitter(), SIGNAL(splitterMoved(int, int)), this, SLOT(onSplitterMoved(int, int)));
}

void AppWindow::onEditorChanged(MainWindow *widget)
{
    if (widget == currentWindow())
    {
        if (widget->isUntitled())
            setWindowTitle("CP Editor: " + widget->getFileName());
        else
            setWindowTitle("CP Editor: " + widget->getFilePath());
    }

    QMap<QString, QVector<int>> tabsByName;

    for (int t = 0; t < ui->tabWidget->count(); ++t)
    {
        tabsByName[windowIndex(t)->getFileName()].push_back(t);
    }

    for (auto tabs : tabsByName)
    {
        for (auto index : tabs)
        {
            ui->tabWidget->setTabText(index, windowIndex(index)->getTabTitle(tabs.size() > 1));
        }
    }
}

void AppWindow::onSaveTimerElapsed()
{
    for (int t = 0; t < ui->tabWidget->count(); t++)
    {
        auto tmp = windowIndex(t);
        if (!tmp->isUntitled())
        {
            tmp->save(false);
        }
    }
}

void AppWindow::onSettingsApplied()
{
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
    openTab(data.url, true);
    int current = ui->tabWidget->currentIndex();
    currentWindow()->applyCompanion(data);
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
    auto splitter = currentWindow()->getSplitter();
    settingManager->setSplitterSizes(splitter->saveState());
}

/************************* ACTIONS ************************/
void AppWindow::on_actionCheck_for_updates_triggered()
{
    updater->checkUpdate(true);
}

void AppWindow::on_actionCompile_triggered()
{
    if (ui->actionEditor_Mode->isChecked())
        on_actionSplit_Mode_triggered();
    currentWindow()->compile();
}

void AppWindow::on_actionCompile_Run_triggered()
{
    if (ui->actionEditor_Mode->isChecked())
        on_actionSplit_Mode_triggered();
    currentWindow()->runAndCompile();
}

void AppWindow::on_actionRun_triggered()
{
    if (ui->actionEditor_Mode->isChecked())
        on_actionSplit_Mode_triggered();
    currentWindow()->run();
}

void AppWindow::on_actionFormat_code_triggered()
{
    currentWindow()->formatSource();
}

void AppWindow::on_actionRun_Detached_triggered()
{
    currentWindow()->detachedExecution();
}

void AppWindow::on_actionKill_Processes_triggered()
{
    currentWindow()->killProcesses();
}

void AppWindow::on_actionUse_Snippets_triggered()
{
    auto current = currentWindow();
    if (current != nullptr)
    {
        auto lang = current->getLanguage();
        auto names = settingManager->getSnippetsNames(lang);
        if (names.isEmpty())
        {
            activeLogger->warn("Snippets", "There are no snippets for " + lang.toStdString() +
                                               ". Please add snippets in the preference window.");
        }
        else
        {
            auto ok = new bool;
            auto name = QInputDialog::getItem(this, tr("Use Snippets"), tr("Choose a snippet:"), names, 0, true, ok);
            if (*ok)
            {
                if (names.contains(name))
                {
                    auto content = settingManager->getSnippet(lang, name);
                    current->insertText(content);
                }
                else
                {
                    activeLogger->warn("Snippets", "There is no snippet named " + name.toStdString() + " for " +
                                                       lang.toStdString());
                }
            }
        }
    }
}

void AppWindow::on_actionEditor_Mode_triggered()
{
    settingManager->setViewMode(Settings::ViewMode::FULL_EDITOR);
    ui->actionEditor_Mode->setChecked(true);
    ui->actionIO_Mode->setChecked(false);
    ui->actionSplit_Mode->setChecked(false);
    currentWindow()->getSplitter()->setSizes({1, 0});
}

void AppWindow::on_actionIO_Mode_triggered()
{
    settingManager->setViewMode(Settings::ViewMode::FULL_IO);
    ui->actionEditor_Mode->setChecked(false);
    ui->actionIO_Mode->setChecked(true);
    ui->actionSplit_Mode->setChecked(false);
    currentWindow()->getSplitter()->setSizes({0, 1});
}

void AppWindow::on_actionSplit_Mode_triggered()
{
    settingManager->setViewMode(Settings::ViewMode::SPLIT);
    ui->actionEditor_Mode->setChecked(false);
    ui->actionIO_Mode->setChecked(false);
    ui->actionSplit_Mode->setChecked(true);
    auto state = settingManager->getSplitterSizes();
    currentWindow()->getSplitter()->restoreState(state);
}

void AppWindow::on_confirmTriggered(MainWindow *widget)
{
    int index = ui->tabWidget->indexOf(widget);
    if (index != -1)
        ui->tabWidget->setCurrentIndex(index);
}

MainWindow *AppWindow::currentWindow()
{
    int current = ui->tabWidget->currentIndex();
    if (current == -1)
        return nullptr;
    return dynamic_cast<MainWindow *>(ui->tabWidget->widget(current));
}

MainWindow *AppWindow::windowIndex(int index)
{
    if (index == -1)
        return nullptr;
    return dynamic_cast<MainWindow *>(ui->tabWidget->widget(index));
}

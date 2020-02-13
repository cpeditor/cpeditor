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
#include "Core/EventLogger.hpp"
#include "Extensions/EditorTheme.hpp"
#include <QClipboard>
#include <QDesktopServices>
#include <QFileDialog>
#include <QInputDialog>
#include <QJsonDocument>
#include <QMap>
#include <QMessageBox>
#include <QMetaMethod>
#include <QMimeData>
#include <QProgressDialog>
#include <QTimer>
#include <QUrl>

AppWindow::AppWindow(bool noHotExit, QWidget *parent) : QMainWindow(parent), ui(new Ui::AppWindow)
{
    ui->setupUi(this);
    setAcceptDrops(true);
    allocate();
    setConnections();

    if (settingManager->isCheckUpdateOnStartup())
        updater->checkUpdate();

    setWindowOpacity(settingManager->getTransparency() / 100.0);

    applySettings();
    onSettingsApplied();

    if (!noHotExit && settingManager->isUseHotExit())
    {
        int length = settingManager->getNumberOfTabs();

        QProgressDialog progress(this);
        progress.setWindowModality(Qt::WindowModal);
        progress.setWindowTitle("Restoring Last Session");
        progress.setMaximum(length);
        progress.setValue(0);

        auto oldSize = size();
        setUpdatesEnabled(false);

        for (int i = 0; i < length; ++i)
        {
            if (progress.wasCanceled())
                break;
            auto status = MainWindow::EditorStatus(settingManager->getEditorStatus(i));
            progress.setValue(i);
            openTab("");
            currentWindow()->loadStatus(status);
            progress.setLabelText(currentWindow()->getTabTitle(true, false));
        }

        progress.setValue(length);

        setUpdatesEnabled(true);
        resize(oldSize);

        settingManager->clearEditorStatus();

        int index = settingManager->getCurrentIndex();
        if (index >= 0 && index < ui->tabWidget->count())
            ui->tabWidget->setCurrentIndex(index);
    }
}

AppWindow::AppWindow(int depth, bool cpp, bool java, bool python, bool noHotExit, const QStringList &paths,
                     QWidget *parent)
    : AppWindow(noHotExit, parent)
{
    openPaths(paths, cpp, java, python, depth);
    if (ui->tabWidget->count() == 0)
        openTab("");
}

AppWindow::AppWindow(bool cpp, bool java, bool python, bool noHotExit, int number, const QString &path, QWidget *parent)
    : AppWindow(noHotExit, parent)
{
    QString lang = settingManager->getDefaultLanguage();
    if (cpp)
        lang = "C++";
    else if (java)
        lang = "Java";
    else if (python)
        lang = "Python";
    openContest(path, lang, number);
    if (ui->tabWidget->count() == 0)
        openTab("");
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
    if (quit())
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
    auto urls = event->mimeData()->urls();
    QStringList paths;
    for (auto &e : urls)
        paths.append(e.toLocalFile());
    openPaths(paths);
}

/******************** PRIVATE METHODS ********************/
void AppWindow::setConnections()
{
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(onTabCloseRequested(int)));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(onTabChanged(int)));
    ui->tabWidget->tabBar()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tabWidget->tabBar(), SIGNAL(customContextMenuRequested(const QPoint &)), this,
            SLOT(onTabContextMenuRequested(const QPoint &)));
    connect(timer, SIGNAL(timeout()), this, SLOT(onSaveTimerElapsed()));

    connect(preferenceWindow, SIGNAL(settingsApplied()), this, SLOT(onSettingsApplied()));

    if (settingManager->isCompetitiveCompanionActive())
        companionEditorConnection =
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

bool AppWindow::closeTab(int index)
{
    auto tmp = windowIndex(index);
    if (tmp->closeConfirm())
    {
        ui->tabWidget->removeTab(index);
        onEditorChanged();
        delete tmp;
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

void AppWindow::openTab(QString path, bool iscompanionOpenedTab)
{
    if (!iscompanionOpenedTab)
    {
        if (QFile::exists(path))
        {
            auto fileInfo = QFileInfo(path);
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
        int index = 0;
        if (path.isEmpty())
        {
            QSet<int> vis;
            for (int t = 0; t < ui->tabWidget->count(); ++t)
            {
                auto tmp = windowIndex(t);
                if (tmp->isUntitled() && tmp->getProblemURL().isEmpty())
                {
                    vis.insert(tmp->getUntitledIndex());
                }
            }
            for (index = 1; vis.contains(index); ++index)
                ;
        }
        auto fsp = new MainWindow(path, settingManager, index);
        connect(fsp, SIGNAL(confirmTriggered(MainWindow *)), this, SLOT(on_confirmTriggered(MainWindow *)));
        connect(fsp, SIGNAL(editorChanged()), this, SLOT(onEditorChanged()));
        QString lang = settingManager->getDefaultLanguage();

        if (path.endsWith(".java"))
            lang = "Java";
        else if (path.endsWith(".py") || path.endsWith(".py3"))
            lang = "Python";
        else if (path.endsWith(".cpp") || path.endsWith(".cxx") || path.endsWith(".c") || path.endsWith(".cc") ||
                 path.endsWith(".hpp") || path.endsWith(".h"))
            lang = "C++";

        ui->tabWidget->addTab(fsp, fsp->getTabTitle(false, true));
        fsp->setLanguage(lang);
        ui->tabWidget->setCurrentIndex(t);
    }
    else
    {
        for (int t = 0; t < ui->tabWidget->count(); t++)
        {
            auto tmp = windowIndex(t);
            if (path == tmp->getProblemURL())
            {
                ui->tabWidget->setCurrentIndex(t);
                return;
            }
        }

        int t = ui->tabWidget->count();
        auto fsp = new MainWindow("", settingManager, 0);
        connect(fsp, SIGNAL(confirmTriggered(MainWindow *)), this, SLOT(on_confirmTriggered(MainWindow *)));
        connect(fsp, SIGNAL(editorChanged()), this, SLOT(onEditorChanged()));
        ui->tabWidget->addTab(fsp, fsp->getTabTitle(false, true));
        ui->tabWidget->setCurrentIndex(t);
    }
    currentWindow()->focusOnEditor();
}

void AppWindow::openTabs(const QStringList &paths)
{
    int length = paths.length();

    QProgressDialog progress(this);
    progress.setWindowModality(Qt::WindowModal);
    progress.setWindowTitle("Opening Files");
    progress.setMaximum(length);
    progress.setValue(0);

    auto oldSize = size();
    setUpdatesEnabled(false);

    for (int i = 0; i < length; ++i)
    {
        if (progress.wasCanceled())
            break;
        progress.setValue(i);
        openTab(paths[i]);
        progress.setLabelText(currentWindow()->getTabTitle(true, false));
    }

    setUpdatesEnabled(true);
    resize(oldSize);

    progress.setValue(length);
}

void AppWindow::openPaths(const QStringList &paths, bool cpp, bool java, bool python, int depth)
{
    QStringList res;
    for (auto &path : paths)
    {
        if (QDir(path).exists())
            res.append(openFolder(path, cpp, java, python, depth));
        else
            res.append(path);
    }
    openTabs(res);
}

QStringList AppWindow::openFolder(const QString &path, bool cpp, bool java, bool python, int depth)
{
    auto entries = QDir(path).entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);
    QStringList res;
    for (auto &entry : entries)
    {
        if (entry.isDir())
        {
            if (depth > 0)
                res.append(openFolder(entry.canonicalFilePath(), cpp, java, python, depth - 1));
            else if (depth == -1)
                res.append(openFolder(entry.canonicalFilePath(), cpp, java, python, -1));
        }
        else if (cpp && QStringList({"cpp", "hpp", "h", "cc", "cxx", "c"}).contains(entry.suffix()) ||
                 java && QStringList({"java"}).contains(entry.suffix()) ||
                 python && QStringList({"py", "py3"}).contains(entry.suffix()))
        {
            res.append(entry.canonicalFilePath());
        }
    }
    return res;
}

void AppWindow::openContest(const QString &path, const QString &lang, int number)
{
    QDir dir(path), parent(path);
    parent.cdUp();
    if (!dir.exists() && parent.exists())
        parent.mkdir(dir.dirName());

    auto language = lang.isEmpty() ? settingManager->getDefaultLanguage() : lang;

    QStringList tabs;

    for (int i = 0; i < number; ++i)
    {
        QString name('A' + i);
        if (language == "C++")
            name += ".cpp";
        else if (language == "Java")
            name += ".java";
        else if (language == "Python")
            name += ".py";
        tabs.append(QDir(path).filePath(name));
    }

    openTabs(tabs);
}

bool AppWindow::quit()
{
    settingManager->clearEditorStatus();
    if (settingManager->isUseHotExit())
    {
        if (ui->tabWidget->count() == 1 && windowIndex(0)->isUntitled() && !windowIndex(0)->isTextChanged())
        {
            settingManager->setNumberOfTabs(0);
            settingManager->setCurrentIndex(-1);
        }
        else
        {
            settingManager->setNumberOfTabs(ui->tabWidget->count());
            settingManager->setCurrentIndex(ui->tabWidget->currentIndex());
            for (int i = 0; i < ui->tabWidget->count(); ++i)
            {
                settingManager->setEditorStatus(i, windowIndex(i)->toStatus().toMap());
            }
        }
        return true;
    }
    else
    {
        on_actionClose_All_triggered();
        return ui->tabWidget->count() == 0;
    }
}

/***************** ABOUT SECTION ***************************/

void AppWindow::on_actionSupport_me_triggered()
{
    QDesktopServices::openUrl(QUrl("https://paypal.me/coder3101", QUrl::TolerantMode));
}

void AppWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "About CP Editor " APP_VERSION,
                       "<p><b>CP Editor</b> is a native Qt-based Code Editor. It's specially designed "
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
    if (quit())
        QApplication::exit();
}

void AppWindow::on_actionNew_Tab_triggered()
{
    openTab("");
}

void AppWindow::on_actionOpen_triggered()
{
    auto fileNames = QFileDialog::getOpenFileNames(this, tr("Open Files"), "",
                                                   "Source Files (*.cpp *.hpp *.h *.cc *.cxx *.c *.py *.py3 *.java)");
    openTabs(fileNames);
}

void AppWindow::on_actionOpenContest_triggered()
{
    auto path = QFileDialog::getExistingDirectory(this, "Open Contest");
    if (QFile::exists(path) && QFileInfo(path).isDir())
    {
        bool ok = false;
        int number =
            QInputDialog::getInt(this, "Open Contest", "Number of problems in this contest:", 5, 0, 26, 1, &ok);
        if (ok)
        {
            int current = 0;
            if (settingManager->getDefaultLanguage() == "Java")
                current = 1;
            else if (settingManager->getDefaultLanguage() == "Python")
                current = 2;
            auto lang = QInputDialog::getItem(this, "Open Contest", "Choose a language", {"C++", "Java", "Python"},
                                              current, false, &ok);
            if (ok)
            {
                openContest(path, lang, number);
            }
        }
    }
}

// ************************************* LOGS ARE TAKEN FOR CODE BELOW THIS *********************************

void AppWindow::on_actionSave_triggered()
{
    Core::Log::i("appwindow/on_actionSave", "Invoked");
    if (currentWindow() != nullptr)
        currentWindow()->save(true, "Save");
}

void AppWindow::on_actionSave_As_triggered()
{
    Core::Log::i("appwindow/on_actionSave_As", "Invoked");
    if (currentWindow() != nullptr)
        currentWindow()->saveAs();
}

void AppWindow::on_actionSave_All_triggered()
{
    Core::Log::i("appwindow/on_actionSave_All", "Invoked");
    for (int t = 0; t < ui->tabWidget->count(); ++t)
    {
        auto tmp = windowIndex(t);
        tmp->save(true, "Save All");
    }
}

void AppWindow::on_actionClose_Current_triggered()
{
    int index = ui->tabWidget->currentIndex();
    Core::Log::i("appwindow/on_actionClose_Current") << "Invoked with index : " << index << endl;
    if (index != -1)
        closeTab(index);
}

void AppWindow::on_actionClose_All_triggered()
{
    Core::Log::i("appwindow/on_actionClose_All", "Invoked");
    for (int t = 0; t < ui->tabWidget->count(); t++)
    {
        if (closeTab(t))
            --t;
        else
            break;
    }
}

void AppWindow::on_actionClose_Saved_triggered()
{
    Core::Log::i("appwindow/on_actionClose_Saved", "Invoked");
    for (int t = 0; t < ui->tabWidget->count(); t++)
        if (!windowIndex(t)->isTextChanged() && closeTab(t))
            --t;
}

/************************ PREFERENCES SECTION **********************/

void AppWindow::on_actionRestore_Settings_triggered()
{
    Core::Log::i("appwindow/on_actionRestore_Settings_triggered", "Invoked");
    auto res = QMessageBox::question(this, "Reset preferences?",
                                     "Are you sure you want to reset the"
                                     " all preferences to default?",
                                     QMessageBox::Yes | QMessageBox::No);
    if (res == QMessageBox::Yes)
    {
        settingManager->resetSettings();
        onSettingsApplied();
        Core::Log::i("appwindow/on_actionRestore_Settings_triggered", "Reset success");
    }
}

void AppWindow::on_actionSettings_triggered()
{
    Core::Log::i("appwindow/on_actionSettings_triggered", "Launching settings window");
    preferenceWindow->updateShow();
}

/************************** SLOTS *********************************/

#define FROMJSON(x) auto x = json[#x]

void AppWindow::onReceivedMessage(quint32 instanceId, QByteArray message)
{
    raise();
    Core::Log::i("appwindow/onReceivedMessage", "Recieved a request from secondary instances. Data is : " + message);

    message = message.mid(message.indexOf("NOLOSTDATA") + 10);
    auto json = QJsonDocument::fromBinaryData(message);
    FROMJSON(cpp).toBool();
    FROMJSON(java).toBool();
    FROMJSON(python).toBool();

    if (json["type"] == "normal")
    {
        Core::Log::i("appwindow/onReceivedMessage", "branched to normal");
        FROMJSON(depth).toInt();
        FROMJSON(paths).toVariant().toStringList();
        openPaths(paths);
    }
    else if (json["type"] == "contest")
    {
        Core::Log::i("appwindow/onReceivedMessage", "branched to contest");
        FROMJSON(number).toInt();
        FROMJSON(path).toString();
        QString lang = settingManager->getDefaultLanguage();
        if (cpp)
            lang = "C++";
        else if (java)
            lang = "Java";
        else if (python)
            lang = "Python";
        openContest(path, lang, number);
    }
    else
        Core::Log::w("appwindow/onReceivedMessage", "ignored");
}

#undef FROMJSON

void AppWindow::onTabCloseRequested(int index)
{
    Core::Log::i("appwindow/onTabCloseRequested") << "Closing tab at index : " << index << endl;
    closeTab(index);
}

void AppWindow::onTabChanged(int index)
{
    Core::Log::i("appwindow/onTabChanged") << "tab is being changed to " << index << endl;
    if (index == -1)
    {
        activeLogger = nullptr;
        server->setMessageLogger(nullptr);
        setWindowTitle("CP Editor: An editor specially designed for competitive programming");
        return;
    }

    disconnect(activeSplitterMoveConnection);
    disconnect(activeRightSplitterMoveConnection);

    auto tmp = windowIndex(index);

    setWindowTitle(tmp->getTabTitle(true, false) + " - CP Editor");

    activeLogger = tmp->getLogger();
    server->setMessageLogger(activeLogger);

    if (settingManager->isCompetitiveCompanionActive() && diagonistics)
        server->checkServer();

    tmp->applySettingsData(diagonistics);
    diagonistics = false;

    if (ui->actionEditor_Mode->isChecked())
        on_actionEditor_Mode_triggered();
    else if (ui->actionIO_Mode->isChecked())
        on_actionIO_Mode_triggered();
    else if (ui->actionSplit_Mode->isChecked())
        on_actionSplit_Mode_triggered();

    tmp->getRightSplitter()->restoreState(settingManager->getRightSplitterSizes());

    activeSplitterMoveConnection =
        connect(tmp->getSplitter(), SIGNAL(splitterMoved(int, int)), this, SLOT(onSplitterMoved(int, int)));
    activeRightSplitterMoveConnection =
        connect(tmp->getRightSplitter(), SIGNAL(splitterMoved(int, int)), this, SLOT(onRightSplitterMoved(int, int)));
}

void AppWindow::onEditorChanged()
{
    Core::Log::i("appwindow/onEditorChanged", "Invoked onEditorChanged");
    if (currentWindow() != nullptr)
    {
        QMap<QString, QVector<int>> tabsByName;

        for (int t = 0; t < ui->tabWidget->count(); ++t)
        {
            tabsByName[windowIndex(t)->getTabTitle(false, false)].push_back(t);
        }

        for (auto tabs : tabsByName)
        {
            for (auto index : tabs)
            {
                ui->tabWidget->setTabText(index, windowIndex(index)->getTabTitle(tabs.size() > 1, true));
            }
        }

        setWindowTitle(currentWindow()->getTabTitle(true, false) + " - CP Editor");
        Core::Log::i("appwindow/onEditorChanged",
                     "Changed window title to " + currentWindow()->getTabTitle(true, false) + " - CP Editor");
    }
}

void AppWindow::onSaveTimerElapsed()
{
    Core::Log::i("appwindow/onSaveTimerElapsed", "Autosave invoked");
    for (int t = 0; t < ui->tabWidget->count(); t++)
    {
        auto tmp = windowIndex(t);
        if (!tmp->isUntitled())
        {
            tmp->save(false, "Auto Save");
            Core::Log::i("appwindow/onSaveTimerElapsed", "Autosave success");
        }
    }
}

void AppWindow::onSettingsApplied()
{
    Core::Log::i("appwindow/onSettingsApplied", "Applying settings to appwindow");

    updater->setBeta(settingManager->isBeta());
    maybeSetHotkeys();

    disconnect(companionEditorConnection);

    server->updatePort(settingManager->getConnectionPort());

    if (settingManager->isCompetitiveCompanionActive())
        companionEditorConnection =
            connect(server, &Network::CompanionServer::onRequestArrived, this, &AppWindow::onIncomingCompanionRequest);
    diagonistics = true;
    onTabChanged(ui->tabWidget->currentIndex());
    onEditorChanged();

    Core::Log::i("appwindow/onSettingsApplied", "Applied settings to appwindow");
}

void AppWindow::onIncomingCompanionRequest(Network::CompanionData data)
{
    Core::Log::i("appwindow/onIncomingCompanionRequest")
        << "Applying data to new tab. Args: shouldOpenNewTab:" << settingManager->isCompetitiveCompanionOpenNewTab()
        << ", currentWindow == nullptr:" << (currentWindow() == nullptr) << endl;

    if (settingManager->isCompetitiveCompanionOpenNewTab() || currentWindow() == nullptr)
    {
        openTab(data.url, true);
    }
    currentWindow()->applyCompanion(data);
}

void AppWindow::onViewModeToggle()
{
    Core::Log::i("appwindow/onViewModeToggle", "Switching view mode");
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
    Core::Log::i("appwindow/onSplitterMoved", "updating state");
    auto splitter = currentWindow()->getSplitter();
    settingManager->setSplitterSizes(splitter->saveState());
}

void AppWindow::onRightSplitterMoved(int _, int __)
{
    Core::Log::i("appwindow/onRightSplitterMoved", "updating state");
    auto splitter = currentWindow()->getRightSplitter();
    settingManager->setRightSplitterSizes(splitter->saveState());
}

/************************* ACTIONS ************************/
void AppWindow::on_actionCheck_for_updates_triggered()
{
    Core::Log::i("appwindow/on_actionCheck_for_updates_triggered", "Checking update non-silent mode");
    // Non-silent means if a update is not available, still the dialog is shown that no update available.
    updater->checkUpdate(true);
}

void AppWindow::on_actionCompile_triggered()
{
    if (currentWindow() != nullptr)
    {
        if (ui->actionEditor_Mode->isChecked())
            on_actionSplit_Mode_triggered();
        currentWindow()->compileOnly();
        Core::Log::i("appwindow/on_actionCompile_Run_triggered", "Invoked compile for current Window");
    }
    else
    {
        Core::Log::w("appwindow/on_actionCompile_Run_triggered", "Nothing happened, No active window");
    }
}

void AppWindow::on_actionCompile_Run_triggered()
{
    if (currentWindow() != nullptr)
    {
        if (ui->actionEditor_Mode->isChecked())
            on_actionSplit_Mode_triggered();
        currentWindow()->compileAndRun();
        Core::Log::i("appwindow/on_actionCompile_Run_triggered", "Invoked compile run for current Window");
    }
    else
    {
        Core::Log::w("appwindow/on_actionCompile_Run_triggered", "Nothing happened, No active window");
    }
}

void AppWindow::on_actionRun_triggered()
{
    if (currentWindow() != nullptr)
    {
        if (ui->actionEditor_Mode->isChecked())
            on_actionSplit_Mode_triggered();
        currentWindow()->runOnly();
        Core::Log::i("appwindow/on_actionRun_triggered", "Invoked Run only for current window");
    }
    else
    {
        Core::Log::w("appwindow/on_actionRun_triggered", "Nothing happened, No active window");
    }
}

void AppWindow::on_actionFormat_code_triggered()
{
    if (currentWindow() != nullptr)
    {
        currentWindow()->formatSource();
        Core::Log::i("appwindow/on_actionFormat_code_triggered", "Invoked on currentWindow");
    }
    else
    {
        Core::Log::w("appwindow/on_actionFormat_code_triggered", "Nothing happened, No active window");
    }
}

void AppWindow::on_actionRun_Detached_triggered()
{
    if (currentWindow() != nullptr)
    {
        currentWindow()->detachedExecution();
        Core::Log::i("appwindow/on_actionRun_Detached_triggered", "Invoked on currentWindow");
    }
    else
    {
        Core::Log::w("appwindow/on_actionRun_Detached_triggered", "Nothing happened, No active window");
    }
}

void AppWindow::on_actionKill_Processes_triggered()
{
    if (currentWindow() != nullptr)
    {
        currentWindow()->killProcesses();
        Core::Log::i("appwindow/on_actionKill_Processes_triggered", "Invoked on currentWindow");
    }
    else
    {
        Core::Log::w("appwindow/on_actionKill_Processes_triggered", "Nothing happened, No active window");
    }
}

void AppWindow::on_actionUse_Snippets_triggered()
{
    Core::Log::i("appwindow/on_actionUse_Snippets_triggered", "Use snipped trigerred");
    auto current = currentWindow();
    if (current != nullptr)
    {
        auto lang = current->getLanguage();
        auto names = settingManager->getSnippetsNames(lang);
        Core::Log::i("appwindow/on_actionUse_Snippets_triggered", "Lang : " + lang + "name : " + names.join(","));
        if (names.isEmpty())
        {
            Core::Log::w("appwindow/on_actionUse_Snippets_triggered", "No snippets exists");
            activeLogger->warn("Snippets",
                               "There are no snippets for " + lang + ". Please add snippets in the preference window.");
        }
        else
        {
            auto ok = new bool;
            auto name = QInputDialog::getItem(this, tr("Use Snippets"), tr("Choose a snippet:"), names, 0, true, ok);
            if (*ok)
            {
                Core::Log::i("appwindow/on_actionUse_Snippets_triggered", "Looking for snippet : " + name);
                if (names.contains(name))
                {
                    Core::Log::i("appwindow/on_actionUse_Snippets_triggered", "Found snippet and inserted");
                    auto content = settingManager->getSnippet(lang, name);
                    current->insertText(content);
                }
                else
                {
                    Core::Log::w("appwindow/on_actionUse_Snippets_triggered", "No snippet for query");
                    activeLogger->warn("Snippets", "There is no snippet named " + name + " for " + lang);
                }
            }
            delete ok;
        }
    }
    else
    {
        Core::Log::w("appwindow/on_actionUse_Snippets_triggered", "No window. Skipped");
    }
}

void AppWindow::on_actionEditor_Mode_triggered()
{
    settingManager->setViewMode(Settings::ViewMode::FULL_EDITOR);
    ui->actionEditor_Mode->setChecked(true);
    ui->actionIO_Mode->setChecked(false);
    ui->actionSplit_Mode->setChecked(false);
    if (currentWindow() != nullptr)
    {
        Core::Log::i("appwindow/on_actionEditor_Mode_triggered", "Switched to editor only mode");
        currentWindow()->getSplitter()->setSizes({1, 0});
    }
    else
    {
        Core::Log::w("appwindow/on_actionEditor_Mode_triggered", "currentWindow is null. No UI changed");
    }
}

void AppWindow::on_actionIO_Mode_triggered()
{
    settingManager->setViewMode(Settings::ViewMode::FULL_IO);
    ui->actionEditor_Mode->setChecked(false);
    ui->actionIO_Mode->setChecked(true);
    ui->actionSplit_Mode->setChecked(false);
    if (currentWindow() != nullptr)
    {
        Core::Log::w("appwindow/on_actionIO_Mode_triggered", "Switched to IO Mode");
        currentWindow()->getSplitter()->setSizes({0, 1});
    }
    else
    {
        Core::Log::w("appwindow/on_actionIO_Mode_triggered", "currentWindow is null. No UI changed");
    }
}

void AppWindow::on_actionSplit_Mode_triggered()
{
    settingManager->setViewMode(Settings::ViewMode::SPLIT);
    ui->actionEditor_Mode->setChecked(false);
    ui->actionIO_Mode->setChecked(false);
    ui->actionSplit_Mode->setChecked(true);
    auto state = settingManager->getSplitterSizes();
    Core::Log::i("appwindow/on_actionSplit_Mode_triggered", "Entered split mode");
    if (currentWindow() != nullptr)
    {
        Core::Log::i("appwindow/on_actionSplit_Mode_triggered", "Restored splitter state");
        currentWindow()->getSplitter()->restoreState(state);
    }
    else
        Core::Log::w("appwindow/on_actionSplit_Mode_triggered", "No UI change required");
}

void AppWindow::on_action_indent_triggered()
{
    auto tmp = currentWindow();
    if (tmp != nullptr)
        tmp->getEditor()->indent();
    else
        Core::Log::w("appwindow/on_action_indent_triggered", "skipped action because no active tab exists");
}

void AppWindow::on_action_unindent_triggered()
{
    auto tmp = currentWindow();
    if (tmp != nullptr)
        tmp->getEditor()->unindent();
    else
        Core::Log::w("appwindow/on_action_unindent_triggered", "skipped action because no active tab exists");
}

void AppWindow::on_action_swap_line_up_triggered()
{
    auto tmp = currentWindow();
    if (tmp != nullptr)
        tmp->getEditor()->swapLineUp();
    else
        Core::Log::w("appwindow/on_action_swap_line_up_triggered", "skipped action because no active tab exists");
}

void AppWindow::on_action_swap_line_down_triggered()
{
    auto tmp = currentWindow();
    if (tmp != nullptr)
        tmp->getEditor()->swapLineDown();
    else
        Core::Log::w("appwindow/on_action_swap_line_down_triggered", "skipped action because no active tab exists");
}

void AppWindow::on_action_delete_line_triggered()
{
    auto tmp = currentWindow();
    if (tmp != nullptr)
        tmp->getEditor()->deleteLine();
    else
        Core::Log::w("appwindow/on_action_delete_line_triggered", "skipped action because no active tab exists");
}

void AppWindow::on_action_toggle_comment_triggered()
{
    auto tmp = currentWindow();
    if (tmp != nullptr)
        tmp->getEditor()->toggleComment();
    else
        Core::Log::w("appwindow/on_action_toggle_comment_triggered", "skipped action because no active tab exists");
}

void AppWindow::on_action_toggle_block_comment_triggered()
{
    auto tmp = currentWindow();
    if (tmp != nullptr)
        tmp->getEditor()->toggleBlockComment();
    else
        Core::Log::w("appwindow/on_action_toggle_block_comment_triggered",
                     "skipped action because no active tab exists");
}

void AppWindow::on_confirmTriggered(MainWindow *widget)
{
    int index = ui->tabWidget->indexOf(widget);
    if (index != -1)
        ui->tabWidget->setCurrentIndex(index);
    else
        Core::Log::w("appwindow/on_confirmTriggered", "index of widget returned nullptr");
}

void AppWindow::onTabContextMenuRequested(const QPoint &pos)
{
    Core::Log::i("appwindow/onTabContextMenuRequested") << "Location: (" << pos.x() << ", " << pos.y() << ")" << endl;

    int index = ui->tabWidget->tabBar()->tabAt(pos);
    if (index != -1)
    {
        Core::Log::i("appwindow/onTabContextMenuRequested") << "Tab index is : " << index;

        auto widget = windowIndex(index);
        auto menu = new QMenu();

        menu->addAction("Close", [index, this] { closeTab(index); });

        menu->addAction("Close Others", [widget, this] {
            for (int i = 0; i < ui->tabWidget->count(); ++i)
                if (windowIndex(i) != widget && closeTab(i))
                    --i;
        });

        menu->addAction("Close to the Left", [widget, this] {
            for (int i = 0; i < ui->tabWidget->count() && windowIndex(i) != widget; ++i)
                if (closeTab(i))
                    --i;
        });

        menu->addAction("Close to the Right", [index, this] {
            for (int i = index + 1; i < ui->tabWidget->count(); ++i)
                if (closeTab(i))
                    --i;
        });
        menu->addAction("Close Saved", [this] { on_actionClose_Saved_triggered(); });

        menu->addAction("Close All", [this] { on_actionClose_All_triggered(); });
        QString filePath = widget->getFilePath();

        Core::Log::i("appwindow/onTabContextMenuRequested", "Filepath is : " + filePath);

        if (!widget->isUntitled() && QFile::exists(filePath))
        {
            Core::Log::i("appwindow/onTabContextMenuRequested", "Not untitled and filepath exists in system");
            menu->addSeparator();
            menu->addAction("Copy File Path", [filePath] { QGuiApplication::clipboard()->setText(filePath); });
            // Reference: http://lynxline.com/show-in-finder-show-in-explorer/ and https://forum.qt.io/post/296072
#if defined(Q_OS_MACOS)
            Core::Log::i("appwindow/onTabContextMenuRequested", "Adding menu reveal in finder");
            menu->addAction("Reveal in Finder", [filePath] {
                QStringList args;
                args << "-e";
                args << "tell application \"Finder\"";
                args << "-e";
                args << "activate";
                args << "-e";
                args << "select POSIX file \"" + filePath + "\"";
                args << "-e";
                args << "end tell";
                QProcess::startDetached("osascript", args);
            });
#elif defined(Q_OS_WIN)
            Core::Log::i("appwindow/onTabContextMenuRequested", "Adding menu reveal in explorer");
            menu->addAction("Reveal in Explorer", [filePath] {
                QStringList args;
                args << "/select," << QDir::toNativeSeparators(filePath);
                QProcess::startDetached("explorer", args);
            });
#elif defined(Q_OS_UNIX)
            Core::Log::i("appwindow/onTabContextMenuRequested", "Adding menu reveal in linux");
            QProcess proc;
            proc.start("xdg-mime", QStringList() << "query"
                                                 << "default"
                                                 << "inode/directory");
            auto finished = proc.waitForFinished(2000);
            if (finished)
            {
                Core::Log::i("appwindow/onTabContextMenuRequested/xdg-mime", "Process finished");
                auto output = proc.readLine().simplified();
                Core::Log::i("appwindow/onTabContextMenuRequested/xdg-mime/output", output);
                QString program;
                QStringList args;
                auto nativePath = QUrl::fromLocalFile(filePath).toString();
                if (output == "dolphin.desktop" || output == "org.kde.dolphin.desktop")
                {
                    program = "dolphin";
                    args << "--select" << nativePath;
                }
                else if (output == "nautilus.desktop" || output == "org.gnome.Nautilus.desktop" ||
                         output == "nautilus-folder-handler.desktop")
                {
                    program = "nautilus";
                    args << "--no-desktop" << nativePath;
                }
                else if (output == "caja-folder-handler.desktop")
                {
                    program = "caja";
                    args << "--no-desktop" << nativePath;
                }
                else if (output == "nemo.desktop")
                {
                    program = "nemo";
                    args << "--no-desktop" << nativePath;
                }
                else if (output == "kfmclient_dir.desktop")
                {
                    program = "konqueror";
                    args << "--select" << nativePath;
                }
                if (program.isEmpty())
                {
                    menu->addAction("Open Containing Folder", [filePath] {
                        QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(filePath).path()));
                    });
                }
                else
                {
                    menu->addAction("Reveal in File Manager", [program, args] {
                        QProcess openProcess;
                        openProcess.startDetached(program, args);
                    });
                }
            }
            else
            {
                Core::Log::i("appwindow/onTabContextMenuRequested/xdg-mime",
                             "Process cannot finish. So opting for openFolder by using QDesktopServices");

                menu->addAction("Open Containing Folder", [filePath] {
                    QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(filePath).path()));
                });
            }
#else
            Core::Log::w("appwindow/onTabContextMenuRequested", "Unknown OS. Fallback to open via QDesktopServices");
            menu->addAction("Open Containing Folder",
                            [filePath] { QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(filePath).path())); });
#endif
        }
        else if (!widget->isUntitled() && QFile::exists(QFileInfo(widget->getFilePath()).path()))
        {
            Core::Log::i("appwindow/onTabContextMenuRequested",
                         "filepath does not exists. Looking if tab provides filepath " +
                             QFileInfo(widget->getFilePath()).path());
            menu->addSeparator();
            menu->addAction("Copy path", [filePath] {
                auto clipboard = QGuiApplication::clipboard();
                clipboard->setText(filePath);
            });
            menu->addAction("Open Containing Folder",
                            [filePath] { QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(filePath).path())); });
        }
        menu->addSeparator();
        if (!widget->getProblemURL().isEmpty())
        {
            menu->addAction("Copy Problem URL",
                            [widget] { QGuiApplication::clipboard()->setText(widget->getProblemURL()); });
        }
        menu->addAction("Set Problem URL", [widget, this] {
            bool ok = false;
            auto url =
                QInputDialog::getText(this, "Set Problem URL", "Enter the new problem URL here:", QLineEdit::Normal,
                                      widget->getProblemURL().isEmpty() ? "https://codeforces.com/contest//problem/"
                                                                        : widget->getProblemURL(),
                                      &ok);
            if (ok)
                widget->setProblemURL(url);
            else
                Core::Log::i("appwindow/onTabContextMenuRequested", "set problem url dialog closed or cancelled");
        });
        menu->popup(ui->tabWidget->tabBar()->mapToGlobal(pos));
    }
}

MainWindow *AppWindow::currentWindow()
{
    int current = ui->tabWidget->currentIndex();
    if (current == -1)
    {
        Core::Log::w("appwindow/currentWindow", "No active window, returning nullptr");
        return nullptr;
    }
    return dynamic_cast<MainWindow *>(ui->tabWidget->widget(current));
}

MainWindow *AppWindow::windowIndex(int index)
{
    if (index == -1)
    {
        Core::Log::w("appwindow/windowIndex", "No active window(-1), returning nullptr");
        return nullptr;
    }
    return dynamic_cast<MainWindow *>(ui->tabWidget->widget(index));
}

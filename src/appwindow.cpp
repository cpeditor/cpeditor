/*
 * Copyright (C) 2019-2020 Ashar Khan <ashar786khan@gmail.com>
 *
 * This file is part of CP Editor.
 *
 * CP Editor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * I will not be responsible if CP Editor behaves in unexpected way and
 * causes your ratings to go down and or lose any important contest.
 *
 * Believe Software is "Software" and it isn't immune to bugs.
 *
 */

#include "appwindow.hpp"
#include "../ui/ui_appwindow.h"
#include "Core/EventLogger.hpp"
#include "Extensions/EditorTheme.hpp"
#include "Util.hpp"
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
#include <QSettings>
#include <QStyleFactory>
#include <QTimer>
#include <QUrl>
#include <generated/SettingsHelper.hpp>

AppWindow::AppWindow(bool noHotExit, QWidget *parent) : QMainWindow(parent), ui(new Ui::AppWindow)
{
    Core::Log::i("appwindow/constructed") << "noHotExit " << noHotExit << endl;
    ui->setupUi(this);
    setAcceptDrops(true);
    allocate();
    setConnections();

#ifdef Q_OS_WIN
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
                       QSettings::NativeFormat);
    if (settings.value("AppsUseLightTheme") == 0)
    {
        Core::Log::i("main/Normal", "Using dark pallete for the theme as Windows Settings");
        qApp->setStyle(QStyleFactory::create("Fusion"));
        qApp->setPalette(Util::windowsDarkThemePalette());
        qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");
    }
#endif

    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    setWindowIcon(QIcon(":/icon.png"));

    if (SettingsHelper::isCheckUpdate())
        updater->checkUpdate();

#ifdef Q_OS_WIN
    // setWindowOpacity(0.99) when transparency should be 100 is a workaround for a strange issue on Windows
    // The behavior: If the transparency is 100, and the window is maximized, it will resize to smaller than maximized
    if (SettingsHelper::getTransparency() < 100)
        setWindowOpacity(SettingsHelper::getTransparency() / 100.0);
    else
        setWindowOpacity(0.99);
#else
    setWindowOpacity(SettingsHelper::getTransparency() / 100.0);
#endif

    applySettings();
    onSettingsApplied("");

    if (!noHotExit && SettingsHelper::isHotExitEnable())
    {
        int length = SettingsHelper::getHotExitTabCount();

        QProgressDialog progress(this);
        progress.setWindowModality(Qt::WindowModal);
        progress.setWindowTitle("Restoring Last Session");
        progress.setMaximum(length);
        progress.setValue(0);

        auto oldSize = size();
        setUpdatesEnabled(false);

        // save these so that they won't be affected by saveEditorStatus() in onEditorFileChanged()
        QVector<MainWindow::EditorStatus> status;
        for (int i = 0; i < length; ++i)
            status.push_back(
                MainWindow::EditorStatus(SettingsManager::get(QString("Editor Status/%1").arg(i)).toMap()));
        bool loadFromFile = SettingsHelper::isHotExitLoadFromFile();
        int currentIndex = SettingsHelper::getHotExitCurrentIndex();

        for (int i = 0; i < length; ++i)
        {
            if (progress.wasCanceled())
                break;
            progress.setValue(i);
            openTab("");
            currentWindow()->loadStatus(status[i], loadFromFile);
            progress.setLabelText(currentWindow()->getTabTitle(true, false));
        }

        progress.setValue(length);

        setUpdatesEnabled(true);
        resize(oldSize);

        SettingsHelper::setHotExitLoadFromFile(true);

        if (currentIndex >= 0 && currentIndex < ui->tabWidget->count())
            ui->tabWidget->setCurrentIndex(currentIndex);
    }
}

AppWindow::AppWindow(int depth, bool cpp, bool java, bool python, bool noHotExit, const QStringList &paths,
                     QWidget *parent)
    : AppWindow(noHotExit, parent)
{
    Core::Log::i("appwindow/constructed") << "args : "
                                          << "depth : " << depth << "cpp: " << cpp << "java: " << java << "python "
                                          << python << "noHotExit " << noHotExit << "paths " << paths.join(" ") << endl;
    openPaths(paths, cpp, java, python, depth);
    if (ui->tabWidget->count() == 0)
        openTab("");

#ifdef Q_OS_WIN
    // This is necessary because of setWindowOpacity(0.99) earlier
    if (SettingsHelper::getTransparency() == 100)
        setWindowOpacity(1);
#endif
}

AppWindow::AppWindow(bool cpp, bool java, bool python, bool noHotExit, int number, const QString &path, QWidget *parent)
    : AppWindow(noHotExit, parent)
{
    Core::Log::i("appwindow/constructed") << "args : "
                                          << "cpp: " << cpp << "java: " << java << "python " << python << "noHotExit "
                                          << noHotExit << "paths " << path << endl;
    QString lang = SettingsHelper::getDefaultLanguage();
    if (cpp)
        lang = "C++";
    else if (java)
        lang = "Java";
    else if (python)
        lang = "Python";
    openContest(path, lang, number);
    if (ui->tabWidget->count() == 0)
        openTab("");

#ifdef Q_OS_WIN
    // This is necessary because of setWindowOpacity(0.99) earlier
    if (SettingsHelper::getTransparency() == 100)
        setWindowOpacity(1);
#endif
    setLanguageClient();
}

AppWindow::~AppWindow()
{
    Core::Log::i("appwindow/destroyed", "Invoked");
    saveSettings();
    if (languageClient != nullptr)
    {
        languageClient->shutdown();
        languageClient->exit();
        delete languageClient;
    }
    Extensions::EditorTheme::release();
    delete ui;
    delete preferencesWindow;
    delete autoSaveTimer;
    delete updater;
    delete server;
    delete findReplaceDialog;
    SettingsManager::deinit();
}

/******************* PUBLIC METHODS ***********************/

void AppWindow::setLanguageClient()
{
    // Let us check here if user wants to use LSP. From settings
    // otherwise return;

    Core::Log::i("appwindow/setLanguageClient", "Invoked");
    // Make this path value get picked from the SettingManager
    // Also Make the Command line arguments for LSP picked from Settings

    languageClient = new LSPClient("C:/Program Files/LLVM/bin/clangd.exe", {/* No commandline args */});

    connect(languageClient, SIGNAL(onNotify(QString, QJsonObject)), this,
            SLOT(onLSPServerNotificationArrived(QString, QJsonObject)));
    connect(languageClient, SIGNAL(onResponse(QJsonObject, QJsonObject)), this,
            SLOT(onLSPServerResponseArrived(QJsonObject, QJsonObject)));
    connect(languageClient, SIGNAL(onRequest(QString, QJsonObject, QJsonObject)), this,
            SLOT(onLSPServerRequestArrived(QString, QJsonObject, QJsonObject)));
    connect(languageClient, SIGNAL(onError(QJsonObject, QJsonObject)), this,
            SLOT(onLSPServerErrorArrived(QJsonObject, QJsonObject)));

    connect(languageClient, SIGNAL(onServerError(QProcess::ProcessError)), this,
            SLOT(onLSPServerProcessError(QProcess::ProcessError)));
    connect(languageClient, SIGNAL(onServerFinished(int, QProcess::ExitStatus)), this,
            SLOT(onLSPServerProcessFinished(int, QProcess::ExitStatus)));

    languageClient->initialize();
}

void AppWindow::closeEvent(QCloseEvent *event)
{
    Core::Log::i("appwindow/closeEvent", "Invoked");
    if (quit())
        event->accept();
    else
        event->ignore();
}

void AppWindow::dragEnterEvent(QDragEnterEvent *event)
{
    Core::Log::i("appwindow/dragEnterEvent", "Invoked");
    if (event->mimeData()->hasUrls())
    {
        Core::Log::i("appwindow/dragEnterEvent", "Accepted the dropped value");
        event->acceptProposedAction();
    }
}

void AppWindow::dropEvent(QDropEvent *event)
{
    Core::Log::i("appwindow/dropEvent", "Invoked");
    auto urls = event->mimeData()->urls();
    QStringList paths;
    for (auto &e : urls)
        paths.append(e.toLocalFile());
    openPaths(paths);
}

/******************** PRIVATE METHODS ********************/
void AppWindow::setConnections()
{
    Core::Log::i("appwindow/setConnections", "Invoked");
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(onTabCloseRequested(int)));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(onTabChanged(int)));
    ui->tabWidget->tabBar()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tabWidget->tabBar(), SIGNAL(customContextMenuRequested(const QPoint &)), this,
            SLOT(onTabContextMenuRequested(const QPoint &)));
    connect(autoSaveTimer, SIGNAL(timeout()), this, SLOT(onSaveTimerElapsed()));

    connect(preferencesWindow, SIGNAL(settingsApplied(const QString &)), this,
            SLOT(onSettingsApplied(const QString &)));

    connect(server, &Extensions::CompanionServer::onRequestArrived, this, &AppWindow::onIncomingCompanionRequest);

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
            SLOT(onTrayIconActivated(QSystemTrayIcon::ActivationReason)));
    connect(trayIcon, SIGNAL(messageClicked()), this, SLOT(showOnTop()));
}

void AppWindow::allocate()
{
    Core::Log::i("appwindow/allocate", "Invoked");
    SettingsManager::init();
    autoSaveTimer = new QTimer();
    updater = new Telemetry::UpdateNotifier(SettingsHelper::isBeta());
    preferencesWindow = new PreferencesWindow(this);
    server = new Extensions::CompanionServer(SettingsHelper::getCompetitiveCompanionConnectionPort());
    findReplaceDialog = new FindReplaceDialog(this);
    findReplaceDialog->setModal(false);
    findReplaceDialog->setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint |
                                      Qt::WindowCloseButtonHint);

    autoSaveTimer->setInterval(3000);
    autoSaveTimer->setSingleShot(false);

    trayIconMenu = new QMenu();
    trayIconMenu->addAction("Show Main Window", this, SLOT(showOnTop()));
    trayIconMenu->addAction("About", this, SLOT(on_actionAbout_triggered()));
    trayIconMenu->addAction("Quit", this, SLOT(on_actionQuit_triggered()));
    trayIcon = new QSystemTrayIcon();
    trayIcon->setIcon(QIcon(":/icon.png"));
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->show();
}

void AppWindow::applySettings()
{
    Core::Log::i("appwindow/applySettings", "Invoked");
    QString mode = SettingsHelper::getViewMode();

    if (mode == "code")
        on_actionEditor_Mode_triggered();
    else if (mode == "io")
        on_actionIO_Mode_triggered();
    else
        on_actionSplit_Mode_triggered();

    if (!SettingsHelper::getGeometry().isEmpty() && !SettingsHelper::getGeometry().isNull() &&
        SettingsHelper::getGeometry().isValid() && !SettingsHelper::isMaximizedWindow())
    {
        setGeometry(SettingsHelper::getGeometry());
    }

    if (SettingsHelper::isMaximizedWindow())
    {
        this->showMaximized();
    }

    maybeSetHotkeys();

    // FindReplaceDialog->readSettings(*SettingsHelper::settings()); FIX IT!!!
}

void AppWindow::maybeSetHotkeys()
{
    Core::Log::i("appwindow/maybeSetHotkeys", "Invoked");
    for (auto e : hotkeyObjects)
        delete e;
    hotkeyObjects.clear();

    if (!SettingsHelper::getHotkeyRun().isEmpty())
    {
        hotkeyObjects.push_back(new QShortcut(SettingsHelper::getHotkeyRun(), this, SLOT(on_actionRun_triggered())));
    }
    if (!SettingsHelper::getHotkeyCompile().isEmpty())
    {
        hotkeyObjects.push_back(
            new QShortcut(SettingsHelper::getHotkeyCompile(), this, SLOT(on_actionCompile_triggered())));
    }
    if (!SettingsHelper::getHotkeyCompileRun().isEmpty())
    {
        hotkeyObjects.push_back(
            new QShortcut(SettingsHelper::getHotkeyCompileRun(), this, SLOT(on_actionCompile_Run_triggered())));
    }
    if (!SettingsHelper::getHotkeyFormat().isEmpty())
    {
        hotkeyObjects.push_back(
            new QShortcut(SettingsHelper::getHotkeyFormat(), this, SLOT(on_actionFormat_code_triggered())));
    }
    if (!SettingsHelper::getHotkeyKill().isEmpty())
    {
        hotkeyObjects.push_back(
            new QShortcut(SettingsHelper::getHotkeyKill(), this, SLOT(on_actionKill_Processes_triggered())));
    }
    if (!SettingsHelper::getHotkeyChangeViewMode().isEmpty())
    {
        hotkeyObjects.push_back(
            new QShortcut(SettingsHelper::getHotkeyChangeViewMode(), this, SLOT(onViewModeToggle())));
    }
    if (!SettingsHelper::getHotkeySnippets().isEmpty())
    {
        hotkeyObjects.push_back(
            new QShortcut(SettingsHelper::getHotkeySnippets(), this, SLOT(on_actionUse_Snippets_triggered())));
    }
}

bool AppWindow::closeTab(int index)
{
    Core::Log::i("appwindowCloseTab") << "index : " << index << endl;
    auto tmp = windowAt(index);
    if (tmp->closeConfirm())
    {
        ui->tabWidget->removeTab(index);
        onEditorFileChanged();
        delete tmp;
        return true;
    }
    return false;
}

void AppWindow::saveSettings()
{
    Core::Log::i("appwindow/saveSettings", "Invoked");
    if (!this->isMaximized())
        SettingsHelper::setGeometry(this->geometry());
    SettingsHelper::setMaximizedWindow(this->isMaximized());
    // findReplaceDialog->writeSettings(*SettingsHelper::settings()); FIX IT!!!
}

void AppWindow::openTab(const QString &path)
{
    Core::Log::i("appwindow/openTab") << "path " << path << endl;
    if (QFile::exists(path))
    {
        Core::Log::i("appwindow/openTab", "branched to exists file");
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

    auto fsp = new MainWindow(path, getNewUntitledIndex(), this);
    connect(fsp, SIGNAL(confirmTriggered(MainWindow *)), this, SLOT(on_confirmTriggered(MainWindow *)));
    connect(fsp, SIGNAL(editorFileChanged()), this, SLOT(onEditorFileChanged()));
    connect(fsp, SIGNAL(editorTextChanged(MainWindow *)), this, SLOT(onEditorTextChanged(MainWindow *)));
    connect(fsp, SIGNAL(requestToastMessage(const QString &, const QString &)), trayIcon,
            SLOT(showMessage(const QString &, const QString &)));

    QString lang = SettingsHelper::getDefaultLanguage();

    auto suffix = QFileInfo(path).suffix();

    if (Util::cppSuffix.contains(suffix))
        lang = "C++";
    else if (Util::javaSuffix.contains(suffix))
        lang = "Java";
    else if (Util::pythonSuffix.contains(suffix))
        lang = "Python";

    ui->tabWidget->setCurrentIndex(ui->tabWidget->addTab(fsp, fsp->getTabTitle(false, true)));
    fsp->setLanguage(lang);

    currentWindow()->getEditor()->setFocus();
    onEditorFileChanged();
}

void AppWindow::openTabs(const QStringList &paths)
{
    Core::Log::i("appwindow/openTab") << "paths : " << paths.join(", ") << endl;
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
    Core::Log::i("appwindow/openPaths") << "args are " << paths.join(", ") << " cpp:" << cpp << " java:" << java
                                        << "python:" << python << "depth:" << depth << endl;
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
    Core::Log::i("appwindow/openFolder") << "args are " << path << " cpp:" << cpp << " java:" << java
                                         << "python:" << python << "depth:" << depth << endl;
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
        else if ((cpp && Util::cppSuffix.contains(entry.suffix())) ||
                 (java && Util::javaSuffix.contains(entry.suffix())) ||
                 (python && Util::pythonSuffix.contains(entry.suffix())))
        {
            res.append(entry.canonicalFilePath());
        }
    }
    return res;
}

void AppWindow::openContest(const QString &path, const QString &lang, int number)
{
    Core::Log::i("appwindow/openContest", "Invoked");
    Core::Log::i("appwindoww/openContest") << "args are : " << path << " " << lang << " " << number << endl;
    QDir dir(path), parent(path);
    parent.cdUp();
    if (!dir.exists() && parent.exists())
        parent.mkdir(dir.dirName());

    auto language = lang.isEmpty() ? SettingsHelper::getDefaultLanguage() : lang;

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

void AppWindow::saveEditorStatus(bool loadFromFile)
{
    Core::Log::i("appwindow/saveEditorStatus") << "loadFromFile " << loadFromFile << endl;
    SettingsManager::remove(SettingsManager::keyStartsWith("Editor Status/"));
    if (ui->tabWidget->count() == 1 && windowAt(0)->isUntitled() && !windowAt(0)->isTextChanged() &&
        windowAt(0)->getProblemURL().isEmpty())
    {
        Core::Log::i("appwindow/saveEditorStatus", "branched to if");
        SettingsHelper::setHotExitTabCount(0);
        SettingsHelper::setHotExitCurrentIndex(-1);
    }
    else
    {
        Core::Log::i("appwindow/saveEditorStatus", "branched to else");
        SettingsHelper::setHotExitTabCount(ui->tabWidget->count());
        SettingsHelper::setHotExitCurrentIndex(ui->tabWidget->currentIndex());
        for (int i = 0; i < ui->tabWidget->count(); ++i)
            SettingsManager::set(QString("Editor Status/%1").arg(i), windowAt(i)->toStatus(loadFromFile).toMap());
    }
}

bool AppWindow::quit()
{
    bool ret = false;
    if (SettingsHelper::isHotExitEnable())
    {
        Core::Log::i("appwindow/quit", "Using hotexit");
        SettingsHelper::setHotExitLoadFromFile(false);
        saveEditorStatus(false);
        ret = true;
    }
    else
    {
        Core::Log::i("appwindow/quit", "closeAll() called without hotExit");
        on_actionClose_All_triggered();
        ret = ui->tabWidget->count() == 0;
    }
    if (ret && preferencesWindow->isVisible())
        ret = preferencesWindow->close();
    // The tray icon is considered as a visible window, if it is not hidden, even if the app window is closed,
    // the application won't exit.
    if (ret)
        trayIcon->hide();
    return ret;
}

int AppWindow::getNewUntitledIndex()
{
    int index = 0;
    QSet<int> vis;
    for (int t = 0; t < ui->tabWidget->count(); ++t)
    {
        auto tmp = windowAt(t);
        if (tmp->isUntitled() && tmp->getProblemURL().isEmpty())
        {
            vis.insert(tmp->getUntitledIndex());
        }
    }
    for (index = 1; vis.contains(index); ++index)
        ;
    return index;
}

/***************** ABOUT SECTION ***************************/

void AppWindow::on_actionSupport_me_triggered()
{
    Core::Log::i("appwindow/on_actionSupport_me_triggered", "Invoked");
    QDesktopServices::openUrl(QUrl("https://paypal.me/coder3101", QUrl::TolerantMode));
}

void AppWindow::on_actionManual_triggered()
{
    QDesktopServices::openUrl(
        QUrl("https://github.com/cpeditor/cpeditor/blob/" APP_VERSION "/doc/MANUAL.md", QUrl::TolerantMode));
}

void AppWindow::on_actionAbout_triggered()
{
    Core::Log::i("appwindow/on_actionAbout_triggered", "Invoked");
    QMessageBox::about(this, "About CP Editor " APP_VERSION,
                       "<p><b>CP Editor</b> is a native Qt-based code editor. It's specially designed for competitive "
                       "programming, unlike other editors/IDEs which are mainly for developers. It helps you focus on "
                       "your algorithm and automates the compilation, executing and testing. It even fetches test "
                       "cases for you from different platforms and submits solutions to Codeforces!</p>"
                       "<p>Copyright (C) 2019-2020 Ashar Khan &lt;ashar786khan@gmail.com&gt;</p>"
                       "<p>This is free software; see the source for copying conditions. There is NO warranty; not "
                       "even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. The source code for CP Editor is "
                       "available at <a href=\"https://github.com/cpeditor/cpeditor\"> "
                       "https://github.com/cpeditor/cpeditor</a>.</p>");
}

void AppWindow::on_actionAboutQt_triggered()
{
    Core::Log::i("AppWindow/on_actionAboutQt_triggered", "Invoked");
    QMessageBox::aboutQt(this);
}

void AppWindow::on_actionBuildInfo_triggered()
{
    Core::Log::i("AppWindow/on_actionBuildInfo_triggered", "Invoked");
    QMessageBox::about(this, "Build Info",
                       "App version: " APP_VERSION "\n"
                       "Git commit hash: " GIT_COMMIT_HASH "\n"
                       "Build time: " __DATE__ " " __TIME__ "\n"
#if defined(Q_OS_UNIX)
                       "OS: Linux\n"
#elif defined(Q_OS_WIN)
                       "OS: Windows\n"
#elif defined(Q_OS_MACOS)
                       "OS: MacOS\n"
#else
                       "OS: Unknown\n"
#endif
    );
}

/******************* FILES SECTION *************************/

void AppWindow::on_actionQuit_triggered()
{
    Core::Log::i("appwindow/on_actionQuit_triggered", "invoked");
    if (quit())
    {
        Core::Log::i("appwindow/on_actionQuit_triggered", "Exiting application");
        QApplication::exit();
    }
}

void AppWindow::on_actionNew_Tab_triggered()
{
    Core::Log::i("appwindow/on_actionNew_Tab_triggered", "invoked");
    openTab("");
}

void AppWindow::on_actionOpen_triggered()
{
    auto fileNames = QFileDialog::getOpenFileNames(this, tr("Open Files"), SettingsHelper::getSavePath(),
                                                   Util::fileNameFilter(true, true, true));
    Core::Log::i("appwindow/on_actionOpen_triggered") << " filename " << fileNames.join(", ") << endl;
    openTabs(fileNames);
}

void AppWindow::on_actionOpenContest_triggered()
{
    Core::Log::i("appwindow/on_actionOpenContest_triggered", "Invoked");
    auto path = QFileDialog::getExistingDirectory(this, "Open Contest");
    if (QFile::exists(path) && QFileInfo(path).isDir())
    {
        Core::Log::i("appwindow/on_actionOpenContest_triggered", "path exists and is a directory");
        bool ok = false;
        int number =
            QInputDialog::getInt(this, "Open Contest", "Number of problems in this contest:", 5, 0, 26, 1, &ok);
        if (ok)
        {
            Core::Log::i("appwindow/on_actionOpenContest_triggered") << "number of problems : " << number << endl;
            int current = 0;
            if (SettingsHelper::getDefaultLanguage() == "Java")
                current = 1;
            else if (SettingsHelper::getDefaultLanguage() == "Python")
                current = 2;
            auto lang = QInputDialog::getItem(this, "Open Contest", "Choose a language", {"C++", "Java", "Python"},
                                              current, false, &ok);
            if (ok)
            {
                Core::Log::i("appwindow/on_actionOpenContest_triggered")
                    << "opening contest with args " << path << " " << lang << " " << number << endl;
                openContest(path, lang, number);
            }
        }
    }
}

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
        auto tmp = windowAt(t);
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
        if (!windowAt(t)->isTextChanged() && closeTab(t))
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
        SettingsManager::reset();
        onSettingsApplied("");
        Core::Log::i("appwindow/on_actionRestore_Settings_triggered", "Reset success");
    }
}

void AppWindow::on_actionSettings_triggered()
{
    Core::Log::i("appwindow/on_actionSettings_triggered", "Launching settings window");
    preferencesWindow->display();
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
        openPaths(paths, cpp, java, python, depth);
    }
    else if (json["type"] == "contest")
    {
        Core::Log::i("appwindow/onReceivedMessage", "branched to contest");
        FROMJSON(number).toInt();
        FROMJSON(path).toString();
        QString lang = SettingsHelper::getDefaultLanguage();
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
        findReplaceDialog->setTextEdit(nullptr);
        setWindowTitle("CP Editor: An editor specially designed for competitive programming");
        return;
    }

    disconnect(activeSplitterMoveConnection);
    disconnect(activeRightSplitterMoveConnection);

    auto tmp = windowAt(index);

    findReplaceDialog->setTextEdit(tmp->getEditor());

    setWindowTitle(tmp->getCompleteTitle() + " - CP Editor");

    activeLogger = tmp->getLogger();
    server->setMessageLogger(activeLogger);

    if (ui->actionEditor_Mode->isChecked())
        on_actionEditor_Mode_triggered();
    else if (ui->actionIO_Mode->isChecked())
        on_actionIO_Mode_triggered();
    else if (ui->actionSplit_Mode->isChecked())
        on_actionSplit_Mode_triggered();

    tmp->getRightSplitter()->restoreState(SettingsHelper::getRightSplitterSize());

    activeSplitterMoveConnection =
        connect(tmp->getSplitter(), SIGNAL(splitterMoved(int, int)), this, SLOT(onSplitterMoved(int, int)));
    activeRightSplitterMoveConnection =
        connect(tmp->getRightSplitter(), SIGNAL(splitterMoved(int, int)), this, SLOT(onRightSplitterMoved(int, int)));
}

void AppWindow::onEditorFileChanged()
{
    Core::Log::i("appwindow/onEditorFileChanged", "Invoked onEditorFileChanged");

    if (SettingsHelper::isHotExitEnable() && SettingsHelper::isHotExitLoadFromFile())
        saveEditorStatus(true);

    if (currentWindow() != nullptr)
    {
        QMap<QString, QVector<int>> tabsByName;

        for (int t = 0; t < ui->tabWidget->count(); ++t)
        {
            tabsByName[windowAt(t)->getTabTitle(false, false)].push_back(t);
        }

        for (auto tabs : tabsByName)
        {
            QString longestCommonPrefix;
            if (tabs.size() > 1)
            {
                longestCommonPrefix = windowAt(tabs.front())->getCompleteTitle();
                for (int t = 1; t < tabs.length(); ++t)
                {
                    auto current = windowAt(tabs[t])->getCompleteTitle();
                    longestCommonPrefix = longestCommonPrefix.left(current.length());
                    for (int i = 0; i < longestCommonPrefix.length(); ++i)
                    {
                        if (longestCommonPrefix[i] != current[i])
                        {
                            longestCommonPrefix = longestCommonPrefix.left(i);
                            break;
                        }
                    }
                }
            }
            int removeLength = longestCommonPrefix.lastIndexOf('/') + 1;
            for (auto index : tabs)
            {
                ui->tabWidget->setTabText(index, windowAt(index)->getTabTitle(tabs.size() > 1, true, removeLength));
            }
        }

        setWindowTitle(currentWindow()->getCompleteTitle() + " - CP Editor");
        Core::Log::i("appwindow/onEditorFileChanged",
                     "Changed window title to " + currentWindow()->getTabTitle(true, false) + " - CP Editor");
    }
}

void AppWindow::onEditorTextChanged(MainWindow *window)
{
    int index = ui->tabWidget->indexOf(window);
    if (index != -1)
    {
        auto title = ui->tabWidget->tabText(index);
        // assume the clean title doesn't end with " *"
        if (title.endsWith(" *"))
            title.chop(2);
        if (windowAt(index)->isTextChanged())
            title += " *";
        ui->tabWidget->setTabText(index, title);
    }
}

void AppWindow::onSaveTimerElapsed()
{
    Core::Log::i("appwindow/onSaveTimerElapsed", "Autosave invoked");
    for (int t = 0; t < ui->tabWidget->count(); t++)
    {
        auto tmp = windowAt(t);
        if (!tmp->isUntitled())
        {
            tmp->save(false, "Auto Save", false);
            Core::Log::i("appwindow/onSaveTimerElapsed", "Autosave success");
        }
    }
}

void AppWindow::onSettingsApplied(const QString &pagePath)
{
    Core::Log::i("appwindow/onSettingsApplied") << INFO_OF(pagePath) << endl;

    for (int i = 0; i < ui->tabWidget->count(); ++i)
    {
        windowAt(i)->applySettings(pagePath, i == ui->tabWidget->currentIndex());
        onEditorTextChanged(windowAt(i));
    }

    if (pagePath.isEmpty() || pagePath == "Advanced/Update")
        updater->setBeta(SettingsHelper::isBeta());

    if (pagePath.isEmpty() || pagePath == "Key Bindings")
        maybeSetHotkeys();

    if (pagePath.isEmpty() || pagePath == "Extensions/Competitive Companion")
    {
        if (SettingsHelper::isCompetitiveCompanionEnable())
            server->updatePort(SettingsHelper::getCompetitiveCompanionConnectionPort());
        else
            server->updatePort(0);
    }

    if (pagePath.isEmpty() || pagePath == "Actions/Save")
    {
        if (SettingsHelper::isAutoSave())
            autoSaveTimer->start();
        else
            autoSaveTimer->stop();
    }

    if (pagePath.isEmpty() || pagePath == "Appearance")
        setWindowOpacity(SettingsHelper::getTransparency() / 100.0);

    Core::Log::i("appwindow/onSettingsApplied", "Finished");
}

void AppWindow::onIncomingCompanionRequest(const Extensions::CompanionData &data)
{
    Core::Log::i("appwindow/onIncomingCompanionRequest")
        << "Applying data to new tab. Args: shouldOpenNewTab:" << SettingsHelper::isCompetitiveCompanionOpenNewTab()
        << ", currentWindow == nullptr:" << (currentWindow() == nullptr) << endl;

    for (int i = 0; i < ui->tabWidget->count(); ++i)
    {
        if (windowAt(i)->getProblemURL() == data.url)
        {
            ui->tabWidget->setCurrentIndex(i);
            return;
        }
    }

    if (SettingsHelper::isCompetitiveCompanionOpenNewTab() || currentWindow() == nullptr)
        openTab("");
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
    SettingsHelper::setSplitterSize(splitter->saveState());
}

void AppWindow::onRightSplitterMoved(int _, int __)
{
    Core::Log::i("appwindow/onRightSplitterMoved", "updating state");
    auto splitter = currentWindow()->getRightSplitter();
    SettingsHelper::setRightSplitterSize(splitter->saveState());
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

void AppWindow::on_action_find_replace_triggered()
{
    auto tmp = currentWindow();
    if (tmp != nullptr)
        findReplaceDialog->showDialog(tmp->getEditor()->textCursor().selectedText());
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
        QString lang = current->getLanguage();
        QString head = QString("Snippets/%1/").arg(lang);
        QStringList names = SettingsManager::keyStartsWith(head);
        for (QString &key : names)
            key = key.mid(head.size());
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
                    QString content = SettingsManager::get(QString("Snippets/%1/%2").arg(lang, name)).toString();
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
    SettingsHelper::setViewMode("code");
    ui->actionEditor_Mode->setChecked(true);
    ui->actionIO_Mode->setChecked(false);
    ui->actionSplit_Mode->setChecked(false);
    if (currentWindow() != nullptr)
    {
        Core::Log::i("appwindow/on_actionEditor_Mode_triggered", "Switched to editor only mode");
        currentWindow()->setViewMode("code");
    }
    else
    {
        Core::Log::w("appwindow/on_actionEditor_Mode_triggered", "currentWindow is null. No UI changed");
    }
}

void AppWindow::on_actionIO_Mode_triggered()
{
    SettingsHelper::setViewMode("io");
    ui->actionEditor_Mode->setChecked(false);
    ui->actionIO_Mode->setChecked(true);
    ui->actionSplit_Mode->setChecked(false);
    if (currentWindow() != nullptr)
    {
        Core::Log::w("appwindow/on_actionIO_Mode_triggered", "Switched to IO Mode");
        currentWindow()->setViewMode("io");
    }
    else
    {
        Core::Log::w("appwindow/on_actionIO_Mode_triggered", "currentWindow is null. No UI changed");
    }
}

void AppWindow::on_actionSplit_Mode_triggered()
{
    SettingsHelper::setViewMode("split");
    ui->actionEditor_Mode->setChecked(false);
    ui->actionIO_Mode->setChecked(false);
    ui->actionSplit_Mode->setChecked(true);
    Core::Log::i("appwindow/on_actionSplit_Mode_triggered", "Entered split mode");
    if (currentWindow() != nullptr)
    {
        Core::Log::i("appwindow/on_actionSplit_Mode_triggered", "Restored splitter state");
        currentWindow()->setViewMode("split");
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

void AppWindow::on_action_duplicate_line_triggered()
{
    auto tmp = currentWindow();
    if (tmp != nullptr)
        tmp->getEditor()->duplicate();
    else
        Core::Log::w("appwindow/on_action_duplicate_line_triggered", "skipped action because no active tab exists");
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
        Core::Log::i("appwindow/onTabContextMenuRequested") << "Tab index is : " << index << endl;

        auto widget = windowAt(index);

        if (tabMenu != nullptr)
            delete tabMenu;
        tabMenu = new QMenu();

        tabMenu->addAction("Close", [index, this] { closeTab(index); });

        tabMenu->addAction("Close Others", [widget, this] {
            for (int i = 0; i < ui->tabWidget->count(); ++i)
                if (windowAt(i) != widget && closeTab(i))
                    --i;
        });

        tabMenu->addAction("Close to the Left", [widget, this] {
            for (int i = 0; i < ui->tabWidget->count() && windowAt(i) != widget; ++i)
                if (closeTab(i))
                    --i;
        });

        tabMenu->addAction("Close to the Right", [index, this] {
            for (int i = index + 1; i < ui->tabWidget->count(); ++i)
                if (closeTab(i))
                    --i;
        });
        tabMenu->addAction("Close Saved", [this] { on_actionClose_Saved_triggered(); });

        tabMenu->addAction("Close All", [this] { on_actionClose_All_triggered(); });
        QString filePath = widget->getFilePath();

        Core::Log::i("appwindow/onTabContextMenuRequested", "Filepath is : " + filePath);

        if (!widget->isUntitled() && QFile::exists(filePath))
        {
            Core::Log::i("appwindow/onTabContextMenuRequested", "Not untitled and filepath exists in system");
            tabMenu->addSeparator();
            tabMenu->addAction("Copy File Path", [filePath] { QGuiApplication::clipboard()->setText(filePath); });
            // Reference: http://lynxline.com/show-in-finder-show-in-explorer/ and https://forum.qt.io/post/296072
#if defined(Q_OS_MACOS)
            Core::Log::i("appwindow/onTabContextMenuRequested", "Adding menu reveal in finder");
            tabMenu->addAction("Reveal in Finder", [filePath] {
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
            tabMenu->addAction("Reveal in Explorer", [filePath] {
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
                    tabMenu->addAction("Open Containing Folder", [filePath] {
                        QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(filePath).path()));
                    });
                }
                else
                {
                    tabMenu->addAction("Reveal in File Manager", [program, args] {
                        QProcess openProcess;
                        openProcess.startDetached(program, args);
                    });
                }
            }
            else
            {
                Core::Log::i("appwindow/onTabContextMenuRequested/xdg-mime",
                             "Process cannot finish. So opting for openFolder by using QDesktopServices");

                tabMenu->addAction("Open Containing Folder", [filePath] {
                    QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(filePath).path()));
                });
            }
#else
            Core::Log::w("appwindow/onTabContextMenuRequested", "Unknown OS. Fallback to open via QDesktopServices");
            tabMenu->addAction("Open Containing Folder", [filePath] {
                QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(filePath).path()));
            });
#endif
        }
        else if (!widget->isUntitled() && QFile::exists(QFileInfo(widget->getFilePath()).path()))
        {
            Core::Log::i("appwindow/onTabContextMenuRequested",
                         "filepath does not exists. Looking if tab provides filepath " +
                             QFileInfo(widget->getFilePath()).path());
            tabMenu->addSeparator();
            tabMenu->addAction("Copy path", [filePath] {
                auto clipboard = QGuiApplication::clipboard();
                clipboard->setText(filePath);
            });
            tabMenu->addAction("Open Containing Folder", [filePath] {
                QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(filePath).path()));
            });
        }
        tabMenu->addSeparator();
        if (!widget->getProblemURL().isEmpty())
        {
            tabMenu->addAction("Open problem in browser",
                               [widget] { QDesktopServices::openUrl(widget->getProblemURL()); });
            tabMenu->addAction("Copy Problem URL",
                               [widget] { QGuiApplication::clipboard()->setText(widget->getProblemURL()); });
        }
        tabMenu->addAction("Set Codeforces URL", [widget, this] {
            QString contestId, problemCode;
            Extensions::CFTool::parseCfUrl(widget->getProblemURL(), contestId, problemCode);
            bool ok = false;
            contestId =
                QInputDialog::getText(this, "Set CF URL", "Enter the contest ID:", QLineEdit::Normal, contestId, &ok);
            if (ok)
                problemCode = QInputDialog::getText(
                    this, "Set CF URL", "Enter the problem Code (A-Z):", QLineEdit::Normal, problemCode, &ok);
            if (ok)
            {
                auto url = "https://codeforces.com/contest/" + contestId + "/problem/" + problemCode;
                widget->setProblemURL(url);
            }
        });
        tabMenu->addAction("Set Problem URL", [widget, this] {
            bool ok = false;
            auto url = QInputDialog::getText(this, "Set Problem URL", "Enter the new problem URL:", QLineEdit::Normal,
                                             widget->getProblemURL(), &ok);
            if (ok)
            {
                if (url.isEmpty() && widget->isUntitled())
                    widget->setUntitledIndex(getNewUntitledIndex());
                widget->setProblemURL(url);
            }
            else
                Core::Log::i("appwindow/onTabContextMenuRequested", "set problem url dialog closed or cancelled");
        });
        tabMenu->popup(ui->tabWidget->tabBar()->mapToGlobal(pos));
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

MainWindow *AppWindow::windowAt(int index)
{
    if (index == -1)
    {
        Core::Log::w("appwindow/windowAt", "No active window(-1), returning nullptr");
        return nullptr;
    }
    return dynamic_cast<MainWindow *>(ui->tabWidget->widget(index));
}

void AppWindow::on_actionShow_Logs_triggered()
{
    Core::Log::revealInFileManager();
}

void AppWindow::on_actionClear_Logs_triggered()
{
    Core::Log::clearOldLogs();
    if (currentWindow() != nullptr)
    {
        currentWindow()->getLogger()->info("EventLogger", "All logs except for current session has been deleted");
    }
}

void AppWindow::showOnTop()
{
    setWindowState((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    activateWindow();
    raise();
}

void AppWindow::onTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger)
    {
        if (!isMinimized())
            showMinimized();
        else
            showOnTop();
    }
}

// ---------------------------- LSP SLOTS ------------------------

void AppWindow::onLSPServerNotificationArrived(QString method, QJsonObject param)
{
}

void AppWindow::onLSPServerResponseArrived(QJsonObject method, QJsonObject param)
{
}

void AppWindow::onLSPServerRequestArrived(QString method, QJsonObject param, QJsonObject id)
{
}

void AppWindow::onLSPServerErrorArrived(QJsonObject id, QJsonObject error)
{
}

void AppWindow::onLSPServerProcessError(QProcess::ProcessError error)
{
}

void AppWindow::onLSPServerProcessFinished(int exitCode, QProcess::ExitStatus status)
{
}

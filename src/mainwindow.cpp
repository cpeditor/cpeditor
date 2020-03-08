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

#include "mainwindow.hpp"

#include "Core/Compiler.hpp"
#include "Core/EventLogger.hpp"
#include "Core/MessageLogger.hpp"
#include "Core/Runner.hpp"
#include "Util.hpp"
#include <QCXXHighlighter>
#include <QFileDialog>
#include <QFont>
#include <QFontDialog>
#include <QInputDialog>
#include <QJavaHighlighter>
#include <QMenu>
#include <QMessageBox>
#include <QMimeData>
#include <QPythonCompleter>
#include <QPythonHighlighter>
#include <QSaveFile>
#include <QScrollBar>
#include <QShortcut>
#include <QSyntaxStyle>
#include <QTextStream>
#include <QThread>
#include <QTimer>

#include "../ui/ui_mainwindow.h"

// ***************************** RAII  ****************************

MainWindow::MainWindow(const QString &fileOpen, int index, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), untitledIndex(index), fileWatcher(new QFileSystemWatcher(this))
{
    Core::Log::i("mainwindow/constructed") << " fileOpen " << fileOpen << " index " << index << endl;

    ui->setupUi(this);
    setTestCases();
    setEditor();
    setupCore();
    connect(fileWatcher, SIGNAL(fileChanged(const QString &)), this, SLOT(onFileWatcherChanged(const QString &)));
    applySettings(true);
    loadFile(fileOpen);
    if (testcases->count() == 0)
        testcases->addTestCase();
    QTimer::singleShot(0, [this] { setLanguage(language); }); // See issue #187 for more information
}

MainWindow::~MainWindow()
{
    Core::Log::i("mainwindow/destoryed", "Invoked");
    killProcesses();

    if (cftool != nullptr)
        delete cftool;
    if (tmpDir != nullptr)
        delete tmpDir;

    delete ui;
    delete editor;
    delete testcases;
    delete formatter;
    delete fileWatcher;
}

// ************************* RAII HELPER *****************************

void MainWindow::setTestCases()
{
    Core::Log::i("mainwindow/setTestCases", "Invoked");
    testcases = new TestCases(&log, this);
    ui->test_cases_layout->addWidget(testcases);
    connect(testcases, SIGNAL(checkerChanged()), this, SLOT(updateChecker()));
    connect(testcases, SIGNAL(requestRun(int)), this, SLOT(runTestCase(int)));
}

void MainWindow::setEditor()
{
    Core::Log::i("mainwindow/setEditor", "Invoked");
    editor = new QCodeEditor();
    editor->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
    editor->setAcceptDrops(false);

    ui->verticalLayout_8->addWidget(editor);

    connect(editor, SIGNAL(textChanged()), this, SLOT(onTextChanged()));

    // cursorPositionChanged() does not imply selectionChanged() if you press Left with
    // a selection (and the cursor is at the begin of the selection)
    connect(editor, SIGNAL(cursorPositionChanged()), this, SLOT(updateCursorInfo()));
    connect(editor, SIGNAL(selectionChanged()), this, SLOT(updateCursorInfo()));
}

void MainWindow::setupCore()
{
    Core::Log::i("mainwindow/setupCore", "Invoked");
    formatter = new Core::Formatter(Settings::SettingsManager::getClangFormatBinary(),
                                    Settings::SettingsManager::getClangFormatStyle(), &log);
    log.setContainer(ui->compiler_edit);
}

void MainWindow::compile()
{
    Core::Log::i("mainwindow/compile", "Invoked");
    killProcesses();
    compiler = new Core::Compiler();
    if (saveTemp("Compiler"))
    {
        if (language == "Python")
        {
            onCompilationFinished("");
            return;
        }
        else if (language != "C++" && language != "Java")
        {
            log.warn("Compiler", "Please set the language");
            return;
        }
        connect(compiler, SIGNAL(compilationStarted()), this, SLOT(onCompilationStarted()));
        connect(compiler, SIGNAL(compilationFinished(const QString &)), this,
                SLOT(onCompilationFinished(const QString &)));
        connect(compiler, SIGNAL(compilationErrorOccured(const QString &)), this,
                SLOT(onCompilationErrorOccured(const QString &)));
        connect(compiler, SIGNAL(compilationKilled()), this, SLOT(onCompilationKilled()));
        compiler->start(tmpPath(), Settings::SettingsManager::getCompileCommand(language), language);
    }
}

void MainWindow::run()
{
    Core::Log::i("mainwindow/run", "Invoked");
    killProcesses();
    testcases->clearOutput();

    if (!QStringList({"C++", "Java", "Python"}).contains(language))
    {
        log.warn("Runner", "Wrong language, please set the language");
        return;
    }

    for (int i = 0; i < testcases->count(); ++i)
    {
        if (!testcases->input(i).trimmed().isEmpty() && testcases->isShow(i))
        {
            run(i);
        }
    }

    if (runner.empty())
        log.warn("Runner", "All inputs are empty, nothing to run");
}

void MainWindow::run(int index)
{
    Core::Log::i("MainWindow/run") << INFO_OF(index) << endl;
    auto tmp = new Core::Runner(index);
    connect(tmp, SIGNAL(runStarted(int)), this, SLOT(onRunStarted(int)));
    connect(tmp, SIGNAL(runFinished(int, const QString &, const QString &, int, int)), this,
            SLOT(onRunFinished(int, const QString &, const QString &, int, int)));
    connect(tmp, SIGNAL(failedToStartRun(int, const QString &)), this, SLOT(onFailedToStartRun(int, const QString &)));
    connect(tmp, SIGNAL(runTimeout(int)), this, SLOT(onRunTimeout(int)));
    connect(tmp, SIGNAL(runKilled(int)), this, SLOT(onRunKilled(int)));
    tmp->run(tmpPath(), language, Settings::SettingsManager::getRunCommand(language),
             Settings::SettingsManager::getRuntimeArguments(language), testcases->input(index),
             Settings::SettingsManager::getTimeLimit());
    runner.push_back(tmp);
}

void MainWindow::runTestCase(int index)
{
    Core::Log::i("MainWindow/runTestCase") << INFO_OF(index) << endl;
    killProcesses();
    testcases->clearOutput();
    log.clear();

    if (!QStringList({"C++", "Java", "Python"}).contains(language))
    {
        log.warn("Runner", "Wrong language, please set the language");
        return;
    }

    run(index);
}

void MainWindow::loadTests()
{
    Core::Log::i("mainwindow/loadTests", "Invoked");
    if (!isUntitled() && Settings::SettingsManager::isSaveTests())
        testcases->loadFromFile(filePath);
}

void MainWindow::saveTests(bool safe)
{
    Core::Log::i("mainwindow/saveTests", "Invoked");
    if (!isUntitled() && Settings::SettingsManager::isSaveTests())
        testcases->save(filePath, safe);
}

void MainWindow::setCFToolUI()
{
    Core::Log::i("mainwindow/setCFToolUI", "Invoked");
    if (submitToCodeforces == nullptr)
    {
        submitToCodeforces = new QPushButton("Submit", this);
        cftool = new Network::CFTool(cftoolPath, &log);
        connect(cftool, SIGNAL(requestToastMessage(const QString &, const QString &)), this,
                SIGNAL(requestToastMessage(const QString &, const QString &)));
        ui->compile_and_run_buttons->addWidget(submitToCodeforces);
        connect(submitToCodeforces, &QPushButton::clicked, this, [this] {
            auto response = QMessageBox::warning(
                this, "Sure to submit",
                "Are you sure you want to submit this solution to Codeforces?\n\n URL: " + problemURL +
                    "\n Language : " + language,
                QMessageBox::Yes | QMessageBox::No);

            if (response == QMessageBox::Yes)
            {
                if (saveTemp("CF Tool Saver"))
                {
                    log.clear();
                    cftool->submit(tmpPath(), problemURL);
                }
            }
        });
    }
    if (!Network::CFTool::check(cftoolPath))
    {
        submitToCodeforces->setEnabled(false);
        log.error("CFTool", "You will not be able to submit code to Codeforces because CFTool is not installed or is "
                            "not on SYSTEM PATH. You can set it manually in settings.");
    }
}

int MainWindow::getUntitledIndex() const
{
    return untitledIndex;
}

QString MainWindow::getFileName() const
{
    if (!isUntitled())
        return QFileInfo(filePath).fileName();
    if (!problemURL.isEmpty())
        return problemURL.mid(problemURL.lastIndexOf('/') + 1);
    return "Untitled-" + QString::number(untitledIndex);
}

QString MainWindow::getFilePath() const
{
    return filePath;
}

QString MainWindow::getProblemURL() const
{
    return problemURL;
}

QString MainWindow::getCompleteTitle() const
{
    if (!isUntitled())
        return filePath;
    else if (!problemURL.isEmpty())
        return problemURL;
    else
        return getFileName();
}

QString MainWindow::getTabTitle(bool complete, bool star, int removeLength)
{
    QString tabTitle;
    if (!complete || (isUntitled() && problemURL.isEmpty()))
        tabTitle = getFileName();
    else if (!isUntitled())
        tabTitle = getFileName() + " - " + QFileInfo(filePath).canonicalPath().remove(0, removeLength);
    else
        tabTitle = getFileName() + " - " + getProblemURL().remove(0, removeLength);
    if (star && isTextChanged())
        tabTitle += " *";
    return tabTitle;
}

QCodeEditor *MainWindow::getEditor() const
{
    return editor;
}

bool MainWindow::isUntitled() const
{
    return filePath.isEmpty();
}

void MainWindow::setProblemURL(const QString &url)
{
    Core::Log::i("mainwindow/setProblemURL") << "url : " << url << endl;
    problemURL = url;
    if (problemURL.contains("codeforces.com"))
        setCFToolUI();
    emit editorFileChanged();
}

void MainWindow::setUntitledIndex(int index)
{
    Core::Log::i("mainwindow/setUntitledIndex") << "index " << index << endl;
    untitledIndex = index;
}

#define FROMSTATUS(x) x = status[#x]
MainWindow::EditorStatus::EditorStatus(const QMap<QString, QVariant> &status)
{
    Core::Log::i("mainwindow/editorStatus", "Invoked");
    FROMSTATUS(isLanguageSet).toInt();
    FROMSTATUS(filePath).toString();
    FROMSTATUS(savedText).toString();
    FROMSTATUS(problemURL).toString();
    FROMSTATUS(editorText).toString();
    FROMSTATUS(language).toString();
    FROMSTATUS(editorCursor).toInt();
    FROMSTATUS(editorAnchor).toInt();
    FROMSTATUS(horizontalScrollBarValue).toInt();
    FROMSTATUS(verticalScrollbarValue).toInt();
    FROMSTATUS(untitledIndex).toInt();
    FROMSTATUS(checkerIndex).toInt();
    FROMSTATUS(input).toStringList();
    FROMSTATUS(expected).toStringList();
    FROMSTATUS(customCheckers).toStringList();
    FROMSTATUS(testcasesIsShow).toList();
}
#undef FROMSTATUS

#define TOSTATUS(x) status[#x] = x
QMap<QString, QVariant> MainWindow::EditorStatus::toMap() const
{
    Core::Log::i("mainwindow/toMap", "Invoked");
    QMap<QString, QVariant> status;
    TOSTATUS(isLanguageSet);
    TOSTATUS(filePath);
    TOSTATUS(savedText);
    TOSTATUS(problemURL);
    TOSTATUS(editorText);
    TOSTATUS(language);
    TOSTATUS(editorCursor);
    TOSTATUS(editorAnchor);
    TOSTATUS(horizontalScrollBarValue);
    TOSTATUS(verticalScrollbarValue);
    TOSTATUS(untitledIndex);
    TOSTATUS(checkerIndex);
    TOSTATUS(input);
    TOSTATUS(expected);
    TOSTATUS(customCheckers);
    TOSTATUS(testcasesIsShow);
    return status;
}
#undef TOSTATUS

MainWindow::EditorStatus MainWindow::toStatus(bool simple) const
{
    Core::Log::i("mainwindow/toStatus", "Invoked");
    EditorStatus status;

    status.isLanguageSet = isLanguageSet;
    status.filePath = filePath;
    status.problemURL = problemURL;
    status.language = language;
    status.untitledIndex = untitledIndex;
    status.checkerIndex = testcases->checkerIndex();
    status.customCheckers = testcases->customCheckers();

    if (!simple)
    {
        status.editorText = editor->toPlainText();
        status.editorCursor = editor->textCursor().position();
        status.editorAnchor = editor->textCursor().anchor();
        status.horizontalScrollBarValue = editor->horizontalScrollBar()->value();
        status.verticalScrollbarValue = editor->verticalScrollBar()->value();
        status.input = testcases->inputs();
        status.expected = testcases->expecteds();
        for (int i = 0; i < testcases->count(); ++i)
            status.testcasesIsShow.push_back(testcases->isShow(i));
    }

    return status;
}

void MainWindow::loadStatus(const EditorStatus &status, bool simple)
{
    Core::Log::i("mainwindow/loadStatus", "Invoked");
    setProblemURL(status.problemURL);
    if (status.isLanguageSet)
        setLanguage(status.language);
    untitledIndex = status.untitledIndex;
    testcases->addCustomCheckers(status.customCheckers);
    testcases->setCheckerIndex(status.checkerIndex);

    Core::Log::i("mainwindow/loadStatus") << INFO_OF(simple) << ' ' << INFO_OF(status.filePath) << endl;

    if (!simple)
    {
        filePath = status.filePath;
        updateWatcher();
        savedText = status.savedText;
        editor->setPlainText(status.editorText);
        auto cursor = editor->textCursor();
        cursor.setPosition(status.editorAnchor);
        cursor.setPosition(status.editorCursor, QTextCursor::KeepAnchor);
        editor->setTextCursor(cursor);
        editor->horizontalScrollBar()->setValue(status.horizontalScrollBarValue);
        editor->verticalScrollBar()->setValue(status.verticalScrollbarValue);
        testcases->loadStatus(status.input, status.expected);
        for (int i = 0; i < status.testcasesIsShow.count() && i < testcases->count(); ++i)
            testcases->setShow(i, status.testcasesIsShow[i].toBool());
    }
    else if (!status.filePath.isEmpty())
    {
        loadFile(status.filePath);
    }
}

void MainWindow::applyCompanion(const Network::CompanionData &data)
{
    Core::Log::i("mainwindow/applyCompanion", "Invoked");
    if (isUntitled() && !isTextChanged())
    {
        QString meta = data.toMetaString();
        meta.prepend("\n");
        meta.append("Powered by CP Editor (https://github.com/cpeditor/cpeditor)");

        if (language == "Python")
            meta.replace('\n', "\n# ");
        else
            meta.replace('\n', "\n// ");

        editor->setPlainText(meta + "\n\n" + editor->toPlainText());
    }

    testcases->clear();

    for (int i = 0; i < data.testcases.size(); ++i)
        testcases->addTestCase(data.testcases[i].input, data.testcases[i].output);

    setProblemURL(data.url);
}

void MainWindow::applySettings(bool shouldPerformDigonistic)
{
    Core::Log::i("mainwindow/applySettingsData") << "shouldPerformDiagonistics " << shouldPerformDigonistic << endl;
    formatter->updateBinary(Settings::SettingsManager::getClangFormatBinary());
    formatter->updateStyle(Settings::SettingsManager::getClangFormatStyle());

    cftoolPath = Settings::SettingsManager::getCFPath();

    if (cftool != nullptr && Network::CFTool::check(cftoolPath))
    {
        cftool->updatePath(cftoolPath);
        if (submitToCodeforces != nullptr)
            submitToCodeforces->setEnabled(true);
    }

    Util::applySettingsToEditor(editor);

    if (!isLanguageSet)
    {
        setLanguage(Settings::SettingsManager::getDefaultLanguage());
    }
    if (shouldPerformDigonistic)
    {
        performCoreDiagonistics();
    }

    if (Settings::SettingsManager::isCompileAndRunOnly())
    {
        ui->compile->hide();
        ui->runOnly->hide();
    }
    else
    {
        ui->compile->show();
        ui->runOnly->show();
    }

    updateChecker();
}

void MainWindow::save(bool force, const QString &head, bool safe)
{
    Core::Log::i("mainwindow/save") << "force " << force << "head " << head << "safe " << safe << endl;
    saveFile(force ? SaveUntitled : IgnoreUntitled, head, safe);
}

void MainWindow::saveAs()
{
    Core::Log::i("mainwindow/saveAs", "Invoked");
    saveFile(SaveAs, "Save as", true);
}

void MainWindow::on_compile_clicked()
{
    Core::Log::i("mainwindow/on_compile_clicked", "Invoked");
    compileOnly();
}

void MainWindow::on_runOnly_clicked()
{
    Core::Log::i("mainwindow/on_runOnly_clicked", "Invoked");
    runOnly();
}

void MainWindow::on_run_clicked()
{
    Core::Log::i("mainwindow/on_run_clicked", "Invoked");
    compileAndRun();
}

void MainWindow::compileOnly()
{
    Core::Log::i("mainwindow/compileOnly", "Invoked");
    afterCompile = Nothing;
    log.clear();
    compile();
}

void MainWindow::runOnly()
{
    Core::Log::i("mainwindow/runOnly", "Invoked");
    if (language == "Python")
    {
        compileAndRun();
    }
    else
    {
        log.clear();
        run();
    }
}

void MainWindow::compileAndRun()
{
    Core::Log::i("mainwindow/compileAndRun", "Invoked");
    afterCompile = Run;
    log.clear();
    compile();
}

void MainWindow::formatSource()
{
    Core::Log::i("mainwindow/formatSource", "Invoked");
    formatter->format(editor, filePath, language, true);
}

void MainWindow::setLanguage(const QString &lang)
{
    Core::Log::i("mainwindow/setLanguage") << "lang " << lang << endl;
    log.clear();
    if (!QFile::exists(filePath))
    {
        QFile templateFile(Settings::SettingsManager::getTemplatePath(language));
        templateFile.open(QIODevice::ReadOnly | QIODevice::Text);
        QString templateContent;
        if (templateFile.isOpen())
            templateContent = templateFile.readAll();
        if (templateContent == editor->toPlainText())
        {
            language = lang;
            loadFile(filePath);
        }
    }
    language = lang;
    if (lang == "Python")
    {
        editor->setHighlighter(new QPythonHighlighter);
        editor->setCompleter(new QPythonCompleter);
        ui->changeLanguageButton->setText("Python");
    }
    else if (lang == "Java")
    {
        editor->setHighlighter(new QJavaHighlighter);
        editor->setCompleter(nullptr);
        ui->changeLanguageButton->setText("Java");
    }
    else
    {
        language = "C++";
        if (lang != "C++")
            log.warn("CP Editor", "Unknown lanague set, fallback to C++");
        editor->setHighlighter(new QCXXHighlighter);
        editor->setCompleter(nullptr);
        ui->changeLanguageButton->setText("C++");
    }
    performCoreDiagonistics();
    isLanguageSet = true;
}

QString MainWindow::getLanguage()
{
    return language;
}

MessageLogger *MainWindow::getLogger()
{
    return &log;
}

void MainWindow::insertText(const QString &text)
{
    editor->insertPlainText(text);
}

void MainWindow::setViewMode(Settings::ViewMode mode)
{
    switch (mode)
    {
    case Settings::ViewMode::FULL_EDITOR:
        ui->left_widget->show();
        ui->right_widget->hide();
        break;
    case Settings::ViewMode::FULL_IO:
        ui->left_widget->hide();
        ui->right_widget->show();
        break;
    case Settings::ViewMode::SPLIT:
        ui->left_widget->show();
        ui->right_widget->show();
        ui->splitter->restoreState(Settings::SettingsManager::getSplitterSizes());
        break;
    }
}

void MainWindow::detachedExecution()
{
    Core::Log::i("mainwindow/detachedExecution", "Invoked");
    afterCompile = RunDetached;
    log.clear();
    compile();
}

void MainWindow::killProcesses()
{
    Core::Log::i("mainwindow/killProcesses", "Invoked");

    if (compiler != nullptr)
    {
        delete compiler;
        compiler = nullptr;
    }

    for (auto &t : runner)
    {
        if (t != nullptr)
        {
            delete t;
        }
    }
    runner.clear();

    if (detachedRunner != nullptr)
    {
        delete detachedRunner;
        detachedRunner = nullptr;
    }
}

//***************** HELPER FUNCTIONS *****************

void MainWindow::setText(const QString &text, bool keep)
{
    if (keep)
    {
        auto cursor = editor->textCursor();
        int old_pos = cursor.position();
        cursor.select(QTextCursor::Document);
        cursor.insertText(text);
        cursor.setPosition(old_pos);
        editor->setTextCursor(cursor);
    }
    else
        editor->setPlainText(text);
}

void MainWindow::updateWatcher()
{
    Core::Log::i("mainwindow/updateWatcher", "Invoked");

    emit editorFileChanged();
    if (!fileWatcher->files().isEmpty())
        fileWatcher->removePaths(fileWatcher->files());
    if (!isUntitled())
        fileWatcher->addPath(filePath);
}

void MainWindow::loadFile(const QString &loadPath)
{
    Core::Log::i("mainwindow/loadFile") << "loadPath : " << loadPath << endl;

    auto path = loadPath;

    bool samePath = !isUntitled() && filePath == path;
    filePath = path;
    updateWatcher();

    if (!QFile::exists(path))
    {
        QString templatePath = Settings::SettingsManager::getTemplatePath(language);

        QFile f(templatePath);
        f.open(QIODevice::ReadOnly | QIODevice::Text);

        if (f.isOpen())
        {
            path = templatePath;
        }
        else
        {
            setText("");
            return;
        }
    }

    QFile openFile(path);
    openFile.open(QIODevice::ReadOnly | QFile::Text);
    if (openFile.isOpen())
    {
        savedText = openFile.readAll();
        setText(savedText, samePath);
    }
    else
    {
        log.warn("Loader", "Failed to load " + path + ". Do I have read permission?");
        return;
    }

    loadTests();
}

bool MainWindow::saveFile(SaveMode mode, const QString &head, bool safe)
{
    Core::Log::i("mainwindow/saveFile") << "mode " << mode << "head " << head << "safe " << safe << endl;
    if (Settings::SettingsManager::isFormatOnSave())
        formatter->format(editor, filePath, language, false);

    if (mode == SaveAs || (isUntitled() && mode == SaveUntitled))
    {
        QString defaultPath;
        if (!isUntitled())
        {
            defaultPath = filePath;
        }
        else
        {
            defaultPath = QDir(Settings::SettingsManager::getSavePath()).filePath(getTabTitle(false, false));
            if (language == "C++")
                defaultPath += ".cpp";
            else if (language == "Java")
                defaultPath += ".java";
            else if (language == "Python")
                defaultPath += ".py";
        }

        emit confirmTriggered(this);
        auto newFilePath = QFileDialog::getSaveFileName(
            this, tr("Save File"), defaultPath, "Source Files (*.cpp *.hpp *.h *.cc *.cxx *.c *.py *.py3 *.java)");
        if (newFilePath.isEmpty())
            return false;

        if (!Util::saveFile(newFilePath, editor->toPlainText(), head, safe, &log))
            return false;

        filePath = newFilePath;
        updateWatcher();
        Settings::SettingsManager::setSavePath(QFileInfo(filePath).canonicalPath());

        auto suffix = QFileInfo(filePath).suffix();
        if (suffix == ".cpp" || suffix == ".hpp" || suffix == ".h" || suffix == ".cc" || suffix == ".cxx" ||
            suffix == ".c")
            setLanguage("C++");
        else if (suffix == "java")
            setLanguage("Java");
        else if (suffix == "py" || suffix == "py3")
            setLanguage("Python");
    }
    else if (!isUntitled())
    {
        if (!Util::saveFile(filePath, editor->toPlainText(), head, safe, &log))
            return false;
        updateWatcher();
    }
    else
    {
        return false;
    }

    saveTests(safe);

    return true;
}

MainWindow::SaveTempStatus MainWindow::saveTemp(const QString &head)
{
    Core::Log::i("mainwindow/saveTemp") << "head " << head << endl;
    if (!saveFile(IgnoreUntitled, head, true))
    {
        if (tmpDir != nullptr)
        {
            delete tmpDir;
        }

        tmpDir = new QTemporaryDir();
        if (!tmpDir->isValid())
        {
            log.error(head, "Failed to create temporary directory");
            return Failed;
        }

        return Util::saveFile(tmpPath(), editor->toPlainText(), head, true, &log) ? TempSaved : Failed;
    }

    return NormalSaved;
}

QString MainWindow::tmpPath()
{
    Core::Log::i("mainwindow/tmpPath", "Invoked");

    if (!isUntitled() && !isTextChanged())
        return filePath;
    if (tmpDir == nullptr || !tmpDir->isValid())
    {
        switch (saveTemp("Temp Saver"))
        {
        case Failed:
            Core::Log::i("MainWindow/tmpPath", "Failed");
            log.error("Temp Saver", "Error occurred when trying to save temp file");
            return "";
        case TempSaved:
            Core::Log::i("MainWindow/tmpPath", "TempSaved");
            break;
        case NormalSaved:
            Core::Log::i("MainWindow/tmpPath", "NormalSaved");
            return filePath;
        }
    }
    QString name;
    if (language == "C++")
        name = "sol.cpp";
    else if (language == "Java")
        name = "sol.java";
    else if (language == "Python")
        name = "sol.py";
    else
    {
        log.error("Temp Saver", "Please set the language");
        return "";
    }
    return tmpDir->filePath(name);
}

bool MainWindow::isTextChanged()
{
    if (QFile::exists(filePath))
        filePath = QFileInfo(filePath).canonicalFilePath();

    if (isUntitled())
    {
        QFile f(Settings::SettingsManager::getTemplatePath(language));
        f.open(QIODevice::ReadOnly | QFile::Text);
        if (f.isOpen())
            return editor->toPlainText() != f.readAll();
        else
            return !editor->toPlainText().isEmpty();
    }
    else
    {
        QFile openFile(filePath);
        openFile.open(QIODevice::ReadOnly | QFile::Text);

        if (openFile.isOpen())
            return openFile.readAll() != editor->toPlainText();
        else
            return true;
    }
}

bool MainWindow::closeConfirm()
{
    Core::Log::i("mainwindow/closeConfirm", "Invoked");
    bool confirmed = !isTextChanged();
    if (!confirmed)
    {
        emit confirmTriggered(this);
        auto res = QMessageBox::warning(
            this, "Save changes?",
            "Save changes to " + (isUntitled() ? QString("New File") : getFileName()) + " before closing?",
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Cancel);
        if (res == QMessageBox::Save)
            confirmed = saveFile(SaveUntitled, "Save", true);
        else if (res == QMessageBox::Discard)
            confirmed = true;
    }
    return confirmed;
}

void MainWindow::on_changeLanguageButton_clicked()
{
    Core::Log::i("mainwindow/on_changeLanguageButton_clicked", "Invoked");
    bool ok = false;
    int curr = 0;

    if (language == "C++")
        curr = 0;
    else if (language == "Java")
        curr = 1;
    else if (language == "Python")
        curr = 2;

    auto response = QInputDialog::getItem(this, "Set Tab language", "Set the language to use in this Tab",
                                          {"C++", "Java", "Python"}, curr, false, &ok);

    if (ok)
        setLanguage(response);
}

void MainWindow::onFileWatcherChanged(const QString &path)
{
    Core::Log::i("mainwindow/onFileWatcherChanged") << "path " << path << endl;

    emit editorTextChanged(this);

    auto currentText = editor->toPlainText();

    QString fileText;
    QFile openFile(path);

    if (openFile.exists())
    {
        openFile.open(QIODevice::ReadOnly | QIODevice::Text);
        if (openFile.isOpen())
        {
            fileText = openFile.readAll();

            if (fileText == currentText)
            {
                savedText = fileText;
                return;
            }

            if (savedText == currentText)
            {
                loadFile(path);
                return;
            }

            if (!reloading)
            {
                reloading = true;

                auto reload = QMessageBox::question(
                    this, "Reload?", "\"" + filePath + "\"\n\nhas been changed on disk.\nDo you want to reload it?");

                reloading = false;

                if (reload == QMessageBox::StandardButton::Yes)
                {
                    loadFile(path);
                    return;
                }
            }
        }
    }
}

void MainWindow::onTextChanged()
{
    emit editorTextChanged(this);
}

void MainWindow::updateCursorInfo()
{
    auto cursor = editor->textCursor();
    auto selection = cursor.selectedText();
    QString info;
    if (selection.isEmpty())
    {
        auto line = editor->document()->findBlockByNumber(cursor.blockNumber()).text();
        int column = cursor.columnNumber();
        int col = 0;
        for (int i = 0; i < column; ++i)
        {
            if (line[i] != '\t')
                ++col;
            else
                col += Settings::SettingsManager::getTabStop() - col % Settings::SettingsManager::getTabStop();
        }
        info = "Line " + QString::number(cursor.blockNumber() + 1) + ", Column " + QString::number(col + 1);
    }
    else
    {
        int selectionStart = cursor.selectionStart();
        int selectionEnd = cursor.selectionEnd();
        cursor.setPosition(selectionStart);
        int lineStart = cursor.blockNumber();
        cursor.setPosition(selectionEnd);
        int lineEnd = cursor.blockNumber();
        int selectionLines = lineEnd - lineStart + 1;
        if (selectionLines > 1)
            info = QString::number(selectionLines) + " lines, ";
        info += QString::number(selection.length()) + " characters selected";
    }
    ui->cursor_info->setText(info);
}

void MainWindow::updateChecker()
{
    if (checker)
        delete checker;
    if (testcases->checkerType() == Core::Checker::Custom)
        checker = new Core::Checker(testcases->checkerText(), &log, Settings::SettingsManager::getTimeLimit(), this);
    else
        checker = new Core::Checker(testcases->checkerType(), &log, Settings::SettingsManager::getTimeLimit(), this);
    connect(checker, &Core::Checker::checkFinished, testcases, &TestCases::setVerdict);
    checker->prepare(Settings::SettingsManager::getCompileCommand("C++"));
}

QSplitter *MainWindow::getSplitter()
{
    return ui->splitter;
}

QSplitter *MainWindow::getRightSplitter()
{
    return ui->right_splitter;
}

void MainWindow::performCoreDiagonistics()
{
    Core::Log::i("mainwindow/performCoreDiagonistics", "Invoked");
    bool formatResult = Core::Formatter::check(Settings::SettingsManager::getClangFormatBinary(),
                                               Settings::SettingsManager::getClangFormatStyle());
    bool compilerResult = true;
    bool runResult = true;

    if (language == "C++" || language == "Java")
        compilerResult = Core::Compiler::check(Settings::SettingsManager::getCompileCommand(language));

    if (language == "Java" || language == "Python")
        runResult = Core::Compiler::check(Settings::SettingsManager::getRunCommand(language));

    if (!formatResult)
        log.warn("Formatter", "Code formatting failed to work. Please check whether the clang-format binary is in the "
                              "PATH and the style is valid.");
    if (!compilerResult)
        log.error("Compiler", "Compiler command for " + language + " is invalid. Is compiler on PATH?");

    if (!runResult)
        log.error("Runner",
                  "Binary or Script won't be executed because its corresponding program or VM could not be loaded");
}

// -------------------- COMPILER SLOTS ---------------------------

void MainWindow::onCompilationStarted()
{
    log.info("Compiler", "Compilation has started");
}

void MainWindow::onCompilationFinished(const QString &warning)
{
    Core::Log::i("mainwindow/oncompilationFinished") << "warning : \n" << warning << endl;

    if (language != "Python")
    {
        log.info("Compiler", "Compilation has finished");
        if (!warning.trimmed().isEmpty())
        {
            log.warn("Compile Warnings", warning);
        }
    }

    if (afterCompile == Run)
    {
        run();
    }
    else if (afterCompile == RunDetached)
    {
        killProcesses();

        if (!QStringList({"C++", "Java", "Python"}).contains(language))
        {
            log.warn("Runner", "Wrong language, please set the language");
            return;
        }

        detachedRunner = new Core::Runner(-1);
        connect(detachedRunner, SIGNAL(runStarted(int)), this, SLOT(onRunStarted(int)));
        connect(detachedRunner, SIGNAL(failedToStartRun(int, const QString &)), this,
                SLOT(onFailedToStartRun(int, const QString &)));
        connect(detachedRunner, SIGNAL(runKilled(int)), this, SLOT(onRunKilled(int)));
        detachedRunner->runDetached(tmpPath(), language, Settings::SettingsManager::getRunCommand(language),
                                    Settings::SettingsManager::getRuntimeArguments(language));
    }
}

void MainWindow::onCompilationErrorOccured(const QString &error)
{
    log.error("Complier", "Error occured while compiling");
    if (!error.trimmed().isEmpty())
        log.error("Compile Errors", error);
}

void MainWindow::onCompilationKilled()
{
    log.error("Compiler", "Compilation is killed");
}

// --------------------- RUNNER SLOTS ----------------------------

QString MainWindow::getRunnerHead(int index)
{
    if (index == -1)
        return "Detached Runner";
    return "Runner[" + QString::number(index + 1) + "]";
}

void MainWindow::onRunStarted(int index)
{
    log.info(getRunnerHead(index), "Execution has started");
}

void MainWindow::onRunFinished(int index, const QString &out, const QString &err, int exitCode, int timeUsed)
{
    auto head = getRunnerHead(index);

    if (exitCode == 0)
    {
        log.info(head, "Execution for test case #" + QString::number(index + 1) + " has finished in " +
                           QString::number(timeUsed) + "ms");
    }

    else
    {
        log.error(head, "Execution for test case #" + QString::number(index + 1) +
                            " has finished with non-zero exitcode " + QString::number(exitCode) + " in " +
                            QString::number(timeUsed) + "ms");
    }

    if (!err.trimmed().isEmpty())
        log.error(head + "/stderr", err);
    testcases->setOutput(index, out);
    if (!out.isEmpty() && !testcases->expected(index).isEmpty())
        checker->reqeustCheck(index, testcases->input(index), out, testcases->expected(index));
}

void MainWindow::onFailedToStartRun(int index, const QString &error)
{
    log.error(getRunnerHead(index), error);
}

void MainWindow::onRunTimeout(int index)
{
    log.warn(getRunnerHead(index), "Time Limit Exceeded");
}

void MainWindow::onRunKilled(int index)
{
    log.error(getRunnerHead(index),
              (index == -1 ? "Detached runner" : "Runner for test case #" + QString::number(index + 1)) +
                  " has been killed");
}

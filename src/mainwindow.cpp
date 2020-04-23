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
#include <QFileDialog>
#include <QFont>
#include <QFontDialog>
#include <QInputDialog>
#include <QMenu>
#include <QMessageBox>
#include <QMimeData>
#include <QSaveFile>
#include <QScrollBar>
#include <QShortcut>
#include <QSyntaxStyle>
#include <QTextBlock>
#include <QTextStream>
#include <QThread>
#include <QTimer>
#include <generated/SettingsHelper.hpp>

#include "../ui/ui_mainwindow.h"

// ***************************** RAII  ****************************

MainWindow::MainWindow(const QString &fileOpen, int index, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), untitledIndex(index), fileWatcher(new QFileSystemWatcher(this))
{
    LOG_INFO(INFO_OF(fileOpen) << INFO_OF(index));

    ui->setupUi(this);
    setTestCases();
    setEditor();
    setupCore();
    connect(fileWatcher, SIGNAL(fileChanged(const QString &)), this, SLOT(onFileWatcherChanged(const QString &)));
    applySettings("", true);
    loadFile(fileOpen);
    if (testcases->count() == 0)
        testcases->addTestCase();
    QTimer::singleShot(0, [this] { setLanguage(language); }); // See issue #187 for more information
}

MainWindow::~MainWindow()
{
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
    testcases = new Widgets::TestCases(&log, this);
    ui->test_cases_layout->addWidget(testcases);
    connect(testcases, SIGNAL(checkerChanged()), this, SLOT(updateChecker()));
    connect(testcases, SIGNAL(requestRun(int)), this, SLOT(runTestCase(int)));
}

void MainWindow::setEditor()
{
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
    formatter = new Extensions::ClangFormatter(SettingsHelper::getClangFormatPath(),
                                               SettingsHelper::getClangFormatStyle(), &log);
    log.setContainer(ui->compiler_edit);
}

void MainWindow::compile()
{
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
        connect(compiler, SIGNAL(compilationErrorOccurred(const QString &)), this,
                SLOT(onCompilationErrorOccurred(const QString &)));
        connect(compiler, SIGNAL(compilationKilled()), this, SLOT(onCompilationKilled()));
        compiler->start(tmpPath(), SettingsManager::get(QString("%1/Compile Command").arg(language)).toString(),
                        language);
    }
}

void MainWindow::run()
{
    LOG_INFO("Requesting run of testcases");
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
    auto tmp = new Core::Runner(index);
    connect(tmp, SIGNAL(runStarted(int)), this, SLOT(onRunStarted(int)));
    connect(tmp, SIGNAL(runFinished(int, const QString &, const QString &, int, int)), this,
            SLOT(onRunFinished(int, const QString &, const QString &, int, int)));
    connect(tmp, SIGNAL(failedToStartRun(int, const QString &)), this, SLOT(onFailedToStartRun(int, const QString &)));
    connect(tmp, SIGNAL(runTimeout(int)), this, SLOT(onRunTimeout(int)));
    connect(tmp, SIGNAL(runOutputLimitExceeded(int, const QString &)), this,
            SLOT(onRunOutputLimitExceeded(int, const QString &)));
    connect(tmp, SIGNAL(runKilled(int)), this, SLOT(onRunKilled(int)));
    tmp->run(tmpPath(), language, SettingsManager::get(QString("%1/Run Command").arg(language)).toString(),
             SettingsManager::get(QString("%1/Run Arguments").arg(language)).toString(), testcases->input(index),
             SettingsHelper::getTimeLimit());
    runner.push_back(tmp);
}

void MainWindow::runTestCase(int index)
{
    LOG_INFO(INFO_OF(index));
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
    if (!isUntitled() && SettingsHelper::isSaveTests())
        testcases->loadFromFile(filePath);
}

void MainWindow::saveTests(bool safe)
{
    if (!isUntitled() && SettingsHelper::isSaveTests())
        testcases->save(filePath, safe);
}

void MainWindow::setCFToolUI()
{
    if (submitToCodeforces == nullptr)
    {
        submitToCodeforces = new QPushButton("Submit", this);
        cftool = new Extensions::CFTool(cftoolPath, &log);
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
    if (!Extensions::CFTool::check(cftoolPath))
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
        return QRegularExpression(R"(.*/([^\?#].*?)/?$)").match(problemURL).captured(1);
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
    LOG_INFO(INFO_OF(url));
    problemURL = url;
    if (problemURL.contains("codeforces.com"))
        setCFToolUI();
    emit editorFileChanged();
}

void MainWindow::setUntitledIndex(int index)
{
    untitledIndex = index;
}

#define FROMSTATUS(x) x = status[#x]
MainWindow::EditorStatus::EditorStatus(const QMap<QString, QVariant> &status)
{
    LOG_INFO("Window status from map");
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
    LOG_INFO("Window status to hashmap");
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
    LOG_INFO("Moving to status this window state" << BOOL_INFO_OF(simple));
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
    LOG_INFO("Requesting loadStatus");
    setProblemURL(status.problemURL);
    if (status.isLanguageSet)
        setLanguage(status.language);
    untitledIndex = status.untitledIndex;
    testcases->addCustomCheckers(status.customCheckers);
    testcases->setCheckerIndex(status.checkerIndex);

    LOG_INFO(INFO_OF(simple) << INFO_OF(status.filePath));

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

void MainWindow::applyCompanion(const Extensions::CompanionData &data)
{
    LOG_INFO("Requesting apply from companion");
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

void MainWindow::applySettings(const QString &pagePath, bool shouldPerformDigonistic)
{
    LOG_INFO(INFO_OF(pagePath) << BOOL_INFO_OF(shouldPerformDigonistic));

    if (pagePath.isEmpty() || pagePath == "Extensions/Clang Format")
    {
        formatter->updateBinary(SettingsHelper::getClangFormatPath());
        formatter->updateStyle(SettingsHelper::getClangFormatStyle());
    }

    if (pagePath.isEmpty() || pagePath == "Extensions/CF Tool")
    {
        cftoolPath = SettingsHelper::getCFPath();

        if (cftool != nullptr && Extensions::CFTool::check(cftoolPath))
        {
            cftool->updatePath(cftoolPath);
            if (submitToCodeforces != nullptr)
                submitToCodeforces->setEnabled(true);
        }
    }

    if (pagePath.isEmpty() || pagePath == "Code Edit" || pagePath == "Appearance")
        Util::applySettingsToEditor(editor);

    if (!isLanguageSet && (pagePath.isEmpty() || pagePath == "Language/General"))
    {
        setLanguage(SettingsHelper::getDefaultLanguage());
    }

    if (shouldPerformDigonistic && (pagePath.isEmpty() || pagePath == "Language/Commands"))
    {
        performCompileAndRunDiagonistics();
    }

    if (pagePath.isEmpty() || pagePath == "Appearance")
    {
        if (SettingsHelper::isShowCompileAndRunOnly())
        {
            ui->compile->hide();
            ui->runOnly->hide();
        }
        else
        {
            ui->compile->show();
            ui->runOnly->show();
        }
    }

    if (pagePath.isEmpty() || pagePath == "Language/Commands")
        updateChecker();
}

void MainWindow::save(bool force, const QString &head, bool safe)
{
    LOG_INFO("Save " << BOOL_INFO_OF(force) << INFO_OF(head) << BOOL_INFO_OF(safe));
    saveFile(force ? SaveUntitled : IgnoreUntitled, head, safe);
}

void MainWindow::saveAs()
{
    LOG_INFO("Save as clicked");
    saveFile(SaveAs, "Save as", true);
}

void MainWindow::on_compile_clicked()
{
    LOG_INFO("Compile button clicked");
    compileOnly();
}

void MainWindow::on_runOnly_clicked()
{
    LOG_INFO("Run only button clicked");
    runOnly();
}

void MainWindow::on_run_clicked()
{
    LOG_INFO("Run button clicked");
    compileAndRun();
}

void MainWindow::compileOnly()
{
    LOG_INFO("Requesting Compile Only");
    afterCompile = Nothing;
    log.clear();
    compile();
}

void MainWindow::runOnly()
{
    LOG_INFO("Requesting Run only");
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
    LOG_INFO("Requested Compile and Run");
    afterCompile = Run;
    log.clear();
    compile();
}

void MainWindow::formatSource()
{
    LOG_INFO("Requested code format");
    formatter->format(editor, filePath, language, true);
}

void MainWindow::setLanguage(const QString &lang)
{
    LOG_INFO(INFO_OF(lang));
    if (!QFile::exists(filePath))
    {
        QString templateContent;
        if (!language.isEmpty())
            templateContent = Util::readFile(SettingsManager::get(QString("%1/Template Path").arg(language)).toString(),
                                             QString("Open %1 Template").arg(language), &log);
        if (templateContent == editor->toPlainText())
        {
            language = lang;
            loadFile(filePath);
        }
    }
    language = lang;
    if (language != "Python" && language != "Java")
        language = "C++";
    Util::setEditorLanguage(editor, language);
    ui->changeLanguageButton->setText(language);
    performCompileAndRunDiagonistics();
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

void MainWindow::setViewMode(const QString &mode)
{
    if (mode == "code")
    {
        ui->left_widget->show();
        ui->right_widget->hide();
    }
    else if (mode == "io")
    {
        ui->left_widget->hide();
        ui->right_widget->show();
    }
    else
    {
        ui->left_widget->show();
        ui->right_widget->show();
        ui->splitter->restoreState(SettingsHelper::getSplitterSize());
    }
}

void MainWindow::detachedExecution()
{
    LOG_INFO("Executing in detached mode");
    afterCompile = RunDetached;
    log.clear();
    compile();
}

void MainWindow::killProcesses()
{
    LOG_INFO("Killing all processes");

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
    emit editorFileChanged();
    if (!fileWatcher->files().isEmpty())
        fileWatcher->removePaths(fileWatcher->files());
    if (!isUntitled())
        fileWatcher->addPath(filePath);
}

void MainWindow::loadFile(const QString &loadPath)
{
    LOG_INFO(INFO_OF(loadPath));

    auto path = loadPath;

    bool samePath = !isUntitled() && filePath == path;
    filePath = path;
    updateWatcher();

    if (!QFile::exists(path))
    {
        QString templatePath = SettingsManager::get(QString("%1/Template Path").arg(language)).toString();

        QFile f(templatePath);

        if (!templatePath.isEmpty() && f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            path = templatePath;
        }
        else
        {
            setText("");
            return;
        }
    }

    auto content = Util::readFile(path, "Open File", &log);

    if (content.isNull())
        return;

    savedText = content;
    if (content.length() > SettingsHelper::getOpenFileLengthLimit())
    {
        content = "Open File Length Limit Exceeded";
        log.error("Open File",
                  QString("The file [%1] contains more than %2 characters, so it's not opened. You can change the "
                          "open file length limit in Preferences->Advanced->Limits->Open File Length Limit")
                      .arg(path)
                      .arg(SettingsHelper::getOpenFileLengthLimit()));
    }
    setText(content, samePath);

    loadTests();
}

bool MainWindow::saveFile(SaveMode mode, const QString &head, bool safe)
{
    LOG_INFO(INFO_OF(mode) << INFO_OF(head) << BOOL_INFO_OF(safe));
    if (SettingsHelper::isAutoFormat())
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
            defaultPath = QDir(SettingsHelper::getSavePath()).filePath(getTabTitle(false, false));
            if (language == "C++")
                defaultPath += ".cpp";
            else if (language == "Java")
                defaultPath += ".java";
            else if (language == "Python")
                defaultPath += ".py";
        }

        emit confirmTriggered(this);
        auto newFilePath =
            QFileDialog::getSaveFileName(this, tr("Save File"), defaultPath, Util::fileNameFilter(true, true, true));
        if (newFilePath.isEmpty())
            return false;

        if (!Util::saveFile(newFilePath, editor->toPlainText(), head, safe, &log))
            return false;

        filePath = newFilePath;
        updateWatcher();
        SettingsHelper::setSavePath(QFileInfo(filePath).canonicalPath());

        auto suffix = QFileInfo(filePath).suffix();
        if (Util::cppSuffix.contains(suffix))
            setLanguage("C++");
        else if (Util::javaSuffix.contains(suffix))
            setLanguage("Java");
        else if (Util::pythonSuffix.contains(suffix))
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
    LOG_INFO(INFO_OF(head));
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
    if (!isUntitled() && !isTextChanged())
        return filePath;
    if (tmpDir == nullptr || !tmpDir->isValid())
    {
        switch (saveTemp("Temp Saver"))
        {
        case Failed:
            log.error("Temp Saver", "Error occurred when trying to save temp file");
            return "";
        case TempSaved:
            break;
        case NormalSaved:
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
        auto content = Util::readFile(SettingsManager::get(QString("%1/Template Path").arg(language)).toString(),
                                      QString("Read %1 Template").arg(language), &log);
        if (content.isNull())
            return !editor->toPlainText().isEmpty();
        else
            return editor->toPlainText() != content;
    }
    else
    {
        auto content = Util::readFile(filePath);

        if (content.isNull())
            return true;

        return editor->toPlainText() != content;
    }
}

bool MainWindow::closeConfirm()
{
    LOG_INFO("Confirming to close this window");
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
    LOG_INFO(BOOL_INFO_OF(confirmed));
    return confirmed;
}

void MainWindow::on_changeLanguageButton_clicked()
{
    LOG_INFO("Window language change button requested");
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
    LOG_INFO(INFO_OF(path));

    emit editorTextChanged(this);

    auto currentText = editor->toPlainText();

    auto fileText = Util::readFile(path);

    if (!fileText.isNull())
    {
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
                col += SettingsHelper::getTabWidth() - col % SettingsHelper::getTabWidth();
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
        checker = new Core::Checker(testcases->checkerText(), &log, this);
    else
        checker = new Core::Checker(testcases->checkerType(), &log, this);
    connect(checker, &Core::Checker::checkFinished, testcases, &Widgets::TestCases::setVerdict);
    checker->prepare(SettingsManager::get(QString("C++/Compile Command")).toString());
}

QSplitter *MainWindow::getSplitter()
{
    return ui->splitter;
}

QSplitter *MainWindow::getRightSplitter()
{
    return ui->right_splitter;
}

void MainWindow::performCompileAndRunDiagonistics()
{
    bool compilerResult = true;
    bool runResult = true;

    if (language == "C++" || language == "Java")
        compilerResult =
            Core::Compiler::check(SettingsManager::get(QString("%1/Compile Command").arg(language)).toString());

    if (language == "Java" || language == "Python")
        runResult = Core::Compiler::check(SettingsManager::get(QString("%1/Run Command").arg(language)).toString());

    if (!compilerResult)
        log.error("Compiler",
                  "The compile command for " + language + " is invalid. Is the compiler in the system PATH?");

    if (!runResult)
        log.error("Runner", "The run command for " + language + " is invalid. Is the runner in the system Path?");
}

// -------------------- COMPILER SLOTS ---------------------------

void MainWindow::onCompilationStarted()
{
    log.info("Compiler", "Compilation has started");
}

void MainWindow::onCompilationFinished(const QString &warning)
{
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
        detachedRunner->runDetached(tmpPath(), language,
                                    SettingsManager::get(QString("%1/Run Command").arg(language)).toString(),
                                    SettingsManager::get(QString("%1/Run Arguments").arg(language)).toString());
    }
}

void MainWindow::onCompilationErrorOccurred(const QString &error)
{
    log.error("Complier", "Error occurred while compiling");
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

void MainWindow::onRunOutputLimitExceeded(int index, const QString &type)
{
    log.warn(
        getRunnerHead(index),
        QString("The %1 of the process running on the testcase #%2 contains more than %3 characters, which is longer "
                "than the output length limit, so the process is killed. You can change the output length limit "
                "in Preferences->Advanced->Limits->Output Length Limit")
            .arg(type)
            .arg(index + 1)
            .arg(SettingsHelper::getOutputLengthLimit()));
}

void MainWindow::onRunKilled(int index)
{
    log.error(getRunnerHead(index),
              (index == -1 ? "Detached runner" : "Runner for testcase #" + QString::number(index + 1)) +
                  " has been killed");
}

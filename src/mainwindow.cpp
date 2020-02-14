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

#include "mainwindow.hpp"

#include "Core/Compiler.hpp"
#include "Core/EventLogger.hpp"
#include "Core/MessageLogger.hpp"
#include "Core/Runner.hpp"
#include "Extensions/EditorTheme.hpp"
#include <QCXXHighlighter>
#include <QFileDialog>
#include <QFont>
#include <QFontDialog>
#include <QInputDialog>
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

MainWindow::MainWindow(QString fileOpen, Settings::SettingManager *manager, int index, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), settingManager(manager), untitledIndex(index),
      fileWatcher(new QFileSystemWatcher(this))
{
    ui->setupUi(this);
    setTestCases();
    setEditor();
    setupCore();
    connect(fileWatcher, SIGNAL(fileChanged(const QString &)), this, SLOT(onFileWatcherChanged(const QString &)));
    applySettingsData(true);
    loadFile(fileOpen);
    if (testcases->count() == 0)
        testcases->addTestCase();
}

MainWindow::~MainWindow()
{
    killProcesses();

    if (cftools != nullptr)
        delete cftools;
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
    testcases = new TestCases(&log, this);
    ui->test_cases_layout->addWidget(testcases);
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
    formatter =
        new Core::Formatter(settingManager->getClangFormatBinary(), settingManager->getClangFormatStyle(), &log);
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
        connect(compiler, SIGNAL(compilationErrorOccured(const QString &)), this,
                SLOT(onCompilationErrorOccured(const QString &)));
        compiler->start(tmpPath(), settingManager->getCompileCommand(language), language);
    }
}

void MainWindow::run()
{
    killProcesses();
    testcases->clearOutput();

    if (!QStringList({"C++", "Java", "Python"}).contains(language))
    {
        log.warn("Runner", "Wrong language, please set the language");
        return;
    }

    bool isRun = false;
    runner.resize(testcases->count());

    for (int i = 0; i < testcases->count(); ++i)
    {
        if (!testcases->input(i).trimmed().isEmpty())
        {
            isRun = true;
            runner[i] = new Core::Runner(i);
            connect(runner[i], SIGNAL(runStarted(int)), this, SLOT(onRunStarted(int)));
            connect(runner[i], SIGNAL(runFinished(int, const QString &, const QString &, int, int)), this,
                    SLOT(onRunFinished(int, const QString &, const QString &, int, int)));
            connect(runner[i], SIGNAL(runErrorOccured(int, const QString &)), this,
                    SLOT(onRunErrorOccured(int, const QString &)));
            connect(runner[i], SIGNAL(runTimeout(int)), this, SLOT(onRunTimeout(int)));
            connect(runner[i], SIGNAL(runKilled(int)), this, SLOT(onRunKilled(int)));
            runner[i]->run(tmpPath(), language, settingManager->getRunCommand(language),
                           settingManager->getRuntimeArguments(language), testcases->input(i),
                           settingManager->getTimeLimit());
        }
    }

    if (!isRun)
        log.warn("Runner", "All inputs are empty, nothing to run");
}

void MainWindow::loadTests()
{
    if (!isUntitled() && settingManager->isSaveTests())
        testcases->loadFromFile(filePath);
}

void MainWindow::saveTests()
{
    if (!isUntitled() && settingManager->isSaveTests())
        testcases->save(filePath);
}

void MainWindow::setCFToolsUI()
{
    if (submitToCodeforces == nullptr)
    {
        submitToCodeforces = new QPushButton("Submit Solution", this);
        cftools = new Network::CFTools(cftoolPath, &log);
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
                    cftools->submit(tmpPath(), problemURL, language);
            }
        });
    }
    if (!Network::CFTools::check(cftoolPath))
    {
        submitToCodeforces->setEnabled(false);
        log.error("CFTools", "You will not be able to submit code to Codeforces because CFTools is not installed or is "
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
    problemURL = url;
    if (problemURL.contains("codeforces.com"))
        setCFToolsUI();
    emit editorFileChanged();
}

void MainWindow::setUntitledIndex(int index)
{
    untitledIndex = index;
}

#define FROMSTATUS(x) x = status[#x]
MainWindow::EditorStatus::EditorStatus(const QMap<QString, QVariant> &status)
{
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
    FROMSTATUS(input).toStringList();
    FROMSTATUS(expected).toStringList();
}
#undef FROMSTATUS

#define TOSTATUS(x) status[#x] = x
QMap<QString, QVariant> MainWindow::EditorStatus::toMap() const
{
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
    TOSTATUS(input);
    TOSTATUS(expected);
    return status;
}
#undef TOSTATUS

MainWindow::EditorStatus MainWindow::toStatus(bool simple) const
{
    EditorStatus status;

    status.isLanguageSet = isLanguageSet;
    status.filePath = filePath;
    status.problemURL = problemURL;
    status.language = language;
    status.untitledIndex = untitledIndex;

    if (!simple)
    {
        status.editorText = editor->toPlainText();
        status.editorCursor = editor->textCursor().position();
        status.editorAnchor = editor->textCursor().anchor();
        status.horizontalScrollBarValue = editor->horizontalScrollBar()->value();
        status.verticalScrollbarValue = editor->verticalScrollBar()->value();
        status.input = testcases->inputs();
        status.expected = testcases->expecteds();
    }

    return status;
}

void MainWindow::loadStatus(const EditorStatus &status, bool simple)
{
    setProblemURL(status.problemURL);
    if (status.isLanguageSet)
        setLanguage(status.language);
    untitledIndex = status.untitledIndex;

    Core::Log::i("MainWindow/loadStatus") << INFO_OF(simple) << ' ' << INFO_OF(status.filePath) << endl;

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
    }
    else if (!status.filePath.isEmpty())
    {
        loadFile(status.filePath);
    }
}

void MainWindow::applyCompanion(Network::CompanionData data)
{
    if (isUntitled() && !isTextChanged())
    {
        QString meta = data.toMetaString();
        meta.prepend("\n");
        meta.append("Powered by CP Editor (https://github.com/coder3101/cp-editor)");

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

void MainWindow::applySettingsData(bool shouldPerformDigonistic)
{
    formatter->updateBinary(settingManager->getClangFormatBinary());
    formatter->updateStyle(settingManager->getClangFormatStyle());

    cftoolPath = settingManager->getCFPath();

    if (cftools != nullptr && Network::CFTools::check(cftoolPath))
    {
        cftools->updatePath(cftoolPath);
        if (submitToCodeforces != nullptr)
            submitToCodeforces->setEnabled(true);
    }

    editor->setTabReplace(settingManager->isTabsReplaced());
    editor->setTabReplaceSize(settingManager->getTabStop());
    editor->setAutoIndentation(settingManager->isAutoIndent());
    editor->setAutoParentheses(settingManager->isAutoParentheses());
    editor->setAutoRemoveParentheses(settingManager->isAutoRemoveParentheses());

    if (!settingManager->getFont().isEmpty())
    {
        QFont font;
        font.fromString(settingManager->getFont());
        editor->setFont(font);
    }

    const int tabStop = settingManager->getTabStop();
    QFontMetrics metric(editor->font());
    editor->setTabReplaceSize(tabStop);

    if (settingManager->isWrapText())
        editor->setWordWrapMode(QTextOption::WordWrap);
    else
        editor->setWordWrapMode(QTextOption::NoWrap);

    if (settingManager->getEditorTheme() == "Light")
        editor->setSyntaxStyle(Themes::EditorTheme::getLightTheme());
    else if (settingManager->getEditorTheme() == "Drakula")
        editor->setSyntaxStyle(Themes::EditorTheme::getDrakulaTheme());
    else if (settingManager->getEditorTheme() == "Monkai")
        editor->setSyntaxStyle(Themes::EditorTheme::getMonkaiTheme());
    else if (settingManager->getEditorTheme() == "Solarised")
        editor->setSyntaxStyle(Themes::EditorTheme::getSolarisedTheme());
    else if (settingManager->getEditorTheme() == "Solarised Dark")
        editor->setSyntaxStyle(Themes::EditorTheme::getSolarisedDarkTheme());
    else
    {
        log.warn("Themes", "Editor theme is set to invalid value. Fallback to Light");
        editor->setSyntaxStyle(Themes::EditorTheme::getLightTheme());
    }

    if (!isLanguageSet)
    {
        setLanguage(settingManager->getDefaultLanguage());
    }
    if (shouldPerformDigonistic)
    {
        performCoreDiagonistics();
    }
}

void MainWindow::save(bool force, const QString &head)
{
    saveFile(force ? SaveUntitled : IgnoreUntitled, head);
}

void MainWindow::saveAs()
{
    saveFile(SaveAs, "Save as");
}

void MainWindow::on_compile_clicked()
{
    compileOnly();
}

void MainWindow::on_runOnly_clicked()
{
    runOnly();
}

void MainWindow::on_run_clicked()
{
    compileAndRun();
}

void MainWindow::compileOnly()
{
    afterCompile = Nothing;
    log.clear();
    compile();
}

void MainWindow::runOnly()
{
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
    afterCompile = Run;
    log.clear();
    compile();
}

void MainWindow::formatSource()
{
    formatter->format(editor, filePath, language, true);
}

void MainWindow::setLanguage(QString lang)
{
    log.clear();
    if (!QFile::exists(filePath))
    {
        QFile templateFile(settingManager->getTemplatePath(language));
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
        editor->setHighlighter(new QCXXHighlighter);
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

void MainWindow::insertText(QString text)
{
    editor->insertPlainText(text);
}

void MainWindow::focusOnEditor()
{
    editor->setFocus();
}

void MainWindow::detachedExecution()
{
    afterCompile = RunDetached;
    log.clear();
    compile();
}

void MainWindow::killProcesses()
{
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

void MainWindow::loadFile(QString path)
{
    bool samePath = !isUntitled() && filePath == path;
    filePath = path;
    updateWatcher();

    if (!QFile::exists(path))
    {
        QString templatePath = settingManager->getTemplatePath(language);

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

bool MainWindow::saveFile(SaveMode mode, const QString &head)
{
    if (settingManager->isFormatOnSave())
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
            defaultPath = QDir(settingManager->getSavePath()).filePath(getTabTitle(false, false));
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

        QSaveFile openFile(newFilePath);
        openFile.open(QIODevice::WriteOnly | QFile::Text);
        openFile.write(editor->toPlainText().toStdString().c_str());

        if (!openFile.commit())
        {
            log.error(head, "Failed to save file to [" + newFilePath + "]. Do I have write permission?");
            return false;
        }

        filePath = newFilePath;
        updateWatcher();
        settingManager->setSavePath(QFileInfo(filePath).canonicalPath());

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
        QSaveFile openFile(filePath);
        openFile.open(QFileDevice::WriteOnly | QFile::Text);
        openFile.write(editor->toPlainText().toStdString().c_str());
        if (!openFile.commit())
        {
            log.error(head, "Failed to save file to [" + filePath + "]. Do I have write permission?");
            return false;
        }
        updateWatcher();
    }
    else
    {
        return false;
    }

    saveTests();

    return true;
}

bool MainWindow::saveTemp(const QString &head)
{
    if (!saveFile(IgnoreUntitled, head))
    {
        if (tmpDir != nullptr)
        {
            delete tmpDir;
        }

        tmpDir = new QTemporaryDir();
        if (!tmpDir->isValid())
        {
            log.error(head, "Failed to create temporary directory");
            return false;
        }

        QSaveFile tmpFile(tmpPath());
        tmpFile.open(QIODevice::WriteOnly | QIODevice::Text);
        tmpFile.write(editor->toPlainText().toStdString().c_str());

        if (!tmpFile.commit())
        {
            log.error(head, "Failed to save to " + tmpFile.fileName());
            return false;
        }
    }

    return true;
}

QString MainWindow::tmpPath()
{
    if (!isUntitled() && !isTextChanged())
        return filePath;
    if (tmpDir == nullptr || !tmpDir->isValid())
    {
        if (!saveTemp("Temp Saver"))
        {
            return "";
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
        QFile f(settingManager->getTemplatePath(language));
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
    bool confirmed = !isTextChanged();
    if (!confirmed)
    {
        emit confirmTriggered(this);
        auto res = QMessageBox::warning(
            this, "Save changes?",
            "Save changes to " + (isUntitled() ? QString("New File") : getFileName()) + " before closing?",
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Cancel);
        if (res == QMessageBox::Save)
            confirmed = saveFile(SaveUntitled, "Save");
        else if (res == QMessageBox::Discard)
            confirmed = true;
    }
    return confirmed;
}

void MainWindow::on_changeLanguageButton_clicked()
{
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
                col += settingManager->getTabStop() - col % settingManager->getTabStop();
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
    log.clear();
    bool formatResult =
        Core::Formatter::check(settingManager->getClangFormatBinary(), settingManager->getClangFormatStyle());
    bool compilerResult = true;
    bool runResult = true;

    if (language == "C++" || language == "Java")
        compilerResult = Core::Compiler::check(settingManager->getCompileCommand(language));

    if (language == "Java" || language == "Python")
        runResult = Core::Compiler::check(settingManager->getRunCommand(language));

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
        connect(detachedRunner, SIGNAL(runErrorOccured(int, const QString &)), this,
                SLOT(onRunErrorOccured(int, const QString &)));
        connect(detachedRunner, SIGNAL(runKilled(int)), this, SLOT(onRunKilled(int)));
        detachedRunner->runDetached(tmpPath(), language, settingManager->getRunCommand(language),
                                    settingManager->getRuntimeArguments(language));
    }
}

void MainWindow::onCompilationErrorOccured(const QString &error)
{
    log.error("Complier", "Error occured while compiling");
    if (!error.trimmed().isEmpty())
        log.error("Compile Errors", error);
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
}

void MainWindow::onRunErrorOccured(int index, const QString &error)
{
    log.error(getRunnerHead(index), error);
}

void MainWindow::onRunTimeout(int index)
{
    log.warn(getRunnerHead(index), "Time Limit Exceeded");
}

void MainWindow::onRunKilled(int index)
{
    log.info(getRunnerHead(index),
             (index == -1 ? "Detached runner" : "Runner for test case #" + QString::number(index + 1)) +
                 " has been killed");
}

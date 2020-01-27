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

#include <Compiler.hpp>
#include <DiffViewer.hpp>
#include <EditorTheme.hpp>
#include <MessageLogger.hpp>
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
#include <Runner.hpp>
#include <expand.hpp>

#include "../ui/ui_mainwindow.h"

// ***************************** RAII  ****************************
MainWindow::MainWindow(QString fileOpen, const Settings::SettingsData &data, int index, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), untitledIndex(index), fileWatcher(new QFileSystemWatcher(this))
{
    ui->setupUi(this);
    setEditor();
    setupCore();
    connect(fileWatcher, SIGNAL(fileChanged(const QString &)), this, SLOT(onFileWatcherChanged(const QString &)));
    setSettingsData(data, true);
    loadFile(fileOpen);
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
    delete formatter;
    delete fileWatcher;
}

// ************************* RAII HELPER *****************************

void MainWindow::setEditor()
{
    editor = new QCodeEditor();
    editor->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
    editor->setAcceptDrops(false);

    ui->verticalLayout_8->addWidget(editor);

    input[0] = ui->in1;
    input[1] = ui->in2;
    input[2] = ui->in3;

    output[0] = ui->out1;
    output[1] = ui->out2;
    output[2] = ui->out3;

    verdict[0] = ui->out1_verdict;
    verdict[1] = ui->out2_verdict;
    verdict[2] = ui->out3_verdict;

    for (int i = 0; i < 3; ++i)
    {
        input[i]->setWordWrapMode(QTextOption::NoWrap);
        output[i]->setWordWrapMode(QTextOption::NoWrap);
        input[i]->setAcceptDrops(false);
        expected[i] = new QString;
    }

    QObject::connect(editor, SIGNAL(textChanged()), this, SIGNAL(editorChanged()));

    for (auto i : {0, 1, 2})
        updateVerdict(UNKNOWN, i);
}

void MainWindow::setupCore()
{
    using namespace Core;
    formatter = new Formatter(data.clangFormatBinary, data.clangFormatStyle, &log);
    log.setContainer(ui->compiler_edit);
}

void MainWindow::compile()
{
    killProcesses();
    compiler = new Core::Compiler();
    if (saveTemp("Compiler"))
    {
        QString command;
        if (language == "C++")
            command = data.compileCommandCpp;
        else if (language == "Java")
            command = data.compileCommandJava;
        else
            return;
        connect(compiler, SIGNAL(compilationStarted()), this, SLOT(onCompilationStarted()));
        connect(compiler, SIGNAL(compilationFinished(const QString &)), this,
                SLOT(onCompilationFinished(const QString &)));
        connect(compiler, SIGNAL(compilationErrorOccured(const QString &)), this,
                SLOT(onCompilationErrorOccured(const QString &)));
        compiler->start(tmpPath(), command, language);
    }
}

void MainWindow::run()
{
    killProcesses();
    clearTests(true);

    QString command, args;
    if (language == "C++")
    {
        args = data.runtimeArgumentsCpp;
    }
    else if (language == "Java")
    {
        command = data.runCommandJava;
        args = data.runtimeArgumentsJava;
    }
    else if (language == "Python")
    {
        command = data.runCommandPython;
        args = data.runtimeArgumentsPython;
    }
    else
    {
        log.warn("Runner", "Wrong language, please set the language");
        return;
    }

    bool isRun = false;

    for (int i = 0; i < 3; ++i)
    {
        if (!input[i]->toPlainText().trimmed().isEmpty())
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
            runner[i]->run(tmpPath(), language, command, args, input[i]->toPlainText(), data.timeLimit);
        }
    }

    if (!isRun)
        log.warn("Runner", "All inputs are empty, nothing to run");
}

void MainWindow::clearTests(bool outputOnly)
{
    for (int i = 0; i < 3; ++i)
    {
        if (!outputOnly)
        {
            input[i]->clear();
            expected[i]->clear();
        }
        output[i]->clear();
        updateVerdict(UNKNOWN, i);
    }
}

void MainWindow::loadTests()
{
    if (isUntitled() || !data.shouldSaveTests)
        return;

    QFileInfo fileInfo(filePath);
    QString testFile = fileInfo.canonicalPath() + "/" + fileInfo.completeBaseName();

    for (int i = 0; i < 3; ++i)
    {
        if (QFile::exists(testFile + QString::number(i + 1) + ".in"))
        {
            QFile inputFile(testFile + QString::number(i + 1) + ".in");
            inputFile.open(QIODevice::ReadOnly | QFile::Text);
            if (inputFile.isOpen())
            {
                auto cursor = input[i]->textCursor();
                cursor.select(QTextCursor::Document);
                cursor.insertText(inputFile.readAll());
            }
            else
            {
                log.error("Tests", "Failed to open" + inputFile.fileName() + ". Do I have read permissions?");
            }
        }

        if (QFile::exists(testFile + QString::number(i + 1) + ".ans"))
        {
            QFile answerFile(testFile + QString::number(i + 1) + ".ans");
            answerFile.open(QIODevice::ReadOnly | QFile::Text);
            if (answerFile.isOpen())
            {
                *expected[i] = (answerFile.readAll());
            }
            else
            {
                log.error("Tests", "Failed to open" + answerFile.fileName() + ". Do I have read permissions?");
            }
        }
    }
}

void MainWindow::saveTests()
{
    if (isUntitled() || !data.shouldSaveTests)
        return;

    QFileInfo fileInfo(filePath);
    QString testFile = fileInfo.canonicalPath() + "/" + fileInfo.completeBaseName();

    for (int i = 0; i < 3; ++i)
    {
        if (!input[i]->toPlainText().trimmed().isEmpty())
        {
            QSaveFile inputFile(testFile + QString::number(i + 1) + ".in");
            inputFile.open(QIODevice::WriteOnly | QFile::Text);
            inputFile.write(input[i]->toPlainText().toStdString().c_str());
            if (!inputFile.commit())
            {
                log.error("Tests", "Failed to save Input #" + QString::number(i + 1) + ". Do I have write permission?");
            }
        }

        if (!expected[i]->trimmed().isEmpty())
        {
            QSaveFile answerFile(testFile + QString::number(i + 1) + ".ans");
            answerFile.open(QIODevice::WriteOnly | QFile::Text);
            answerFile.write(expected[i]->toStdString().c_str());
            if (!answerFile.commit())
            {
                log.error("Tests",
                          "Failed to save Expected #" + QString::number(i + 1) + ". Do I have write permission?");
            }
        }
    }
}

void MainWindow::setCFToolsUI()
{
    if (submitToCodeforces == nullptr)
    {
        submitToCodeforces = new QPushButton("Submit Solution", this);
        cftools = new Network::CFTools(cftoolPath, &log);
        ui->horizontalLayout_9->addWidget(submitToCodeforces);
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

QString MainWindow::getTabTitle(bool complete, bool star)
{
    QString tabTitle;
    if (!complete || (isUntitled() && problemURL.isEmpty()))
        tabTitle = getFileName();
    else if (!isUntitled())
        tabTitle = filePath;
    else
        tabTitle = problemURL;
    if (star && isTextChanged())
        tabTitle += " *";
    return tabTitle;
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
    emit editorChanged();
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

MainWindow::EditorStatus MainWindow::toStatus() const
{
    EditorStatus status;

    status.isLanguageSet = isLanguageSet;
    status.filePath = filePath;
    status.savedText = savedText;
    status.problemURL = problemURL;
    status.editorText = editor->toPlainText();
    status.language = language;
    status.editorCursor = editor->textCursor().position();
    status.editorAnchor = editor->textCursor().anchor();
    status.horizontalScrollBarValue = editor->horizontalScrollBar()->value();
    status.verticalScrollbarValue = editor->verticalScrollBar()->value();
    status.untitledIndex = untitledIndex;
    for (int i = 0; i < 3; ++i)
    {
        status.input.push_back(input[i]->toPlainText());
        status.expected.push_back(*expected[i]);
    }

    return status;
}

void MainWindow::loadStatus(const EditorStatus &status)
{
    filePath = status.filePath;
    updateWatcher();
    savedText = status.savedText;
    setProblemURL(status.problemURL);
    editor->setPlainText(status.editorText);
    if (status.isLanguageSet)
        setLanguage(status.language);
    auto cursor = editor->textCursor();
    cursor.setPosition(status.editorAnchor);
    cursor.setPosition(status.editorCursor, QTextCursor::KeepAnchor);
    editor->setTextCursor(cursor);
    editor->horizontalScrollBar()->setValue(status.horizontalScrollBarValue);
    editor->verticalScrollBar()->setValue(status.verticalScrollbarValue);
    untitledIndex = status.untitledIndex;
    for (int i = 0; i < 3; ++i)
    {
        if (status.input.length() > i)
            input[i]->setPlainText(status.input[i]);
        if (status.expected.length() > i)
            *expected[i] = status.expected[i];
    }
}

void MainWindow::updateVerdict(Verdict _verdict, int id)
{
    QString verdict_text, style_sheet;

    switch (_verdict)
    {
    case ACCEPTED:
        verdict_text = "AC";
        style_sheet = "QLabel { color : rgb(0, 180, 0); }";
        break;
    case WRONG_ANSWER:
        verdict_text = "WA";
        style_sheet = "QLabel { color : rgb(255, 0, 0); }";
        break;
    case UNKNOWN:
        verdict_text = "**";
        style_sheet = "";
        break;
    }

    verdict[id]->setText(verdict_text);
    verdict[id]->setStyleSheet(style_sheet);
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

    clearTests();

    if (data.testcases.size() > 3)
    {
        log.warn("CP Editor", "More than 3 testcase were produced. Only First 3 will be used");
    }

    for (int i = 0; i < data.testcases.size() && i < 3; ++i)
    {
        input[i]->setPlainText(data.testcases[i].input);
        *expected[i] = (data.testcases[i].output);
    }
    setProblemURL(data.url);
}

void MainWindow::setSettingsData(const Settings::SettingsData &data, bool shouldPerformDigonistic)
{
    this->data = data;
    formatter->updateBinary(data.clangFormatBinary);
    formatter->updateStyle(data.clangFormatStyle);

    cftoolPath = data.cfPath;

    if (cftools != nullptr && Network::CFTools::check(cftoolPath))
    {
        cftools->updatePath(cftoolPath);
        if (submitToCodeforces != nullptr)
            submitToCodeforces->setEnabled(true);
    }

    editor->setTabReplace(data.isTabsReplaced);
    editor->setTabReplaceSize(data.tabStop);
    editor->setAutoIndentation(data.isAutoIndent);
    editor->setAutoParentheses(data.isAutoParenthesis);

    if (!data.font.isEmpty())
    {
        QFont font;
        font.fromString(data.font);
        editor->setFont(font);
    }

    const int tabStop = data.tabStop;
    QFontMetrics metric(editor->font());
    editor->setTabReplaceSize(tabStop);
    editor->setTabStopDistance(tabStop * metric.horizontalAdvance("9"));

    if (data.isWrapText)
        editor->setWordWrapMode(QTextOption::WordWrap);
    else
        editor->setWordWrapMode(QTextOption::NoWrap);

    if (data.editorTheme == "Light")
        editor->setSyntaxStyle(Themes::EditorTheme::getLightTheme());
    else if (data.editorTheme == "Drakula")
        editor->setSyntaxStyle(Themes::EditorTheme::getDrakulaTheme());
    else if (data.editorTheme == "Monkai")
        editor->setSyntaxStyle(Themes::EditorTheme::getMonkaiTheme());
    else if (data.editorTheme == "Solarised")
        editor->setSyntaxStyle(Themes::EditorTheme::getSolarisedTheme());
    else if (data.editorTheme == "Solarised Dark")
        editor->setSyntaxStyle(Themes::EditorTheme::getSolarisedDarkTheme());
    else
    {
        log.warn("Themes", "Editor theme is set to invalid value. Fallback to Light");
        editor->setSyntaxStyle(Themes::EditorTheme::getLightTheme());
    }

    if (!isLanguageSet)
    {
        setLanguage(data.defaultLanguage);
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
    log.clear();
    run();
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
        QString templateContent, templatePath;
        if (language == "C++")
            templatePath = data.templateCpp;
        else if (language == "Java")
            templatePath = data.templateJava;
        else if (language == "Python")
            templatePath = data.templatePython;
        QFile templateFile(templatePath);
        templateFile.open(QIODevice::ReadOnly | QIODevice::Text);
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
            t = nullptr;
        }
    }

    if (detachedRunner != nullptr)
    {
        delete detachedRunner;
        detachedRunner = nullptr;
    }
}

// ****************************** Context Menus **************************/

void MainWindow::on_in1_customContextMenuRequested(const QPoint &pos)
{
    QMenu *stdMenu = ui->in1->createStandardContextMenu(pos);
    QAction *newAction = new QAction("Expand");

    QObject::connect(newAction, &QAction::triggered, this, [this] {
        auto ptr = new Expand(ui->in1, &this->log, this);
        ptr->setTitle("Input 1");
        ptr->setUpdate(true);
        ptr->setReadFile(true);
        ptr->show();
    });

    stdMenu->insertAction(stdMenu->actions().first(), newAction);
    stdMenu->popup(ui->in1->viewport()->mapToGlobal(pos));
}

void MainWindow::on_in2_customContextMenuRequested(const QPoint &pos)
{
    QMenu *stdMenu = ui->in2->createStandardContextMenu(pos);
    QAction *newAction = new QAction("Expand");

    QObject::connect(newAction, &QAction::triggered, this, [this] {
        auto ptr = new Expand(ui->in2, &this->log, this);
        ptr->setTitle("Input 2");
        ptr->setUpdate(true);
        ptr->setReadFile(true);
        ptr->show();
    });

    stdMenu->insertAction(stdMenu->actions().first(), newAction);
    stdMenu->popup(ui->in2->viewport()->mapToGlobal(pos));
}

void MainWindow::on_in3_customContextMenuRequested(const QPoint &pos)
{
    QMenu *stdMenu = ui->in3->createStandardContextMenu(pos);
    QAction *newAction = new QAction("Expand");

    QObject::connect(newAction, &QAction::triggered, this, [this] {
        auto ptr = new Expand(ui->in3, &this->log, this);
        ptr->setTitle("Input 3");
        ptr->setUpdate(true);
        ptr->setReadFile(true);
        ptr->show();
    });

    stdMenu->insertAction(stdMenu->actions().first(), newAction);
    stdMenu->popup(ui->in3->viewport()->mapToGlobal(pos));
}

void MainWindow::on_compiler_edit_customContextMenuRequested(const QPoint &pos)
{
    QMenu *stdMenu = ui->compiler_edit->createStandardContextMenu(pos);
    QAction *newAction = new QAction("Expand");

    QObject::connect(newAction, &QAction::triggered, this, [this] {
        auto ptr = new Expand(this->ui->compiler_edit, &this->log, this);
        ptr->show();
    });

    stdMenu->insertAction(stdMenu->actions().first(), newAction);
    stdMenu->popup(ui->compiler_edit->viewport()->mapToGlobal(pos));
}

void MainWindow::on_out1_customContextMenuRequested(const QPoint &pos)
{
    QMenu *stdMenu = ui->out1->createStandardContextMenu(pos);
    QAction *newAction = new QAction("Expand");

    QObject::connect(newAction, &QAction::triggered, this, [this] {
        auto ptr = new Expand(ui->out1, &this->log, this);
        ptr->setTitle("Output 1");
        ptr->setUpdate(false);
        ptr->setReadFile(false);
        ptr->show();
    });

    stdMenu->insertAction(stdMenu->actions().first(), newAction);
    stdMenu->popup(ui->out1->viewport()->mapToGlobal(pos));
}
void MainWindow::on_out2_customContextMenuRequested(const QPoint &pos)
{
    QMenu *stdMenu = ui->out2->createStandardContextMenu(pos);
    QAction *newAction = new QAction("Expand");

    QObject::connect(newAction, &QAction::triggered, this, [this] {
        auto ptr = new Expand(ui->out2, &this->log, this);
        ptr->setTitle("Output 2");
        ptr->setUpdate(false);
        ptr->setReadFile(false);
        ptr->show();
    });

    stdMenu->insertAction(stdMenu->actions().first(), newAction);
    stdMenu->popup(ui->out2->viewport()->mapToGlobal(pos));
}

void MainWindow::on_out3_customContextMenuRequested(const QPoint &pos)
{
    QMenu *stdMenu = ui->out3->createStandardContextMenu(pos);
    QAction *newAction = new QAction("Expand");

    QObject::connect(newAction, &QAction::triggered, this, [this] {
        auto ptr = new Expand(ui->out3, &this->log, this);
        ptr->setTitle("Output 3");
        ptr->setUpdate(false);
        ptr->setReadFile(false);
        ptr->show();
    });

    stdMenu->insertAction(stdMenu->actions().first(), newAction);
    stdMenu->popup(ui->out3->viewport()->mapToGlobal(pos));
}

//********************* DIFF Showers ******************

void MainWindow::on_out1_diff_clicked()
{
    auto ptr = new DiffViewer(expected[0], ui->out1, &log);
    ptr->setTitle("Diffviewer for Case #1");
    ptr->show();
}

void MainWindow::on_out2_diff_clicked()
{
    auto ptr = new DiffViewer(expected[1], ui->out2, &log);
    ptr->setTitle("Diffviewer for Case #2");
    ptr->show();
}

void MainWindow::on_out3_diff_clicked()
{
    auto ptr = new DiffViewer(expected[2], ui->out3, &log);
    ptr->setTitle("Diffviewer for Case #3");
    ptr->show();
}

//***************** HELPER FUNCTIONS *****************

bool MainWindow::isVerdictPass(QString output, QString expected)
{
    output = output.remove('\r');
    expected = expected.remove('\r');
    auto a_lines = output.split('\n');
    auto b_lines = expected.split('\n');
    for (int i = 0; i < a_lines.size() || i < b_lines.size(); ++i)
    {
        if (i >= a_lines.size())
        {
            if (b_lines[i].trimmed().isEmpty())
                continue;
            else
                return false;
        }
        if (i >= b_lines.size())
        {
            if (a_lines[i].trimmed().isEmpty())
                continue;
            else
                return false;
        }
        auto a_words = a_lines[i].split(' ');
        auto b_words = b_lines[i].split(' ');
        for (int j = 0; j < a_words.size() || j < b_words.size(); ++j)
        {
            if (j >= a_words.size())
            {
                if (b_words[j].trimmed().isEmpty())
                    continue;
                else
                    return false;
            }
            if (j >= b_words.size())
            {
                if (a_words[j].trimmed().isEmpty())
                    continue;
                else
                    return false;
            }
            if (a_words[j] != b_words[j])
                return false;
        }
    }
    return true;
}

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
    emit editorChanged();
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
        QString templatePath;

        if (language == "C++")
            templatePath = data.templateCpp;
        else if (language == "Java")
            templatePath = data.templateJava;
        else if (language == "Python")
            templatePath = data.templatePython;

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
    if (data.isFormatOnSave)
        formatter->format(editor, filePath, language, false);

    if (mode == SaveAs || (isUntitled() && mode == SaveUntitled))
    {
        emit confirmTriggered(this);
        auto newFilePath = QFileDialog::getSaveFileName(
            this, tr("Save File"), "", "Source Files (*.cpp *.hpp *.h *.cc *.cxx *.c *.py *.py3 *.java)");
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

    QString templatePath;

    if (language == "C++")
        templatePath = data.templateCpp;
    else if (language == "Java")
        templatePath = data.templateJava;
    else if (language == "Python")
        templatePath = data.templatePython;
    else
        return false;

    if (isUntitled())
    {
        QFile f(templatePath);
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
    emit editorChanged();

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

            auto reload = QMessageBox::question(
                this, "Reload?", "\"" + filePath + "\"\n\nhas been changed on disk.\nDo you want to reload it?");

            if (reload == QMessageBox::StandardButton::Yes)
            {
                loadFile(path);
                return;
            }
        }
    }
}

QSplitter *MainWindow::getSplitter()
{
    return ui->splitter;
}

void MainWindow::performCoreDiagonistics()
{
    log.clear();
    bool formatResult = Core::Formatter::check(data.clangFormatBinary, data.clangFormatStyle);
    bool compilerResult = true;
    bool runResults = true;

    if (language == "C++")
        compilerResult = Core::Compiler::check(data.compileCommandCpp);

    if (language == "Java")
    {
        compilerResult = Core::Compiler::check(data.compileCommandJava);
        runResults = Core::Compiler::check(data.runCommandJava);
    }

    if (language == "Python")
        compilerResult = Core::Compiler::check(data.runCommandPython);

    if (!formatResult)
        log.warn("Formatter", "Code formatting failed to work. Please check whether the clang-format binary is in the "
                              "PATH and the style is valid.");
    if (!compilerResult)
        log.error("Compiler", "Compiler command for " + language + " is invalid. Is compiler on PATH?");
    if (!runResults)
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
    log.info("Compiler", "Compilation has finished");
    if (!warning.trimmed().isEmpty())
    {
        log.warn("Compile Warnings", warning);
    }

    if (afterCompile == Run)
    {
        run();
    }
    else if (afterCompile == RunDetached)
    {
        killProcesses();

        QString command, args;
        if (language == "C++")
        {
            args = data.runtimeArgumentsCpp;
        }
        else if (language == "Java")
        {
            command = data.runCommandJava;
            args = data.runtimeArgumentsJava;
        }
        else if (language == "Python")
        {
            command = data.runCommandPython;
            args = data.runtimeArgumentsPython;
        }
        else
        {
            log.warn("Runner", "Wrong language, please set the language");
            return;
        }

        detachedRunner = new Core::Runner(-1);
        connect(detachedRunner, SIGNAL(runErrorOccured(int)), this, SLOT(onRunErrorOccured(int)));
        connect(detachedRunner, SIGNAL(runKilled(int)), this, SLOT(onRunKilled(int)));
        detachedRunner->runDetached(tmpPath(), language, command, args);
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
        if (!err.trimmed().isEmpty())
            log.error(head + "/stderr", err);
        output[index]->setPlainText(out);
        if (!expected[index]->isEmpty())
        {
            if (isVerdictPass(out, *expected[index]))
                updateVerdict(ACCEPTED, index);
            else
                updateVerdict(WRONG_ANSWER, index);
        }
    }

    else
    {
        log.error(head, "Execution for test case #" + QString::number(index + 1) +
                            " has finished with non-zero exitcode " + QString::number(exitCode) + " in " +
                            QString::number(timeUsed) + "ms");
        if (!err.trimmed().isEmpty())
            log.error(head + "/stderr", err);
    }
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

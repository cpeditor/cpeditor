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

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

class MessageLogger;
class QCodeEditor;
class QFileSystemWatcher;
class QPushButton;
class QSplitter;
class QTemporaryDir;

QT_BEGIN_NAMESPACE namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

namespace Core
{
class Checker;
class Compiler;
class Runner;
} // namespace Core

namespace Extensions
{
class CFTool;
class ClangFormatter;
struct CompanionData;
} // namespace Extensions

namespace Widgets
{
class TestCases;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    struct EditorStatus
    {
        bool isLanguageSet;
        QString filePath, savedText, problemURL, editorText, language;
        int editorCursor, editorAnchor, horizontalScrollBarValue, verticalScrollbarValue, untitledIndex, checkerIndex;
        QStringList input, expected, customCheckers;
        QVariantList testcasesIsShow;
        QVariantList testCaseSplitterStates;

        EditorStatus(){};

        explicit EditorStatus(const QMap<QString, QVariant> &status);

        QMap<QString, QVariant> toMap() const;
    };

    explicit MainWindow(int index, QWidget *parent);
    explicit MainWindow(const QString &fileOpen, int index, QWidget *parent);
    explicit MainWindow(const EditorStatus &status, bool duplicate, int index, QWidget *parent);
    ~MainWindow() override;

    int getUntitledIndex() const;
    QString getFileName() const;
    QString getFilePath() const;
    QString getProblemURL() const;
    QString getCompleteTitle() const;
    QString getTabTitle(bool complete, bool star, int removeLength = 0);
    QCodeEditor *getEditor() const;
    bool isUntitled() const;

    void setProblemURL(const QString &url);
    void setUntitledIndex(int index);

    EditorStatus toStatus() const;
    void loadStatus(const EditorStatus &status, bool duplicate = false);

    bool save(bool force, const QString &head, bool safe = true);
    void saveAs();

    bool isTextChanged() const;
    bool closeConfirm();

    void killProcesses();
    void detachedExecution();
    void compileOnly();
    void runOnly();
    void compileAndRun();
    void formatSource();

    void applyCompanion(const Extensions::CompanionData &data);

    void setLanguage(const QString &lang);
    QString getLanguage();
    void applySettings(const QString &pagePath, bool shouldPerformDigonistic);

    MessageLogger *getLogger();
    QSplitter *getSplitter();
    QSplitter *getRightSplitter();

    void insertText(const QString &text);

    void setViewMode(const QString &mode);
    QString tmpPath();

  private slots:
    void onCompilationStarted();
    void onCompilationFinished(const QString &warning);
    void onCompilationErrorOccurred(const QString &error);
    void onCompilationKilled();

    void onRunStarted(int index);
    void onRunFinished(int index, const QString &out, const QString &err, int exitCode, int timeUsed);
    void onFailedToStartRun(int index, const QString &error);
    void onRunTimeout(int index);
    void onRunOutputLimitExceeded(int index, const QString &type);
    void onRunKilled(int index);

    void onFileWatcherChanged(const QString &);
    void onEditorFontChanged(const QFont &newFont);
    void onTextChanged();
    void updateCursorInfo();
    void updateChecker();
    void runTestCase(int index);

    // UI Slots

    void on_compile_clicked();

    void on_runOnly_clicked();

    void on_run_clicked();

    void on_clearMessagesButton_clicked();

    void on_changeLanguageButton_clicked();

  signals:
    void editorFileChanged();
    void editorTmpPathChanged(MainWindow *window, const QString &path);
    void editorTextChanged(MainWindow *window);
    void editorFontChanged();
    void confirmTriggered(MainWindow *widget);
    void requestToastMessage(const QString &head, const QString &body);
    void editorLanguageChanged(MainWindow *window);
    void compileOrRunTriggered();

  private:
    enum SaveMode
    {
        IgnoreUntitled, // save only when filePath is not empty
        AutoSave,       // basically the same as IgnoreUntitled, only different in auto-format
        AlwaysSave,     // save to filePath if it's not empty, otherwise ask for new path
        SaveAs,         // ask for new path no matter filePath is empty or not
    };
    enum AfterCompile
    {
        Nothing,
        Run,
        RunDetached
    };

    Ui::MainWindow *ui;
    QCodeEditor *editor;
    QString language;
    bool isLanguageSet = false;

    Extensions::ClangFormatter *formatter = nullptr;
    Core::Compiler *compiler = nullptr;
    QVector<Core::Runner *> runner;
    Core::Checker *checker = nullptr;
    Core::Runner *detachedRunner = nullptr;
    QTemporaryDir *tmpDir = nullptr;
    AfterCompile afterCompile = Nothing;

    MessageLogger *log = nullptr;

    int untitledIndex;
    QString problemURL;
    QString filePath;
    QString savedText;
    QString cftoolPath;
    QFileSystemWatcher *fileWatcher;
    bool reloading = false;

    QPushButton *submitToCodeforces = nullptr;
    Extensions::CFTool *cftool = nullptr;

    Widgets::TestCases *testcases = nullptr;

    QTimer *autoSaveTimer = nullptr;

    void setTestCases();
    void setEditor();
    void setupCore();
    void compile();
    void run();
    void run(int index);
    void loadTests();
    void saveTests(bool safe);
    void setCFToolUI();
    void setFilePath(QString path, bool updateBinder = true);
    void setText(const QString &text, bool keep = false);
    void updateWatcher();
    void loadFile(const QString &loadPath);
    bool saveFile(SaveMode mode, const QString &head, bool safe);
    void performCompileAndRunDiagonistics();
    QString getRunnerHead(int index);
};
#endif // MAINWINDOW_HPP

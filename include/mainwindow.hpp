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

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <CompanionServer.hpp>
#include <Compiler.hpp>
#include <Formatter.hpp>
#include <IO.hpp>
#include <QCodeEditor>
#include <QFile>
#include <QFileSystemWatcher>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QShortcut>
#include <QSplitter>
#include <QTemporaryDir>
#include <Runner.hpp>
#include <SettingsManager.hpp>
#include <TestCases.hpp>
#include <UpdateNotifier.hpp>
#include <cftools.hpp>
#include <generated/version.hpp>

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    struct EditorStatus
    {
        bool isLanguageSet;
        QString filePath, savedText, problemURL, editorText, language;
        int editorCursor, editorAnchor, horizontalScrollBarValue, verticalScrollbarValue, untitledIndex;
        QStringList input, expected;

        EditorStatus(){};

        EditorStatus(const QMap<QString, QVariant> &status);

        QMap<QString, QVariant> toMap() const;
    };

    MainWindow(QString fileOpen, const Settings::SettingsData &data, int index = 0, QWidget *parent = nullptr);
    ~MainWindow() override;

    int getUntitledIndex() const;
    QString getFileName() const;
    QString getFilePath() const;
    QString getProblemURL() const;
    QString getTabTitle(bool complete, bool star);
    bool isUntitled() const;

    void setProblemURL(const QString &url);

    EditorStatus toStatus() const;
    void loadStatus(const EditorStatus &status);

    void save(bool force, const QString &head);
    void saveAs();

    bool isTextChanged();
    bool closeConfirm();

    void killProcesses();
    void detachedExecution();
    void compileOnly();
    void runOnly();
    void compileAndRun();
    void formatSource();

    void applyCompanion(Network::CompanionData data);

    void setLanguage(QString lang);
    QString getLanguage();
    void setSettingsData(const Settings::SettingsData &data, bool);

    MessageLogger *getLogger();
    QSplitter *getSplitter();

    void insertText(QString text);

    void focusOnEditor();

  private slots:
    void on_compile_clicked();
    void on_runOnly_clicked();
    void on_run_clicked();

    void onCompilationStarted();
    void onCompilationFinished(const QString &warning);
    void onCompilationErrorOccured(const QString &error);

    void onRunStarted(int index);
    void onRunFinished(int index, const QString &out, const QString &err, int exitCode, int timeUsed);
    void onRunErrorOccured(int index, const QString &error);
    void onRunTimeout(int index);
    void onRunKilled(int index);

    void on_changeLanguageButton_clicked();

    void onFileWatcherChanged(const QString &);

  signals:
    void editorChanged();
    void confirmTriggered(MainWindow *widget);

  private:
    enum SaveMode
    {
        IgnoreUntitled, // save only when filePath is not empty
        SaveUntitled,   // save to filePath if it's not empty, otherwise ask for new path
        SaveAs,         // ask for new path no matter filePath is empty or not
    };
    enum AfterCompile
    {
        Nothing,
        Run,
        RunDetached
    };
    enum Verdict
    {
        ACCEPTED,
        WRONG_ANSWER,
        UNKNOWN
    };

    Ui::MainWindow *ui;
    QCodeEditor *editor;
    QString language;
    Settings::SettingsData data;
    bool isLanguageSet = false;

    Core::Formatter *formatter = nullptr;
    Core::Compiler *compiler = nullptr;
    QVector<Core::Runner *> runner;
    Core::Runner *detachedRunner = nullptr;
    QTemporaryDir *tmpDir = nullptr;
    AfterCompile afterCompile = Nothing;

    MessageLogger log;

    int untitledIndex;
    QString problemURL;
    QString filePath;
    QString savedText;
    QString cftoolPath;
    QFileSystemWatcher *fileWatcher;

    QPushButton *submitToCodeforces = nullptr;
    Network::CFTools *cftools = nullptr;

    TestCases *testcases = nullptr;

    void setTestCases();
    void setEditor();
    void setupCore();
    void compile();
    void run();
    void loadTests();
    void saveTests();
    void setCFToolsUI();
    void setText(const QString &text, bool saveCursor = false);
    void updateWatcher();
    void loadFile(QString path);
    bool saveFile(SaveMode, const QString &head);
    bool saveTemp(const QString &head);
    QString tmpPath();
    void performCoreDiagonistics();
    QString getRunnerHead(int index);
};
#endif // MAINWINDOW_HPP

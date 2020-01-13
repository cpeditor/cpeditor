/*
 * Copyright (C) 2019 Ashar Khan <ashar786khan@gmail.com>
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
 * Believe Software is "Software" and it isn't not immune to bugs.
 *
 */

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <CompanionServer.hpp>
#include <Core.hpp>
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
#include <Runner.hpp>
#include <SettingsManager.hpp>
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
    enum SaveMode
    {
        IgnoreUntitled = 0, // save only when filePath is not empty
        SaveUntitled = 1,   // save to filePath if it's not empty, otherwise ask for new path
        SaveAs = 3          // ask for new path no matter filePath is empty or not
    };

    MainWindow(int index, QString fileOpen, QWidget *parent = nullptr);
    ~MainWindow() override;

    QString getFileName() const;
    QString getFilePath() const;
    QString getProblemURL() const;
    QString getTabTitle(bool complete) const;
    bool isUntitled() const;
    void save(bool force);
    void saveAs();

    bool closeConfirm();

    void killProcesses();
    void detachedExecution();
    void compile();
    void run();
    void runAndCompile();
    void formatSource();

    void applyCompanion(Network::CompanionData data);
    void maybeLoadTemplate();

    void setLanguage(QString lang);
    QString getLanguage();
    void setSettingsData(Settings::SettingsData data, bool);

    MessageLogger *getLogger();
    QSplitter *getSplitter();

    void insertText(QString text);

  private slots:
    void onTextChangedTriggered();

    void on_compile_clicked();
    void on_run_clicked();
    void on_runOnly_clicked();

    void executionFinished(int, int, QString);

    void on_in1_customContextMenuRequested(const QPoint &pos);
    void on_in2_customContextMenuRequested(const QPoint &pos);
    void on_in3_customContextMenuRequested(const QPoint &pos);
    void on_compiler_edit_customContextMenuRequested(const QPoint &pos);
    void on_out3_customContextMenuRequested(const QPoint &pos);
    void on_out2_customContextMenuRequested(const QPoint &pos);
    void on_out1_customContextMenuRequested(const QPoint &pos);

    void on_out1_diff_clicked();
    void on_out2_diff_clicked();
    void on_out3_diff_clicked();

    void on_changeLanguageButton_clicked();

    void onFileWatcherChanged(const QString &);

  signals:
    void editorChanged(MainWindow *widget);
    void confirmTriggered(MainWindow *widget);

  private:
    const int windowIndex;
    Ui::MainWindow *ui;
    QCodeEditor *editor;
    QString language;
    Settings::SettingsData data;
    bool isLanguageSet = false;

    Core::Formatter *formatter = nullptr;
    Core::IO::InputReader *inputReader = nullptr;
    Core::Compiler *compiler = nullptr;
    Core::Runner *runner = nullptr;
    MessageLogger log;

    QString problemURL;
    QString filePath;
    QString savedText;
    QFileSystemWatcher *fileWatcher = nullptr;

    QVector<QPlainTextEdit *> input = QVector<QPlainTextEdit *>(3, nullptr);
    QVector<QPlainTextEdit *> output = QVector<QPlainTextEdit *>(3, nullptr);
    QVector<QLabel *> verdict = QVector<QLabel *>(3, nullptr);
    QVector<QString *> expected = QVector<QString *>(3, nullptr);
    QPushButton *submitToCodeforces = nullptr;
    Network::CFTools *cftools = nullptr;

    void setEditor();
    void setupCore();
    void clearTests(bool outputOnly = false);
    void loadTests();
    void saveTests();
    void setCFToolsUI();
    void updateVerdict(Core::Verdict, int);
    bool isTextChanged() const;
    bool isVerdictPass(QString, QString);
    void setText(const QString &text, bool saveCursor = false);
    void loadFile(const QString &path);
    bool saveFile(SaveMode, std::string);
    void performCoreDiagonistics();
};
#endif // MAINWINDOW_HPP

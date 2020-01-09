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
    MainWindow(int index, QString fileOpen, QWidget *parent = nullptr);
    ~MainWindow() override;

    QString fileName() const;
    QString filePath() const;
    QString problemURL() const;
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
    void setSettingsData(Settings::SettingsData data, bool);

    MessageLogger *getLogger();
    QFile *getOpenFile();
    QSplitter *getSplitter();

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

    void on_changeLanguageButoon_clicked();

  signals:
    void editorTextChanged(bool isUnsaved, MainWindow *widget);
    void confirmTriggered(MainWindow *widget);

  private:
    const int windowIndex;
    Ui::MainWindow *ui;
    QCodeEditor *editor;
    QString language;
    QFile *openFile = nullptr;
    Settings::SettingsData data;
    bool isLanguageSet = false;

    Core::Formatter *formatter = nullptr;
    Core::IO::InputReader *inputReader = nullptr;
    Core::Compiler *compiler = nullptr;
    Core::Runner *runner = nullptr;
    MessageLogger log;

    QVector<QPlainTextEdit *> input = QVector<QPlainTextEdit *>(3, nullptr);
    QVector<QPlainTextEdit *> output = QVector<QPlainTextEdit *>(3, nullptr);
    QVector<QLabel *> verdict = QVector<QLabel *>(3, nullptr);
    QVector<QString *> expected = QVector<QString *>(3, nullptr);
    Network::CompanionData companionData;
    QPushButton *submitToCodeforces = nullptr;
    Network::CFTools *cftools = nullptr;

    void setEditor();
    void setupCore();
    void clearTests(bool outputOnly = false);
    void loadTests();
    void saveTests();
    void setCFToolsUI();
    void updateVerdict(Core::Verdict, int);
    bool isTextChanged();

    bool isVerdictPass(QString, QString);
    bool saveFile(bool, std::string);
    void performCoreDiagonistics();
};
#endif // MAINWINDOW_HPP

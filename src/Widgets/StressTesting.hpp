/*
 * Copyright (C) 2019-2021 Ashar Khan <ashar786khan@gmail.com>
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

#ifndef STRESSTESTING_HPP
#define STRESSTESTING_HPP

#include "Widgets/TestCase.hpp"
#include <QMainWindow>

class PathItem;
class QLabel;
class MainWindow;
class QLineEdit;
class QPushButton;
class QCheckBox;
class MessageLogger;
class QTemporaryDir;
class QComboBox;
class AppWindow;

namespace Core
{
class Runner;
class Compiler;
class Checker;
} // namespace Core

namespace Widgets
{
class StressTesting : public QMainWindow
{
    Q_OBJECT

  public:
    enum SourceType
    {
        Generator,
        User,
        Standard
    };
    explicit StressTesting(QWidget *parent = nullptr);
    void onCheckFinished(TestCase::Verdict verdict);

  private:
    MainWindow *mainWindow;
    AppWindow *appWindow;
    PathItem *generatorPath = nullptr, *stdPath = nullptr;
    QLabel *generatorLable = nullptr, *stdLabel = nullptr, *argumentsPatternLabel = nullptr;
    QLineEdit *argumentsPattern = nullptr;
    QPushButton *startButton = nullptr, *stopButton = nullptr;
    QCheckBox *continueAfterCountertest = nullptr, *addCountertest = nullptr;
    QComboBox *generatorSelection = nullptr, *stdSelection = nullptr;
    QVector<QPair<unsigned long long, unsigned long long>> argumentsRange;
    QVector<unsigned long long> currentValue;
    Core::Runner *generatorRunner = nullptr;
    Core::Runner *userRunner = nullptr;
    Core::Runner *stdRunner = nullptr;
    Core::Compiler *generatorCompiler = nullptr;
    Core::Compiler *userCompiler = nullptr;
    Core::Compiler *stdCompiler = nullptr;
    Core::Checker *checker = nullptr;
    MessageLogger *log = nullptr;
    QTemporaryDir *tmpDir = nullptr;
    QString generatorTmpPath;
    QString userTmpPath;
    QString stdTmpPath;
    QString userOut;
    QString stdOut;
    QString pattern;
    QString in;

    QString generatorLang;
    QString userLang;
    QString stdLang;

    int pendingCompilationCount;
    int pendingRunCount;
    int argumentsCount;
    std::atomic<bool> stopping;

    static QString getHead(SourceType type);
    static QString getComboBoxPlaceholder(int type);
    void showEvent(QShowEvent *event);
  signals:
    void compilationErrorOccurred(const QString &error);
    void compilationFailed(const QString &reason);
    void compilationKilled();

  private slots:
    void start();
    void stop();
    void nextTest();
    void onRunFinished(int type, const QString &out, const QString &err, int exitCode, qint64 timeUsed, bool tle);
    void onRunOutputLimitExceeded(int type);
    void onRunKilled(int type);
};
} // namespace Widgets

#endif // STRESSTESTING_HPP

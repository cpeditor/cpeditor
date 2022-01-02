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

#include "Widgets/StressTesting.hpp"
#include "Core/Checker.hpp"
#include "Core/Compiler.hpp"
#include "Core/EventLogger.hpp"
#include "Core/MessageLogger.hpp"
#include "Core/Runner.hpp"
#include "Settings/PathItem.hpp"
#include "Util/FileUtil.hpp"
#include "Widgets/TestCase.hpp"
#include "generated/SettingsHelper.hpp"
#include "mainwindow.hpp"
#include <QCheckBox>
#include <QCodeEditor>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTemporaryDir>
#include <QVBoxLayout>
#include <functional>

namespace Widgets
{
StressTesting::StressTesting(QWidget *parent)
    : QMainWindow(parent), mainWindow(qobject_cast<MainWindow *>(parent)), compiledCount(0), runFinishedCount(0),
      argumentsCount(0), stopping(false)
{
    log = mainWindow->getLogger();

    auto *widget = new QWidget(this);
    auto *layout = new QVBoxLayout();
    widget->setLayout(layout);
    setCentralWidget(widget);
    setWindowTitle(tr("Stress Testing"));
    resize(400, 360);

    auto *generatorLayout = new QHBoxLayout();
    generatorLable = new QLabel(tr("Generator Path:"), widget);
    generatorLayout->addWidget(generatorLable);
    generatorPath = new PathItem(PathItem::CppSource, widget);
    generatorLayout->addWidget(generatorPath);

    layout->addLayout(generatorLayout);

    auto *argumentsPatternLayout = new QHBoxLayout();
    argumentsPatternLabel = new QLabel(tr("Generator Arguments Pattern:"));
    argumentsPatternLayout->addWidget(argumentsPatternLabel);
    argumentsPattern = new QLineEdit(widget);
    argumentsPattern->setPlaceholderText(tr("Example: \"10 [5..100] abacaba\""));
    argumentsPatternLayout->addWidget(argumentsPattern);

    layout->addLayout(argumentsPatternLayout);

    auto *stdLayout = new QHBoxLayout();
    stdLabel = new QLabel(tr("Standard Program Path:"), widget);
    stdLayout->addWidget(stdLabel);
    stdPath = new PathItem(PathItem::AnyFile, widget);
    stdLayout->addWidget(stdPath);

    layout->addLayout(stdLayout);

    auto *controlLayout = new QHBoxLayout();
    startButton = new QPushButton(tr("Start"));
    connect(startButton, &QPushButton::clicked, this, &StressTesting::start);
    controlLayout->addWidget(startButton);
    stopButton = new QPushButton(tr("Stop"));
    connect(stopButton, &QPushButton::clicked, this, &StressTesting::stop);
    stopButton->setDisabled(true);
    controlLayout->addWidget(stopButton);

    layout->addLayout(controlLayout);

    continueAfterCountertest = new QCheckBox(tr("Countinue after found countertest"));
    layout->addWidget(continueAfterCountertest);

    addCountertest = new QCheckBox(tr("Add countertest to testcases"));
    layout->addWidget(addCountertest);
}

void StressTesting::start()
{
    stop();

    startButton->setDisabled(true);
    stopButton->setDisabled(false);

    QString source = argumentsPattern->text();

    pattern = "";
    argumentsCount = 0;

    int leftBracketPos = -1;
    int currentPos = 0;
    bool ok = true;
    for (auto &&c : source)
    {
        if (c == '[')
        {
            if (leftBracketPos != -1)
            {
                ok = false;
                break;
            }
            leftBracketPos = currentPos;
        }
        else if (c == ']')
        {
            if (leftBracketPos == -1)
            {
                ok = false;
                break;
            }
            argumentsCount++;
            pattern += "%";
            pattern += QString::number(argumentsCount);
            QString range = source.mid(leftBracketPos + 1, currentPos - leftBracketPos - 1);
            QStringList tmp = range.split("..");
            if (tmp.length() != 2)
            {
                ok = false;
                break;
            }

            unsigned long long left = tmp[0].toULongLong(&ok);
            if (!ok)
                break;
            unsigned long long right = tmp[1].toULongLong(&ok);
            if (!ok)
                break;

            argumentsRange.append(qMakePair(left, right));

            currentValue.push_back(argumentsCount == 1 ? left - 1 : left);

            leftBracketPos = -1;
        }
        else if (leftBracketPos == -1)
        {
            pattern += c;
        }
        currentPos++;
    }

    ok &= (leftBracketPos == -1);

    if (!ok)
    {
        log->error(tr("Stress Testing"), tr("Invalid arguments pattern"));
        return;
    }

    if (argumentsCount == 0)
    {
        currentValue.push_back(0);
        argumentsRange.push_back(qMakePair(0, 1));
    }

    QString generatorCode = Util::readFile(generatorPath->getLineEdit()->text(), tr("Read Generator"), log);
    QString userCode = mainWindow->getEditor()->toPlainText();
    QString stdCode = Util::readFile(stdPath->getLineEdit()->text(), tr("Read Standard Program"), log);

    if (generatorCode.isNull())
    {
        log->error(tr("Stress Testing"), tr("Failed to open generator"));
        return;
    }

    if (stdCode.isNull())
    {
        log->error(tr("Stress Testing"), tr("Failed to open standard program"));
        return;
    }

    tmpDir = new QTemporaryDir();

    if (!tmpDir->isValid())
    {
        log->error(tr("Stress Testing"), tr("Failed to create temporary directory"));
        return;
    }

    generatorTmpPath = tmpDir->filePath("gen.cpp");
    userTmpPath = tmpDir->filePath("user.cpp");
    stdTmpPath = tmpDir->filePath("std.cpp");

    if (!Util::saveFile(generatorTmpPath, generatorCode, tr("Stress Testing"), false, log) ||
        !Util::saveFile(userTmpPath, userCode, tr("Stress Testing"), false, log) ||
        !Util::saveFile(stdTmpPath, stdCode, tr("Stress Testing"), false, log))
        return;

    auto testlib_h = Util::readFile(":/testlib/testlib.h", tr("Read testlib.h"), log);
    if (testlib_h.isNull())
        return;
    if (!Util::saveFile(tmpDir->filePath("testlib.h"), testlib_h, tr("Save testlib.h"), false, log))
        return;

    compiledCount = 0;

    generatorCompiler = new Core::Compiler();

    connect(generatorCompiler, &Core::Compiler::compilationStarted, this,
            &StressTesting::onGeneratorCompilationStarted);
    connect(generatorCompiler, &Core::Compiler::compilationFinished, this,
            &StressTesting::onGeneratorCompilationFinished);
    connect(generatorCompiler, &Core::Compiler::compilationErrorOccurred, this,
            &StressTesting::onCompilationErrorOccurred);
    connect(generatorCompiler, &Core::Compiler::compilationFailed, this, &StressTesting::onCompilationFailed);
    connect(generatorCompiler, &Core::Compiler::compilationKilled, this, &StressTesting::onCompilationKilled);
    generatorCompiler->start(generatorTmpPath, "", SettingsHelper::getCppCompileCommand(), "C++");

    userCompiler = new Core::Compiler();

    connect(userCompiler, &Core::Compiler::compilationStarted, this, &StressTesting::onUserCompilationStarted);
    connect(userCompiler, &Core::Compiler::compilationFinished, this, &StressTesting::onUserCompilationFinished);
    connect(userCompiler, &Core::Compiler::compilationErrorOccurred, this, &StressTesting::onCompilationErrorOccurred);
    connect(userCompiler, &Core::Compiler::compilationFailed, this, &StressTesting::onCompilationFailed);
    connect(userCompiler, &Core::Compiler::compilationKilled, this, &StressTesting::onCompilationKilled);
    userCompiler->start(userTmpPath, "", mainWindow->compileCommand(), mainWindow->getLanguage());

    stdCompiler = new Core::Compiler();

    connect(stdCompiler, &Core::Compiler::compilationStarted, this, &StressTesting::onStdCompilationStarted);
    connect(stdCompiler, &Core::Compiler::compilationFinished, this, &StressTesting::onStdCompilationFinished);
    connect(stdCompiler, &Core::Compiler::compilationErrorOccurred, this, &StressTesting::onCompilationErrorOccurred);
    connect(stdCompiler, &Core::Compiler::compilationFailed, this, &StressTesting::onCompilationFailed);
    connect(stdCompiler, &Core::Compiler::compilationKilled, this, &StressTesting::onCompilationKilled);
    stdCompiler->start(stdTmpPath, "", mainWindow->compileCommand(), mainWindow->getLanguage());
}

void StressTesting::onGeneratorCompilationStarted()
{
    log->info(tr("Compiler"), tr("Generator compilation has started"));
}

void StressTesting::onGeneratorCompilationFinished()
{
    log->info(tr("Compiler"), tr("Generator compilation has finished"));
    compiledCount++;
    if (compiledCount == 3)
    {
        nextTest();
    }
}

void StressTesting::onUserCompilationStarted()
{
    log->info(tr("Compiler"), tr("User program compilation has started"));
}

void StressTesting::onUserCompilationFinished()
{
    log->info(tr("Compiler"), tr("User program compilation has finished"));
    compiledCount++;
    if (compiledCount == 3)
    {
        nextTest();
    }
}

void StressTesting::onStdCompilationStarted()
{
    log->info(tr("Compiler"), tr("Standard program compilation has started"));
}

void StressTesting::onStdCompilationFinished()
{
    log->info(tr("Compiler"), tr("Standard program compilation has finished"));
    compiledCount++;
    if (compiledCount == 3)
    {
        nextTest();
    }
}

void StressTesting::stop()
{
    if (stopping)
        return;
    stopping = true;

    delete generatorRunner;
    delete userRunner;
    delete stdRunner;
    delete generatorCompiler;
    delete userCompiler;
    delete stdCompiler;

    argumentsRange.clear();
    currentValue.clear();

    startButton->setDisabled(false);
    stopButton->setDisabled(true);

    generatorRunner = nullptr, userRunner = nullptr, stdRunner = nullptr, generatorCompiler = nullptr,
    userCompiler = nullptr, stdCompiler = nullptr;

    stopping = false;
}

void StressTesting::nextTest()
{
    for (int i = 0; i <= argumentsCount; i++)
    {
        if (i == argumentsCount && (argumentsCount != 0 || currentValue[0]))
        {
            stop();
            log->info(tr("Stress Testing"), tr("All tests finished, no countertest found"));
            return;
        }
        if (currentValue[i] != argumentsRange[i].second)
        {
            currentValue[i]++;
            break;
        }
        currentValue[i] = argumentsRange[i].first;
    }

    QString arguments = pattern;

    for (int i = 0; i < argumentsCount; i++)
        arguments = arguments.arg(QString::number(currentValue[i]));

    log->info(tr("Stress Testing"), tr("Running with arguments \"%1\"").arg(arguments));

    runFinishedCount = 0;

    generatorRunner = new Core::Runner(0);
    connect(generatorRunner, &Core::Runner::runFinished, this, &StressTesting::onRunFinished);
    connect(generatorRunner, &Core::Runner::runOutputLimitExceeded, this, &StressTesting::onRunOutputLimitExceeded);
    connect(generatorRunner, &Core::Runner::runKilled, this, &StressTesting::onRunKilled);
    generatorRunner->run(generatorTmpPath, "", "C++", "", arguments, "", SettingsHelper::getDefaultTimeLimit());
}

void StressTesting::onCompilationErrorOccurred(const QString &error)
{
    stop();
    emit compilationErrorOccurred(error);
}

void StressTesting::onCompilationFailed(const QString &reason)
{
    stop();
    emit compilationFailed(reason);
}

void StressTesting::onCompilationKilled()
{
    stop();
    emit compilationKilled();
}

void StressTesting::onRunFinished(int index, const QString &out, const QString & /*unused*/, int exitCode,
                                  qint64 timeUsed, bool tle)
{
    QString head;
    if (index == 0)
    {
        head = tr("Generator");
    }
    else if (index == 1)
    {
        head = tr("User program");
    }
    else if (index == 2)
    {
        head = tr("Standard program");
    }

    if (exitCode == 0)
    {
        log->info(head, tr("Execution has finished in %1ms").arg(timeUsed));
        if (index == 0)
        { // Generator

            in = out;

            QString language = mainWindow->getLanguage();
            stdRunner = new Core::Runner(1);
            connect(stdRunner, &Core::Runner::runFinished, this, &StressTesting::onRunFinished);
            connect(stdRunner, &Core::Runner::runOutputLimitExceeded, this, &StressTesting::onRunOutputLimitExceeded);
            connect(stdRunner, &Core::Runner::runKilled, this, &StressTesting::onRunKilled);
            stdRunner->run(stdTmpPath, "", language,
                           SettingsManager::get(QString("%1/Run Command").arg(language)).toString(),
                           SettingsManager::get(QString("%1/Run Arguments").arg(language)).toString(), in,
                           mainWindow->timeLimit());

            userRunner = new Core::Runner(2);
            connect(userRunner, &Core::Runner::runFinished, this, &StressTesting::onRunFinished);
            connect(userRunner, &Core::Runner::runOutputLimitExceeded, this, &StressTesting::onRunOutputLimitExceeded);
            connect(userRunner, &Core::Runner::runKilled, this, &StressTesting::onRunKilled);
            userRunner->run(userTmpPath, "", language,
                            SettingsManager::get(QString("%1/Run Command").arg(language)).toString(),
                            SettingsManager::get(QString("%1/Run Arguments").arg(language)).toString(), in,
                            mainWindow->timeLimit());
        }
        else
        {
            runFinishedCount++;
            if (index == 1)
                stdOut = out;
            else
                userOut = out;
            if (runFinishedCount == 2)
            {
                mainWindow->getChecker()->reqeustCheck(-1, in, userOut, stdOut);
            }
        }
    }
    else
    {
        if (tle)
        {
            log->warn(head, tr("Time Limit Exceeded"));
        }
        log->error(head, tr("Execution has finished with non-zero exitcode %1 in %2ms").arg(exitCode).arg(timeUsed));

        stop();
    }
}

void StressTesting::onRunKilled(int index)
{
    QString head;
    if (index == 0)
    {
        head = tr("Generator");
    }
    else if (index == 1)
    {
        head = tr("User program");
    }
    else if (index == 2)
    {
        head = tr("Standard program");
    }

    log->error(head, tr("The program was killed"));

    stop();
}

void StressTesting::onRunOutputLimitExceeded(int index)
{
    QString head;
    if (index == 0)
    {
        head = tr("Generator");
    }
    else if (index == 1)
    {
        head = tr("User program");
    }
    else if (index == 2)
    {
        head = tr("Standard program");
    }

    log->warn(head, tr("Output limit exceeded"));

    stop();
}

void StressTesting::onCheckFinished(TestCase::Verdict verdict)
{
    if (verdict == TestCase::Verdict::AC)
    {
        log->message(tr("Stress Testing"), tr("Accepted"), "green");
        nextTest();
    }
    else
    {
        log->message(tr("Stress Testing"), tr("Wrong Answer"), "red");
        if (continueAfterCountertest->checkState())
        {
            nextTest();
        }
        else
        {
            stop();
        }
    }
}

} // namespace Widgets
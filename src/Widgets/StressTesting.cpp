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
#include "Editor/CodeEditor.hpp"
#include "Settings/PathItem.hpp"
#include "Util/FileUtil.hpp"
#include "Widgets/TestCase.hpp"
#include "Widgets/TestCases.hpp"
#include "appwindow.hpp"
#include "generated/SettingsHelper.hpp"
#include "mainwindow.hpp"
#include <QCheckBox>
#include <QComboBox>
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
    : QMainWindow(parent), mainWindow(qobject_cast<MainWindow *>(parent)),
      appWindow(qobject_cast<AppWindow *>(parent->parentWidget())), pendingCompilationCount(0), pendingRunCount(0),
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
    connect(generatorPath, &PathItem::pathChanged, this, [this]() { generatorSelection->setCurrentIndex(0); });
    generatorLayout->addWidget(generatorPath);

    generatorSelection = new QComboBox(widget);
    generatorSelection->addItem(getComboBoxPlaceholder(SourceType::Generator));
    connect(generatorSelection, &QComboBox::currentTextChanged, this, [this](const QString &text) {
        if (text == getComboBoxPlaceholder(SourceType::Generator))
            return;
        generatorPath->getLineEdit()->setText(text);
    });
    generatorLayout->addWidget(generatorSelection);

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
    connect(stdPath, &PathItem::pathChanged, this, [this]() { stdSelection->setCurrentIndex(0); });
    stdLayout->addWidget(stdPath);

    stdSelection = new QComboBox(widget);
    stdSelection->addItem(getComboBoxPlaceholder(SourceType::Standard));
    connect(stdSelection, &QComboBox::currentTextChanged, this, [this](const QString &text) {
        if (text == getComboBoxPlaceholder(SourceType::Standard))
            return;
        stdPath->getLineEdit()->setText(text);
    });
    stdLayout->addWidget(stdSelection);

    layout->addLayout(stdLayout);

    continueAfterCountertest = new QCheckBox(tr("Countinue after found countertest"));
    layout->addWidget(continueAfterCountertest);

    addCountertest = new QCheckBox(tr("Add countertest to testcases"));
    layout->addWidget(addCountertest);

    auto *controlLayout = new QHBoxLayout();
    startButton = new QPushButton(tr("Start"));
    connect(startButton, &QPushButton::clicked, this, &StressTesting::start);
    controlLayout->addWidget(startButton);
    stopButton = new QPushButton(tr("Stop"));
    connect(stopButton, &QPushButton::clicked, this, &StressTesting::stop);
    stopButton->setDisabled(true);
    controlLayout->addWidget(stopButton);

    layout->addLayout(controlLayout);
}

void StressTesting::showEvent(QShowEvent *event)
{

    generatorSelection->clear();
    generatorSelection->addItem(getComboBoxPlaceholder(SourceType::Generator));

    auto tabs = appWindow->getTabs();
    for (auto &&tab : tabs)
    {
        if (tab->getLanguage() == "C++")
            generatorSelection->addItem(tab->getFilePath());
    }

    stdSelection->clear();
    stdSelection->addItem(getComboBoxPlaceholder(SourceType::Standard));

    for (auto &&tab : tabs)
    {
        if (tab->getLanguage() == "C++")
            stdSelection->addItem(tab->getFilePath());
    }

    QMainWindow::showEvent(event);
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

    pendingCompilationCount = 0;

    generatorCompiler = new Core::Compiler();
    userCompiler = new Core::Compiler();
    stdCompiler = new Core::Compiler();

    auto compilerName = [this](Core::Compiler *comp) -> QString {
        if (comp == generatorCompiler)
            return tr("Generator");
        if (comp == userCompiler)
            return tr("User program");
        if (comp == stdCompiler)
            return tr("Standard program");
        return QString();
    };

    auto compilationStartedHandler = [this, compilerName]() {
        auto *comp = qobject_cast<Core::Compiler *>(sender());
        log->info(tr("Compiler"), tr("%1 compilation has started").arg(compilerName(comp)));
    };

    auto compilationFinishedHandler = [this, compilerName]() {
        auto *comp = qobject_cast<Core::Compiler *>(sender());
        log->info(tr("Compiler"), tr("%1 compilation has finished").arg(compilerName(comp)));
        if (--pendingCompilationCount == 0)
        {
            nextTest();
        }
    };

    auto compilationErrorHandler = [this, compilerName](const QString &err) {
        auto *comp = qobject_cast<Core::Compiler *>(sender());
        log->error(tr("Compiler"), tr("%1 compilation error: %2").arg(compilerName(comp), err));
        stop();
        emit compilationErrorOccurred(err);
    };

    auto compilationFailedHandler = [this, compilerName](const QString &reason) {
        auto *comp = qobject_cast<Core::Compiler *>(sender());
        log->error(tr("Compiler"), tr("%1 compilation failed: %2").arg(compilerName(comp), reason));
        stop();
        emit compilationFailed(reason);
    };

    auto compilationKilledHandler = [this, compilerName]() {
        auto *comp = qobject_cast<Core::Compiler *>(sender());
        log->error(tr("Compiler"), tr("%1 compilation killed").arg(compilerName(comp)));
        stop();
        emit compilationKilled();
    };

    auto initCompiler = [this, compilationStartedHandler, compilationFinishedHandler, compilationErrorHandler,
                         compilationFailedHandler, compilationKilledHandler](Core::Compiler *compiler) {
        connect(compiler, &Core::Compiler::compilationStarted, this, compilationStartedHandler);
        connect(compiler, &Core::Compiler::compilationFinished, this, compilationFinishedHandler);
        connect(compiler, &Core::Compiler::compilationErrorOccurred, this, compilationErrorHandler);
        connect(compiler, &Core::Compiler::compilationFailed, this, compilationFailedHandler);
        connect(compiler, &Core::Compiler::compilationKilled, this, compilationKilledHandler);
        pendingCompilationCount++;
    };
    initCompiler(generatorCompiler);
    initCompiler(userCompiler);
    initCompiler(stdCompiler);

    generatorCompiler->start(generatorTmpPath, "", SettingsHelper::getCppCompileCommand(), "C++");
    userCompiler->start(userTmpPath, "", mainWindow->compileCommand(), mainWindow->getLanguage());
    stdCompiler->start(stdTmpPath, "", mainWindow->compileCommand(), mainWindow->getLanguage());
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
            log->info(tr("Stress Testing"), tr("All tests finished"));
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

    pendingRunCount = 0;

    generatorRunner = new Core::Runner(SourceType::Generator);
    connect(generatorRunner, &Core::Runner::runFinished, this, &StressTesting::onRunFinished);
    connect(generatorRunner, &Core::Runner::runOutputLimitExceeded, this, &StressTesting::onRunOutputLimitExceeded);
    connect(generatorRunner, &Core::Runner::runKilled, this, &StressTesting::onRunKilled);
    generatorRunner->run(generatorTmpPath, "", "C++", "", arguments, "", SettingsHelper::getDefaultTimeLimit());
}

void StressTesting::onRunFinished(int type, const QString &out, const QString & /*unused*/, int exitCode,
                                  qint64 timeUsed, bool tle)
{
    QString head = getHead((StressTesting::SourceType)type);

    if (exitCode == 0)
    {
        log->info(head, tr("Execution has finished in %1ms").arg(timeUsed));
        if (type == SourceType::Generator)
        { // Generator

            in = out;

            QString language = mainWindow->getLanguage();

            stdRunner = new Core::Runner(SourceType::Standard);
            userRunner = new Core::Runner(SourceType::User);

            auto initRunner = [this](Core::Runner *runner) {
                pendingRunCount++;
                connect(runner, &Core::Runner::runFinished, this, &StressTesting::onRunFinished);
                connect(runner, &Core::Runner::runOutputLimitExceeded, this, &StressTesting::onRunOutputLimitExceeded);
                connect(runner, &Core::Runner::runKilled, this, &StressTesting::onRunKilled);
            };

            initRunner(stdRunner);
            initRunner(userRunner);

            stdRunner->run(stdTmpPath, "", language,
                           SettingsManager::get(QString("%1/Run Command").arg(language)).toString(),
                           SettingsManager::get(QString("%1/Run Arguments").arg(language)).toString(), in,
                           mainWindow->timeLimit());
            userRunner->run(userTmpPath, "", language,
                            SettingsManager::get(QString("%1/Run Command").arg(language)).toString(),
                            SettingsManager::get(QString("%1/Run Arguments").arg(language)).toString(), in,
                            mainWindow->timeLimit());
        }
        else
        {
            if (type == SourceType::Standard)
                stdOut = out;
            else if (type == SourceType::User)
                userOut = out;
            if (--pendingRunCount == 0)
            {
                mainWindow->getChecker()->requestCheck(-1, in, userOut, stdOut);
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

void StressTesting::onRunKilled(int type)
{
    QString head = getHead((StressTesting::SourceType)type);

    log->error(head, tr("The program was killed"));

    stop();
}

void StressTesting::onRunOutputLimitExceeded(int type)
{
    QString head = getHead((StressTesting::SourceType)type);

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
        if (addCountertest->checkState() == Qt::CheckState::Checked)
        {
            mainWindow->getTestCases()->addTestCase(in, stdOut);
        }
        if (continueAfterCountertest->checkState() == Qt::CheckState::Checked)
        {
            nextTest();
        }
        else
        {
            stop();
        }
    }
}

QString StressTesting::getHead(StressTesting::SourceType type)
{
    if (type == StressTesting::SourceType::Generator)
    {
        return tr("Generator");
    }
    if (type == StressTesting::SourceType::User)
    {
        return tr("User program");
    }
    if (type == StressTesting::SourceType::Standard)
    {
        return tr("Standard program");
    }
    return QString();
}

QString StressTesting::getComboBoxPlaceholder(int type)
{
    if (type == SourceType::Generator)
    {
        return tr("Select generator from editor tabs...");
    }
    if (type == SourceType::Standard)
    {
        return tr("Select standard program from editor tabs...");
    }
    return QString();
}

} // namespace Widgets
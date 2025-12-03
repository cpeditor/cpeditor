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
#include <QAbstractButton>
#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QProgressBar>
#include <QPushButton>
#include <QTemporaryDir>
#include <QVBoxLayout>
#include <limits>

namespace
{
constexpr QChar separator(0x1D);
}

namespace Widgets
{
StressTesting::StressTesting(QWidget *parent)
    : QMainWindow(parent), mainWindow(qobject_cast<MainWindow *>(parent)),
      appWindow(qobject_cast<AppWindow *>(parent->parentWidget())), errorVerdict(TestCase::UNKNOWN), totalTests(0),
      executedTests(0), pendingCompilationCount(0), pendingRunCount(0), argumentsCount(0), stopping(false)
{
    log = mainWindow->getLogger();

    auto *widget = new QWidget(this);
    auto *layout = new QVBoxLayout();
    widget->setLayout(layout);
    setCentralWidget(widget);
    setWindowTitle(tr("Stress Testing"));

    tabTitleLabel = new QLabel(tr("User Program: %1").arg(mainWindow->getTabTitle(false, false)), widget);
    layout->addWidget(tabTitleLabel);

    auto *generatorLayout = new QHBoxLayout();

    generatorLable = new QLabel(tr("Generator Path:"), widget);
    generatorLayout->addWidget(generatorLable);

    generatorPath = new PathItem(PathItem::AnyFile, widget);
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

    auto *controlLayout = new QHBoxLayout();
    startButton = new QPushButton(tr("Start"));
    connect(startButton, &QPushButton::clicked, this, &StressTesting::start);
    controlLayout->addWidget(startButton);
    stopButton = new QPushButton(tr("Stop"));
    connect(stopButton, &QPushButton::clicked, this, &StressTesting::stop);
    stopButton->setDisabled(true);
    controlLayout->addWidget(stopButton);

    layout->addLayout(controlLayout);

    progressBar = new QProgressBar(widget);
    progressBar->setRange(0, 1);
    progressBar->setValue(0);
    layout->addWidget(progressBar);
}

void StressTesting::showEvent(QShowEvent *event)
{

    generatorSelection->clear();
    generatorSelection->addItem(getComboBoxPlaceholder(SourceType::Generator));

    auto tabs = appWindow->getTabs();
    for (auto &&tab : tabs)
    {
        if (!tab->isUntitled())
        {
            generatorSelection->addItem(tab->getFilePath());
        }
    }

    stdSelection->clear();
    stdSelection->addItem(getComboBoxPlaceholder(SourceType::Standard));

    for (auto &&tab : tabs)
    {
        if (!tab->isUntitled())
        {
            stdSelection->addItem(tab->getFilePath());
        }
    }

    tabTitleLabel->setText(tr("User Program: %1").arg(mainWindow->getTabTitle(false, false)));

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
            pattern += separator;
            QString range = source.mid(leftBracketPos + 1, currentPos - leftBracketPos - 1);
            QStringList tmp = range.split("..");
            if (tmp.length() != 2)
            {
                ok = false;
                break;
            }

            long long left = tmp[0].toLongLong(&ok);
            if (!ok)
                break;
            long long right = tmp[1].toLongLong(&ok);
            if (!ok)
                break;

            if (left > right)
            {
                ok = false;
                break;
            }

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
        stop();
        return;
    }

    if (argumentsCount == 0)
    {
        currentValue.push_back(0);
        argumentsRange.push_back(qMakePair(0, 1));
    }

    executedTests = 0;
    totalTests = 0;

    if (argumentsCount == 0)
    {
        totalTests = 1;
    }
    else
    {
        unsigned long long total = 1;
        bool overflow = false;
        constexpr unsigned long long limit = std::numeric_limits<int>::max();
        for (int i = 0; i < argumentsCount; ++i)
        {
            const auto &range = argumentsRange[i];
            const unsigned long long length = static_cast<unsigned long long>(range.second) - range.first + 1;
            if (total > limit / length)
            {
                overflow = true;
                break;
            }
            total *= length;
        }
        if (!overflow)
        {
            totalTests = total;
        }
    }

    if (totalTests > 0)
    {
        progressBar->setRange(0, static_cast<int>(totalTests));
        progressBar->setValue(0);
    }
    else
    {
        progressBar->setRange(0, 0);
    }

    QString generatorCode = Util::readFile(generatorPath->getLineEdit()->text(), tr("Read Generator"), log);
    QString userCode = mainWindow->getEditor()->toPlainText();
    QString stdCode = Util::readFile(stdPath->getLineEdit()->text(), tr("Read Standard Program"), log);

    if (generatorCode.isNull())
    {
        log->error(tr("Stress Testing"), tr("Failed to open generator"));
        stop();
        return;
    }

    if (stdCode.isNull())
    {
        log->error(tr("Stress Testing"), tr("Failed to open standard program"));
        stop();
        return;
    }

    tmpDir = new QTemporaryDir();

    if (!tmpDir->isValid())
    {
        log->error(tr("Stress Testing"), tr("Failed to create temporary directory"));
        stop();
        return;
    }

    auto getLanguage = [&](const QString &path) -> QString {
        QString lang = SettingsHelper::getDefaultLanguage();
        auto suffix = QFileInfo(path).suffix();
        if (Util::cppSuffix.contains(suffix))
            lang = "C++";
        else if (Util::javaSuffix.contains(suffix))
            lang = "Java";
        else if (Util::pythonSuffix.contains(suffix))
            lang = "Python";
        return lang;
    };

    auto getSuffix = [&](const QString &language) -> QString {
        if (language == "C++")
            return "cpp";
        if (language == "Java")
            return "java";
        if (language == "Python")
            return "py";
        return QString();
    };

    generatorLang = getLanguage(generatorPath->getLineEdit()->text());
    userLang = mainWindow->getLanguage();
    stdLang = getLanguage(stdPath->getLineEdit()->text());

    generatorTmpPath = tmpDir->filePath("gen." + getSuffix(generatorLang));
    userTmpPath = tmpDir->filePath("user." + getSuffix(userLang));
    stdTmpPath = tmpDir->filePath("std." + getSuffix(stdLang));

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

    if (generatorLang != "Python")
        initCompiler(generatorCompiler);
    if (userLang != "Python")
        initCompiler(userCompiler);
    if (stdLang != "Python")
        initCompiler(stdCompiler);

    if (generatorLang != "Python")
        generatorCompiler->start(generatorTmpPath, "",
                                 SettingsManager::get(QString("%1/Compile Command").arg(generatorLang)).toString(),
                                 generatorLang);
    if (userLang != "Python")
        userCompiler->start(userTmpPath, "",
                            SettingsManager::get(QString("%1/Compile Command").arg(userLang)).toString(), userLang);
    if (stdLang != "Python")
        stdCompiler->start(stdTmpPath, "", SettingsManager::get(QString("%1/Compile Command").arg(stdLang)).toString(),
                           stdLang);

    if (generatorLang == "Python" && userLang == "Python" && stdLang == "Python")
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

    totalTests = 0;
    executedTests = 0;

    if (progressBar->maximum() == 0)
    {
        progressBar->setRange(0, 1);
        progressBar->setValue(1);
    }

    stopping = false;

    errorVerdict = TestCase::UNKNOWN;
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

    arguments.remove(separator);

    log->info(tr("Stress Testing"), tr("Running with arguments \"%1\"").arg(arguments));

    pendingRunCount = 0;

    generatorRunner = new Core::Runner(SourceType::Generator);
    connect(generatorRunner, &Core::Runner::runFinished, this, &StressTesting::onRunFinished);
    connect(generatorRunner, &Core::Runner::runOutputLimitExceeded, this, &StressTesting::onRunOutputLimitExceeded);
    connect(generatorRunner, &Core::Runner::runKilled, this, &StressTesting::onRunKilled);
    generatorRunner->run(generatorTmpPath, "", generatorLang,
                         SettingsManager::get(QString("%1/Run Command").arg(generatorLang)).toString(), arguments, "",
                         SettingsHelper::getDefaultTimeLimit());
}

void StressTesting::onRunFinished(int type, const QString &out, const QString & /*unused*/, int exitCode,
                                  qint64 timeUsed, bool tle)
{
    QString head = getHead((StressTesting::SourceType)type);

    if (exitCode == 0)
    {
        if (type == SourceType::Generator)
        { // Generator

            in = out;

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

            stdRunner->run(
                stdTmpPath, "", stdLang, SettingsManager::get(QString("%1/Run Command").arg(stdLang)).toString(),
                SettingsManager::get(QString("%1/Run Arguments").arg(stdLang)).toString(), in, mainWindow->timeLimit());
            userRunner->run(userTmpPath, "", userLang,
                            SettingsManager::get(QString("%1/Run Command").arg(userLang)).toString(),
                            SettingsManager::get(QString("%1/Run Arguments").arg(userLang)).toString(), in,
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
                if (errorVerdict != TestCase::UNKNOWN)
                {
                    this->onCheckFinished(errorVerdict);
                    return;
                }
                mainWindow->getChecker()->requestCheck(-1, in, userOut, stdOut);
            }
        }
    }
    else
    {
        if (type != SourceType::User)
        {
            if (tle)
            {
                log->warn(head, tr("Time Limit Exceeded"));
            }
            log->error(head,
                       tr("Execution has finished with non-zero exitcode %1 in %2ms").arg(exitCode).arg(timeUsed));

            stop();
        }
        else
        {
            if (tle)
            {
                errorVerdict = TestCase::TLE;
            }
            else
            {
                errorVerdict = TestCase::RE;
            }
            userOut = out;
            if (--pendingRunCount == 0)
            {
                this->onCheckFinished(errorVerdict);
            }
        }
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
    if (progressBar->maximum() != 0)
    {
        executedTests++;
        progressBar->setValue(static_cast<int>(executedTests));
    }

    if (verdict == TestCase::Verdict::AC)
    {
        nextTest();
    }
    else
    {
        QDialog dialog(this);
        dialog.setWindowTitle(tr("Counterexample Found"));
        dialog.setWindowModality(Qt::WindowModal);
        dialog.resize(800, 480);

        auto *dialogLayout = new QVBoxLayout(&dialog);

        auto *preview = new TestCase(0, log, &dialog, in, stdOut, false);
        preview->setOutput(userOut);
        preview->setVerdict(verdict);
        preview->setTestCaseEditFont(mainWindow->getEditor()->font());
        preview->updateHeight();
        dialogLayout->addWidget(preview);

        auto *addButton = new QPushButton(tr("Add to testcases"), &dialog);
        auto *ignoreButton = new QPushButton(tr("Ignore"), &dialog);
        auto *stopCheckBox = new QCheckBox(tr("Stop stress testing"), &dialog);

        auto *controlsLayout = new QHBoxLayout();
        controlsLayout->addWidget(addButton);
        controlsLayout->addWidget(ignoreButton);
        controlsLayout->addWidget(stopCheckBox);
        controlsLayout->addStretch();

        dialogLayout->addLayout(controlsLayout);

        connect(addButton, &QAbstractButton::clicked, &dialog, &QDialog::accept);
        connect(ignoreButton, &QAbstractButton::clicked, &dialog, &QDialog::reject);

        dialog.exec();

        if (dialog.result() == QDialog::Accepted)
        {
            mainWindow->getTestCases()->addTestCase(in, stdOut);
            log->info(tr("Stress Testing"), tr("Counterexample added to testcases"));
        }

        if (stopCheckBox->isChecked())
        {
            stop();
            return;
        }

        nextTest();
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
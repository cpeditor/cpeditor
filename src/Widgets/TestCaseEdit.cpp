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

#include "Widgets/TestCaseEdit.hpp"
#include "Core/EventLogger.hpp"
#include "Core/MessageLogger.hpp"
#include "Util.hpp"
#include <QApplication>
#include <QFileDialog>
#include <QInputDialog>
#include <QMenu>
#include <QMimeData>
#include <QPropertyAnimation>
#include <QStyle>
#include <generated/SettingsHelper.hpp>

namespace Widgets
{
TestCaseEdit::TestCaseEdit(bool autoAnimation, MessageLogger *logger, const QString &text, QWidget *parent)
    : QPlainTextEdit(text, parent), log(logger)
{
    setFont(SettingsHelper::getTestCasesFont());
    animation = new QPropertyAnimation(this, "minimumHeight", this);
    if (autoAnimation)
        connect(this, SIGNAL(textChanged()), this, SLOT(startAnimation()));
    startAnimation();
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this,
            SLOT(onCustomContextMenuRequested(const QPoint &)));
}

void TestCaseEdit::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
    {
        event->accept();
        event->acceptProposedAction();
    }
}

void TestCaseEdit::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasUrls())
    {
        event->accept();
        event->acceptProposedAction();
    }
}

void TestCaseEdit::dropEvent(QDropEvent *event)
{
    LOG_INFO("Object dropped into testcase widget");
    auto urls = event->mimeData()->urls();
    if (!isReadOnly() && !urls.isEmpty())
    {
        LOG_INFO("Dropped file is " << urls[0].toLocalFile());
        loadFromFile(urls[0].toLocalFile());
        event->acceptProposedAction();
    }
}

void TestCaseEdit::modifyText(const QString &text)
{
    auto cursor = textCursor();
    cursor.select(QTextCursor::Document);
    cursor.insertText(text);
}

void TestCaseEdit::startAnimation()
{
    int newHeight = qMin(fontMetrics().lineSpacing() * (document()->lineCount() + 2) + 5, 300);
    if (newHeight != minimumHeight())
    {
        animation->stop();
        animation->setStartValue(minimumHeight());
        animation->setEndValue(newHeight);
        animation->start();
    }
}

void TestCaseEdit::onCustomContextMenuRequested(const QPoint &pos)
{
    auto menu = createStandardContextMenu();
    if (!isReadOnly())
    {
        menu->addSeparator();
        menu->addAction(QApplication::style()->standardIcon(QStyle::SP_DialogOpenButton), "Load From File", [this] {
            LOG_INFO("Opening file dialog to Load from file");
            auto res = QFileDialog::getOpenFileName(this, "Load From File");
            loadFromFile(res);
        });
        menu->addAction(
            QApplication::style()->standardIcon(QStyle::SP_TitleBarMaxButton), "Edit in Bigger Window", [this] {
                LOG_INFO("Opening for edit in big window");
                bool ok = false;
                auto res = QInputDialog::getMultiLineText(this, "Edit Testcase", QString(), toPlainText(), &ok);
                if (ok)
                    modifyText(res);
            });
    }
    menu->popup(mapToGlobal(pos));
}

void TestCaseEdit::loadFromFile(const QString &path)
{
    auto content = Util::readFile(path, "Load Testcase From File", log);
    if (!content.isNull())
    {
        if (content.length() > SettingsHelper::getLoadTestCaseFileLengthLimit())
        {
            log->error(
                "Testcases",
                QString("The testcase file [%1] contains more than %2 characters, so it's not loaded. You can change "
                        "the length limit in Preferences->Advanced->Limits->Load Test Case File Length Limit")
                    .arg(path)
                    .arg(SettingsHelper::getLoadTestCaseFileLengthLimit()));
        }
        else
            modifyText(content);
    }
}
} // namespace Widgets
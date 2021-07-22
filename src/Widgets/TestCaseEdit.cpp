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

#include "Widgets/TestCaseEdit.hpp"
#include "Core/EventLogger.hpp"
#include "Core/MessageLogger.hpp"
#include "Settings/DefaultPathManager.hpp"
#include "Util/FileUtil.hpp"
#include <QApplication>
#include <QInputDialog>
#include <QMenu>
#include <QMimeData>
#include <QPropertyAnimation>
#include <QStyle>
#include <generated/SettingsHelper.hpp>

namespace Widgets
{
TestCaseEdit::TestCaseEdit(Role role, int id, MessageLogger *logger, const QString &text, QWidget *parent)
    : QPlainTextEdit(parent), log(logger), role(role), id(id)
{
    setFont(SettingsHelper::getTestCasesFont());
    setWordWrapMode(QTextOption::NoWrap);
    modifyText(text, false);

    animation = new QPropertyAnimation(this, "minimumHeight", this);

    switch (role)
    {
    case Input:
    case Expected:
        connect(this, &TestCaseEdit::textChanged, this, &TestCaseEdit::startAnimation);
        break;
    case Output:
        setReadOnly(true);
        break;
    default:
        Q_UNREACHABLE();
    }

    startAnimation();
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &TestCaseEdit::customContextMenuRequested, this, &TestCaseEdit::onCustomContextMenuRequested);
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
    LOG_INFO(INFO_OF(id));
    auto urls = event->mimeData()->urls();
    if (role != Output && !urls.isEmpty())
    {
        LOG_INFO("Dropped file is " << urls[0].toLocalFile());
        loadFromFile(urls[0].toLocalFile());
        event->acceptProposedAction();
    }
}

void TestCaseEdit::modifyText(const QString &text, bool keepHistory)
{
    this->text = text;

    const int limit = role == Output ? SettingsHelper::getOutputDisplayLengthLimit()
                                     : SettingsHelper::getDisplayTestCaseLengthLimit();

    QString displayText;

    if (text.length() <= limit)
    {
        displayText = text;
        if (role != Output)
            setReadOnly(false);
    }
    else
    {
        LOG_INFO("Too long: " << INFO_OF(role) << INFO_OF(id) << INFO_OF(text.length()));

        setReadOnly(true);

        displayText = text.left(limit) + "...";

        const QString name = role == Input ? tr("Input") : (role == Output ? tr("Output") : tr("Expected"));
        const QString setLimitPlace = role == Output ? SettingsHelper::pathOfOutputDisplayLengthLimit()
                                                     : SettingsHelper::pathOfDisplayTestCaseLengthLimit();

        log->warn(QString("%1[%2]").arg(name).arg(id + 1),
                  tr("Only the first %1 characters are shown. Now the test case editor is read-only. You can set "
                     "the length limit at %2.")
                      .arg(limit)
                      .arg(setLimitPlace),
                  false);
    }

    if (keepHistory)
    {
        auto cursor = textCursor();
        cursor.select(QTextCursor::Document);
        cursor.insertText(displayText);
    }
    else
    {
        setPlainText(displayText);
    }
}

QString TestCaseEdit::getText()
{
    if (!isReadOnly())
        text = toPlainText();
    return text;
}

void TestCaseEdit::startAnimation()
{
    int newHeight = qMin(fontMetrics().boundingRect("f").height() * (document()->lineCount() + 2),
                         SettingsHelper::getTestCaseMaximumHeight());
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
    auto *menu = createStandardContextMenu();

    menu->addSeparator();

    menu->addAction(QApplication::style()->standardIcon(QStyle::SP_FileIcon), tr("Save to file"), [this] {
        LOG_INFO("Saving test case to file");
        QString fileName =
            DefaultPathManager::getSaveFileName("Save Test Case To A File", this, tr("Save test case to file"));
        if (!fileName.isEmpty())
            Util::saveFile(fileName, getText(), tr("Save test case to file"), true, log);
    });

    if (role != Output)
    {
        menu->addAction(QApplication::style()->standardIcon(QStyle::SP_DialogOpenButton), tr("Load From File"), [this] {
            LOG_INFO("Opening file dialog to Load from file");
            auto res = DefaultPathManager::getOpenFileName("Load Single Test Case", this, tr("Load From File"));
            if (!res.isEmpty())
                loadFromFile(res);
        });
        menu->addAction(
            QApplication::style()->standardIcon(QStyle::SP_TitleBarMaxButton), tr("Edit in Bigger Window"), [this] {
                LOG_INFO("Opening for edit in big window");
                bool ok = false;
                auto res = QInputDialog::getMultiLineText(this, tr("Edit Testcase"), QString(), getText(), &ok);
                if (ok)
                    modifyText(res);
            });
    }

    if (role == Expected)
    {
        menu->addAction(QApplication::style()->standardIcon(QStyle::SP_ArrowForward), tr("Copy Output to Expected"),
                        [this] {
                            LOG_INFO("Copy Output to Expected");
                            emit requestCopyOutputToExpected();
                        });
    }

    menu->popup(mapToGlobal(pos));
}

void TestCaseEdit::loadFromFile(const QString &path)
{
    auto content = Util::readFile(path, "Load Testcase From File", log);
    if (!content.isNull())
        modifyText(content);
}
} // namespace Widgets

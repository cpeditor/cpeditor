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
#include <QMimeData>

TestCaseEdit::TestCaseEdit(bool autoAnimation, const QString &text, QWidget *parent) : QPlainTextEdit(text, parent)
{
    Core::Log::i("testcaseEdit/constructed") << "autoAnimate " << autoAnimation << " text " << text << endl;
    animation = new QPropertyAnimation(this, "minimumHeight", this);
    if (autoAnimation)
        connect(this, SIGNAL(textChanged()), this, SLOT(startAnimation()));
    startAnimation();
}

TestCaseEdit::TestCaseEdit(bool autoAnimation, QWidget *parent) : QPlainTextEdit(parent)
{
    Core::Log::i("testcaseEdit/constructed") << "autoAnimate " << autoAnimation << endl;
    animation = new QPropertyAnimation(this, "minimumHeight", this);
    if (autoAnimation)
        connect(this, SIGNAL(textChanged()), this, SLOT(startAnimation()));
    startAnimation();
}

void TestCaseEdit::dragEnterEvent(QDragEnterEvent *event)
{
    Core::Log::i("testCaseEdit/dragEnterEvent", "Something is being dropped into testcaseEdit");
    if (event->mimeData()->hasUrls())
    {
        Core::Log::i("testCaseEdit/dragEnterEvent", "Accepting dropped object");
        event->accept();
        event->acceptProposedAction();
    }
}

void TestCaseEdit::dragMoveEvent(QDragMoveEvent *event)
{
    Core::Log::i("testCaseEdit/dragMoveEvent", "Something is being Moved into testcaseEdit");

    if (event->mimeData()->hasUrls())
    {
        Core::Log::i("testCaseEdit/dragMoveEvent", "Accepting moved object");
        event->accept();
        event->acceptProposedAction();
    }
}

void TestCaseEdit::dropEvent(QDropEvent *event)
{
    Core::Log::i("testcaseEdit/dropEvent", "Something has been dropped");
    auto urls = event->mimeData()->urls();
    if (!isReadOnly() && !urls.isEmpty())
    {
        Core::Log::i("testcaseEdit/dropEvent", "Reading the dropped file to testcase");
        QFile file(urls[0].toLocalFile());
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
            modifyText(file.readAll());
        event->acceptProposedAction();
        Core::Log::i("testcaseEdit/dropEvent") << "dropped file was " << file.fileName() << endl;
    }
}

void TestCaseEdit::modifyText(const QString &text)
{
    Core::Log::i("testcasesEdit/modifyText") << "text " << text << endl;
    auto cursor = textCursor();
    cursor.select(QTextCursor::Document);
    cursor.insertText(text);
}

void TestCaseEdit::startAnimation()
{
    Core::Log::i("testcaseEdit/startAnimation", "started Animation");
    int newHeight = qMin(fontMetrics().lineSpacing() * (document()->lineCount() + 2) + 5, 300);
    if (newHeight != minimumHeight())
    {
        animation->stop();
        animation->setStartValue(minimumHeight());
        animation->setEndValue(newHeight);
        animation->start();
        Core::Log::i("testcaseEdit/startAnimation", "re-started Animation");
    }
}

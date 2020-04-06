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

#include "Widgets/DiffViewer.hpp"
#include "Core/EventLogger.hpp"
#include "Core/SettingsHelper.hpp"
#include "diff_match_patch.h"
#include <QScrollBar>

DiffViewer::DiffViewer(QWidget *parent) : QMainWindow(parent)
{
    Core::Log::i("DiffViewer/constructor", "Invoked");

    widget = new QWidget(this);
    layout = new QHBoxLayout();
    widget->setLayout(layout);
    setCentralWidget(widget);
    setWindowTitle("Diff Viewer");
    resize(720, 480);
    Core::Log::i("DiffViewer/constructor", "Main layout is set");

    leftLayout = new QVBoxLayout();
    outputLabel = new QLabel("Output", widget);
    leftLayout->addWidget(outputLabel);
    outputEdit = new QTextEdit(widget);
    outputEdit->setReadOnly(true);
    outputEdit->setWordWrapMode(QTextOption::NoWrap);
    leftLayout->addWidget(outputEdit);
    layout->addLayout(leftLayout);
    Core::Log::i("DiffViewer/constructor", "Left layout is set");

    rightLayout = new QVBoxLayout();
    expectedLabel = new QLabel("Expected", widget);
    rightLayout->addWidget(expectedLabel);
    expectedEdit = new QTextEdit(widget);
    expectedEdit->setReadOnly(true);
    expectedEdit->setWordWrapMode(QTextOption::NoWrap);
    rightLayout->addWidget(expectedEdit);
    layout->addLayout(rightLayout);
    Core::Log::i("DiffViewer/constructor", "Right layout is set");

    connect(expectedEdit->horizontalScrollBar(), SIGNAL(valueChanged(int)), outputEdit->horizontalScrollBar(),
            SLOT(setValue(int)));
    connect(outputEdit->horizontalScrollBar(), SIGNAL(valueChanged(int)), expectedEdit->horizontalScrollBar(),
            SLOT(setValue(int)));
    connect(expectedEdit->verticalScrollBar(), SIGNAL(valueChanged(int)), outputEdit->verticalScrollBar(),
            SLOT(setValue(int)));
    connect(outputEdit->verticalScrollBar(), SIGNAL(valueChanged(int)), expectedEdit->verticalScrollBar(),
            SLOT(setValue(int)));
    Core::Log::i("DiffViewer/constructor", "Signals are connected");
}

void DiffViewer::setText(const QString &output, const QString &expected)
{
    if (output.length() <= MAX_CHARACTERS_FOR_HTML && expected.length() <= MAX_CHARACTERS_FOR_HTML)
    {
        Core::Log::i("DiffViewer/setText", "Use HTML");
        diff_match_patch differ;
        differ.Diff_EditCost = 10;
        auto diffs = differ.diff_main(output, expected);
        differ.diff_cleanupEfficiency(diffs);

        QString outputHTML, expectedHTML;
        for (auto diff : diffs)
        {
            QString text = diff.text.toHtmlEscaped().replace(" ", "&nbsp;");
            if (SettingsHelper::isDisplayEOLNInDiff())
                text.replace("\n", "&para;<br>");
            else
                text.replace("\n", "<br>");
            switch (diff.operation)
            {
            case INSERT:
                expectedHTML += QString("<ins style=\"background:#8f8;\">") + text + QString("</ins>");
                break;
            case DELETE:
                outputHTML += "<s style=\"background:#f88;\">" + text + "</s>";
                break;
            case EQUAL:
                outputHTML += "<span>" + text + "</span>";
                expectedHTML += "<span>" + text + "</span>";
                break;
            }
        }
        QPalette p = outputEdit->palette();
        p.setColor(QPalette::Base, Qt::white); // for system dark theme
        p.setColor(QPalette::Text, Qt::black);
        outputEdit->setPalette(p);
        outputEdit->setHtml(outputHTML);
        expectedEdit->setPalette(p);
        expectedEdit->setHtml(expectedHTML);
    }
    else
    {
        Core::Log::i("DiffViewer/setText", "Use plain text");
        emit toLongForHtml();
        outputEdit->setPlainText(output);
        expectedEdit->setPlainText(expected);
    }

    auto resetScrollBar = [](QScrollBar *scrollBar) { scrollBar->setValue(scrollBar->minimum()); };

    resetScrollBar(outputEdit->horizontalScrollBar());
    resetScrollBar(outputEdit->verticalScrollBar());
    resetScrollBar(expectedEdit->horizontalScrollBar());
    resetScrollBar(outputEdit->verticalScrollBar());
}

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

#include "Widgets/DiffViewer.hpp"
#include "Core/EventLogger.hpp"
#include "Core/MessageLogger.hpp"
#include "third_party/diff_match_patch/diff_match_patch.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollBar>
#include <QTextEdit>
#include <QVBoxLayout>
#include <generated/SettingsHelper.hpp>

namespace Widgets
{
DiffViewer::DiffViewer(QWidget *parent) : QMainWindow(parent)
{
    auto *widget = new QWidget(this);
    auto *layout = new QHBoxLayout();
    widget->setLayout(layout);
    setCentralWidget(widget);
    setWindowTitle(tr("Diff Viewer"));
    resize(720, 480);

    auto *leftLayout = new QVBoxLayout();
    outputLabel = new QLabel(tr("Output"), widget);
    leftLayout->addWidget(outputLabel);
    outputEdit = new QTextEdit(widget);
    outputEdit->setReadOnly(true);
    outputEdit->setWordWrapMode(QTextOption::NoWrap);
    leftLayout->addWidget(outputEdit);
    layout->addLayout(leftLayout);

    auto *rightLayout = new QVBoxLayout();
    expectedLabel = new QLabel(tr("Expected"), widget);
    rightLayout->addWidget(expectedLabel);
    expectedEdit = new QTextEdit(widget);
    expectedEdit->setReadOnly(true);
    expectedEdit->setWordWrapMode(QTextOption::NoWrap);
    rightLayout->addWidget(expectedEdit);
    layout->addLayout(rightLayout);

    connect(expectedEdit->horizontalScrollBar(), &QScrollBar::valueChanged, outputEdit->horizontalScrollBar(),
            &QScrollBar::setValue);
    connect(outputEdit->horizontalScrollBar(), &QScrollBar::valueChanged, expectedEdit->horizontalScrollBar(),
            &QScrollBar::setValue);
    connect(expectedEdit->verticalScrollBar(), &QScrollBar::valueChanged, outputEdit->verticalScrollBar(),
            &QScrollBar::setValue);
    connect(outputEdit->verticalScrollBar(), &QScrollBar::valueChanged, expectedEdit->verticalScrollBar(),
            &QScrollBar::setValue);
}

void DiffViewer::setText(const QString &output, const QString &expected)
{
    if (output.length() <= SettingsHelper::getHTMLDiffViewerLengthLimit() &&
        expected.length() <= SettingsHelper::getHTMLDiffViewerLengthLimit())
    {
        LOG_INFO("Diff viewer is using HTML Text");
        diff_match_patch differ;
        differ.Diff_EditCost = 10;
        const auto nonNullOutput = output.isNull() ? "" : output;
        const auto nonNullExpected = expected.isNull() ? "" : expected;
        auto diffs = differ.diff_main(nonNullOutput, nonNullExpected);
        differ.diff_cleanupEfficiency(diffs);

        QString outputHTML;
        QString expectedHTML;
        outputHTML = expectedHTML = "<body style='background-color: white; color: black;'>";
        for (auto const &diff : diffs)
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
        outputHTML += "</body>";
        expectedHTML += "</body>";
        outputEdit->setHtml(outputHTML);
        expectedEdit->setHtml(expectedHTML);
    }
    else
    {
        LOG_INFO("Diff viewer is using Plain Text");
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
} // namespace Widgets

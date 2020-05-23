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
#include <QDesktopServices>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <Widgets/UpdatePresenter.hpp>
#include <markdown_highlighter.h>

namespace Widgets
{

UpdatePresenter::UpdatePresenter()
{
    textEdit = new QTextEdit(this);
    textEdit->setReadOnly(true);

    name = new QLabel(this);
    mainLayout = new QVBoxLayout(this);
    subLayout = new QHBoxLayout(this);

    downloadButton = new QPushButton("Download", this);
    cancelButton = new QPushButton("Close", this);

    connect(cancelButton, &QPushButton::clicked, this, [this]() { close(); });

    subLayout->addWidget(downloadButton);
    subLayout->addWidget(cancelButton);

    mainLayout->addWidget(name);
    mainLayout->addWidget(textEdit);
    mainLayout->addLayout(subLayout);

    setLayout(mainLayout);
    // setGeometry(geometry());
    setWindowTitle("New update available");
}

void UpdatePresenter::load(Telemetry::UpdateChecker::UpdateMetaInformation meta)
{
    information = meta;

    textEdit->setText(information.body);

    delete downloadButton;
    downloadButton = new QPushButton("Download", this);
    subLayout->insertWidget(0, downloadButton);

    // Instead of delete and re create, if we can somehow disconnect the old connection and set new one, it would be
    // better

    connect(downloadButton, &QPushButton::clicked, this,
            [this] { QDesktopServices::openUrl(QUrl(information.assetDownloadUrl, QUrl::TolerantMode)); });

    auto message = QString("A new %1 update %2 is available. Changelog is mentioned below.\nWe highly recommend "
                           "you keep the editor up "
                           "to date to not miss on new features. Happy CP-ing")
                       .arg(information.preview ?

                                                "beta"
                                                : "stable",
                            information.name);
    name->setText(message);

    auto doc = textEdit->document();
    if (highlighter != nullptr)
        delete highlighter;

    highlighter = new MarkdownHighlighter(doc);

    show();
}

UpdatePresenter::~UpdatePresenter()
{
    delete textEdit;
    delete name;
    delete mainLayout;
    delete downloadButton;
    delete highlighter;
    delete subLayout;
    delete cancelButton;
}
} // namespace Widgets

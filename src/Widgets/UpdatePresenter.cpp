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

#include "Core/EventLogger.hpp"
#include "Util/Util.hpp"
#include <QDesktopServices>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <Widgets/UpdatePresenter.hpp>

namespace Widgets
{
UpdatePresenter::UpdatePresenter()
{
    textEdit = new QTextEdit(this);
    textEdit->setReadOnly(true);

    name = new QLabel(this);
    mainLayout = new QVBoxLayout(this);
    subLayout = new QHBoxLayout();

    downloadButton = new QPushButton("Download", this);
    cancelButton = new QPushButton("Close", this);

    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(downloadButton, &QPushButton::clicked, this,
            [this] { QDesktopServices::openUrl(QUrl(information.assetDownloadUrl, QUrl::TolerantMode)); });

    subLayout->addWidget(downloadButton);
    subLayout->addWidget(cancelButton);

    mainLayout->addWidget(name);
    mainLayout->addWidget(textEdit);
    mainLayout->addLayout(subLayout);

    setWindowTitle("New update available");
}

void UpdatePresenter::load(const Telemetry::UpdateChecker::UpdateMetaInformation &meta)
{
    LOG_INFO(BOOL_INFO_OF(meta.preview) << INFO_OF(meta.name) << INFO_OF(meta.body) << INFO_OF(meta.assetDownloadUrl)
                                        << INFO_OF(meta.releasePageUrl) << INFO_OF(meta.tagName)
                                        << INFO_OF((int)meta.result));

    information = meta;

    textEdit->setMarkdown(information.body);

    auto message =
        QString(
            "A new %1 update <a href=\"%2\">%3</a> is available. See below for the changelog.<br />We highly recommend "
            "you keep the editor up to date so that you won't miss the awesome new features and bug fixes.")
            .arg(information.preview ? "beta" : "stable")
            .arg(information.releasePageUrl)
            .arg(information.name);
    name->setText(message);

    Util::showWidgetOnTop(this);
}
} // namespace Widgets

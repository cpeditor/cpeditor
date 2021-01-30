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

#include "Widgets/UpdatePresenter.hpp"
#include "Core/EventLogger.hpp"
#include "Util/Util.hpp"
#include <QDesktopServices>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

namespace Widgets
{
UpdatePresenter::UpdatePresenter()
{
    textEdit = new QTextEdit(this);
    textEdit->setReadOnly(true);

    hint = new QLabel(this);
    auto *mainLayout = new QVBoxLayout(this);
    auto *subLayout = new QHBoxLayout();

    auto *downloadButton = new QPushButton(tr("Download"), this);
    auto *cancelButton = new QPushButton(tr("Close"), this);

    connect(cancelButton, &QPushButton::clicked, this, &UpdatePresenter::close);
    connect(downloadButton, &QPushButton::clicked, this,
            [this] { QDesktopServices::openUrl(QUrl(downloadUrl, QUrl::TolerantMode)); });

    subLayout->addWidget(downloadButton);
    subLayout->addWidget(cancelButton);

    mainLayout->addWidget(hint);
    mainLayout->addWidget(textEdit);
    mainLayout->addLayout(subLayout);

    setWindowTitle(tr("New update available"));
}

void UpdatePresenter::showUpdate(const Telemetry::UpdateChecker::UpdateMetaInformation &info)
{
    LOG_INFO(BOOL_INFO_OF(info.preview) << INFO_OF(info.version) << INFO_OF(info.changelog)
                                        << INFO_OF(info.assetDownloadUrl) << INFO_OF(info.releasePageUrl));

    downloadUrl = info.assetDownloadUrl;

    textEdit->setMarkdown(info.changelog);

    auto message =
        tr("A new %1 update <a href=\"%2\">%3</a> is available. See below for the changelog.<br />We highly recommend "
           "you keep the editor up to date so that you won't miss the awesome new features and bug fixes.")
            .arg(info.preview ? tr("beta") : tr("stable"))
            .arg(info.releasePageUrl)
            .arg(info.version);
    hint->setText(message);

    Util::showWidgetOnTop(this);
}
} // namespace Widgets

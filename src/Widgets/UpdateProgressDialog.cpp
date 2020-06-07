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

#include "Widgets/UpdateProgressDialog.hpp"
#include "Util/Util.hpp"
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QVBoxLayout>

namespace Widgets
{
UpdateProgressDialog::UpdateProgressDialog()
{
    progressBar = new QProgressBar(this);
    progressBar->setMaximum(0);
    progressBar->setMinimum(0);
    progressBar->setMinimumWidth(width());

    information = new QLabel(this);

    cancelUpdate = new QPushButton(tr("Cancel"), this);
    cancelUpdate->setToolTip(tr("Close this dialog and abort the update check"));

    mainLayout = new QVBoxLayout(this);

    mainLayout->addWidget(information);
    mainLayout->addWidget(progressBar);
    mainLayout->addWidget(cancelUpdate);

    setWindowTitle(tr("Update Checker"));

    setModal(false);

    connect(cancelUpdate, SIGNAL(clicked()), this, SIGNAL(canceled()));
    connect(cancelUpdate, SIGNAL(clicked()), this, SLOT(close()));
}

void UpdateProgressDialog::start()
{
    information->setText(tr("Fetching the list of releases..."));
    progressBar->show();
    Util::showWidgetOnTop(this);
}

void UpdateProgressDialog::onUpdateFailed(const QString &error)
{
    information->setText(
        tr("Error: %1<br /><br />Updater failed to check for update. Please manually check for update at<br /><a "
           "href=\"https://cpeditor.github.io/download\">https://cpeditor.github.io/download</a> or <a "
           "href=\"https://github.com/cpeditor/cpeditor/releases\">https://github.com/cpeditor/cpeditor/releases</a>.")
            .arg(error));
    progressBar->hide();
}

void UpdateProgressDialog::onAlreadyUpToDate()
{
    progressBar->hide();
    cancelUpdate->setText(tr("Close"));
    information->setText(tr("Hooray!! You are already using the latest release of CP Editor."));
}
} // namespace Widgets

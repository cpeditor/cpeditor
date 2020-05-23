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

#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QVBoxLayout>
#include <Widgets/UpdateProgressDialog.hpp>

namespace Widgets
{

UpdateProgressDialog::UpdateProgressDialog()
{
    progressBar = new QProgressBar(this);
    progressBar->setMaximum(0);
    progressBar->setMinimum(0);
    progressBar->setMinimumWidth(width());

    information = new QLabel(this);

    updateChecker = new Telemetry::UpdateChecker();

    cancelUpdate = new QPushButton("Cancel", this);
    cancelUpdate->setToolTip("Closes this dialog and aborts update check");

    mainLayout = new QVBoxLayout(this);

    mainLayout->addWidget(information);
    mainLayout->addWidget(progressBar);
    mainLayout->addWidget(cancelUpdate);

    setLayout(mainLayout);
    setWindowTitle("Update Checker");

    setModal(false);

    connect(updateChecker, &Telemetry::UpdateChecker::updateCheckerFailed, this, &UpdateProgressDialog::updaterFailed);
    connect(updateChecker, &Telemetry::UpdateChecker::updateCheckerFinished, this,
            &UpdateProgressDialog::updaterFinished);

    connect(cancelUpdate, &QPushButton::clicked, [this]() {
        updateChecker->cancelCheckUpdate();
        close();
    });
}

void UpdateProgressDialog::start(bool beta)
{
    resetState();
    updateChecker->checkUpdate(beta);
    show();
}

void UpdateProgressDialog::resetState()
{
    information->setText("Communicating with remote servers for new releases...");
    information->setVisible(true);
    progressBar->setVisible(true);
}

UpdateProgressDialog::~UpdateProgressDialog()
{
    delete information;
    delete updateChecker;
    delete progressBar;
    delete cancelUpdate;
    delete mainLayout;
}

// slots

void UpdateProgressDialog::updaterFailed(QString error)
{
    information->setText("Error: " + error);
    information->setVisible(true);
    cancelUpdate->setText("Close");
    progressBar->setVisible(false);
}

void UpdateProgressDialog::updaterFinished(Telemetry::UpdateChecker::UpdateMetaInformation meta)
{
    if (meta.result == Telemetry::UpdateChecker::UpdateCheckerResult::STABLE_UPDATE ||
        meta.result == Telemetry::UpdateChecker::UpdateCheckerResult::BETA_UPDATE)
    {
        close();
        emit updateAvailable(meta);
    }
    else if (meta.result == Telemetry::UpdateChecker::UpdateCheckerResult::NO_UPDATES)
    {
        information->setVisible(true);
        progressBar->setVisible(false);
        cancelUpdate->setText("Close");
        information->setText(
            "Hooray!! You are already using the latest and greatest of CP Editor. Please keep checking...");
    }
    else
    {
        information->setVisible(true);
        progressBar->setVisible(false);
        cancelUpdate->setText("Close");
        information->setText("Updater failed to check for update. Please manually check for update at "
                             "https://cpeditor.github.io");
    }
}
} // namespace Widgets

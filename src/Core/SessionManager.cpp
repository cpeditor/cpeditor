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

#include "Core/SessionManager.hpp"
#include "../../ui/ui_appwindow.h"
#include "Core/EventLogger.hpp"
#include "Util/FileUtil.hpp"
#include "appwindow.hpp"
#include "generated/portable.hpp"
#include "mainwindow.hpp"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QProgressDialog>
#include <QTimer>

namespace Core
{
const static QStringList sessionFileLocations = {
#ifdef PORTABLE_VERSION
    "$BINARY/cp_editor_session.json",
#endif
    "$APPCONFIG/cp_editor_session.json"};

SessionManager::SessionManager(AppWindow *appwindow) : QObject(appwindow), app(appwindow)
{
    timer = new QTimer(this);

    timer->setInterval(10000);
    connect(timer, &QTimer::timeout, this, &SessionManager::updateSession, Qt::DirectConnection);
}

void SessionManager::restoreSession(const QString &path)
{
    LOG_INFO(INFO_OF(path));

    auto text = Util::readFile(path);

    if (text.isNull())
    {
        LOG_ERR(QString("Failed to load session from [%1]").arg(path));
        return;
    }

    auto document = QJsonDocument::fromJson(text.toUtf8());

    if (document.isNull())
    {
        LOG_ERR("Invalid session JSON: " << text);
        return;
    }

    app->setInitialized(false);

    while (app->ui->tabWidget->count() > 0)
    {
        auto *tmp = app->windowAt(0);
        app->ui->tabWidget->removeTab(0);
        delete tmp;
    }

    QJsonObject object = document.object();

    const int currentIndex = object["currentIndex"].toInt();
    auto tabs = object["tabs"].toArray();

    QProgressDialog progressDialog(app);
    progressDialog.setWindowModality(Qt::WindowModal);
    progressDialog.setWindowTitle(tr("Restoring Last Session"));

    progressDialog.setMaximum(tabs.count());
    progressDialog.setValue(0);

    auto oldSize = app->size();
    app->setUpdatesEnabled(false);

    for (auto &&tab : tabs)
    {
        if (progressDialog.wasCanceled())
            break;
        auto status = MainWindow::EditorStatus(tab.toObject().toVariantMap());
        app->openTab(status);
        progressDialog.setLabelText(QString(tr("Restoring: [%1]")).arg(app->currentWindow()->getTabTitle(true, false)));
        progressDialog.setValue(progressDialog.value() + 1);
    }

    app->setUpdatesEnabled(true);
    app->repaint();
    app->resize(oldSize);

    if (currentIndex >= 0 && currentIndex < app->ui->tabWidget->count())
        app->ui->tabWidget->setCurrentIndex(currentIndex);

    app->setInitialized();
}

void SessionManager::setAutoUpdateSession(bool shouldAutoUpdate)
{
    if (shouldAutoUpdate)
        timer->start();
    else
        timer->stop();
}

void SessionManager::setAutoUpdateDuration(int duration)
{
    timer->setInterval(duration);
}

QString SessionManager::currentSessionText()
{
    QJsonObject json;
    json.insert("currentIndex", app->ui->tabWidget->currentIndex());

    QJsonArray arr;
    for (int t = 0; t < app->ui->tabWidget->count(); t++)
    {
        arr.push_back(QJsonDocument::fromVariant(app->windowAt(t)->toStatus().toMap()).object());
    }

    json.insert("tabs", arr);

    return QJsonDocument(json).toJson();
}

QString SessionManager::lastSessionPath()
{
    return Util::firstExistingConfigPath(sessionFileLocations);
}

void SessionManager::saveSession(const QString &sessionText)
{
    Util::saveFile(Util::configFilePath(sessionFileLocations[0]), sessionText, "Save Session", true, nullptr, true);
}

void SessionManager::updateSession()
{
    saveSession(currentSessionText());
}
} // namespace Core

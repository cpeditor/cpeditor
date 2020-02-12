/*
 * Copyright (C) 2019-2020 Ashar Khan <ashar786khan@gmail.com>
 *
 * This file is part of CPEditor.
 *
 * CPEditor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * I will not be responsible if CPEditor behaves in unexpected way and
 * causes your ratings to go down and or loose any important contest.
 *
 * Believe Software is "Software" and it isn't immune to bugs.
 *
 */

#ifndef UPDATENOTIFIER_HPP
#define UPDATENOTIFIER_HPP
#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <generated/version.hpp>

namespace Telemetry
{
class UpdateNotifier : public QObject
{
    Q_OBJECT
  public:
    UpdateNotifier(bool useBeta);
    void checkUpdate(bool force = false);
    ~UpdateNotifier();
    void setBeta(bool value);

  private slots:

    void managerFinished(QNetworkReply *reply);

  private:
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    bool beta;
    bool force;
};
} // namespace Telemetry

#endif // UPDATENOTIFIER_HPP

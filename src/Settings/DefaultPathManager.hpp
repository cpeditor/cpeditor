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

#ifndef DEFAULTPATHMANAGER_HPP
#define DEFAULTPATHMANAGER_HPP

#include <QFileDialog>
#include <QMap>
#include <QVariant>

class DefaultPathManager
{
  public:
    /**
     * @brief get the default path for *action*
     */
    static QString defaultPathForAction(const QString &action);

    /**
     * @brief set the default path after getting the default path for *action*
     */
    static void setDefaultPathForAction(const QString &action, const QString &path);

    /**
     * @brief a wrapper for QFileDialog::getExistingDirectory
     */
    static QString getExistingDirectory(const QString &action, QWidget *parent = nullptr,
                                        const QString &caption = QString(),
                                        QFileDialog::Options options = QFileDialog::ShowDirsOnly);
    /**
     * @brief a wrapper for QFileDialog::getOpenFileName
     */
    static QString getOpenFileName(const QString &action, QWidget *parent = nullptr, const QString &caption = QString(),
                                   const QString &filter = QString(), QString *selectedFilter = nullptr,
                                   QFileDialog::Options options = QFileDialog::Options());

    /**
     * @brief a wrapper for QFileDialog::getOpenFileNames
     */
    static QStringList getOpenFileNames(const QString &action, QWidget *parent = nullptr,
                                        const QString &caption = QString(), const QString &filter = QString(),
                                        QString *selectedFilter = nullptr,
                                        QFileDialog::Options options = QFileDialog::Options());

    /**
     * @brief a wrapper for QFileDialog::getSaveFileName
     */
    static QString getSaveFileName(const QString &action, QWidget *parent = nullptr, const QString &caption = QString(),
                                   const QString &filter = QString(), QString *selectedFilter = nullptr,
                                   QFileDialog::Options options = QFileDialog::Options());

    /**
     * @brief restore the default paths from a QVariant
     */
    static void fromVariantList(const QVariantList &list);

    /**
     * @brief get a list of all action settings, in a correct order instead of the lexicographical order
     */
    static QStringList actionSettingsList();

  private:
    /**
     * @brief get the path from *str*, replace place holders with default paths
     */
    static QString convertPath(const QString &str);

    /**
     * @brief dump the default paths to a QVariant
     */
    static QVariantList toVariantList();

  private:
    static QMap<QString, QString> defaultPath;
};

#endif // DEFAULTPATHMANAGER_HPP

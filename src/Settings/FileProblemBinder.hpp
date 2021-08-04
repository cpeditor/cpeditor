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

/**
 * This class is used to bind the local file path with the problem URL.
 */

#ifndef FILEPROBLEMBINDER_HPP
#define FILEPROBLEMBINDER_HPP

#include <QMap>

class QVariant;

class FileProblemBinder
{
  public:
    /**
     * @brief add a pair of file and problem, remove the old binding of both of them
     * @param file the path to the file
     * @param problem the URL of the problem
     * @note if either of *file* or *problem* is empty, the new binding won't be added, but the old binding (if exists)
     * will be removed
     */
    static void set(const QString &file, const QString &problem);

    /**
     * @brief get the URL of the problem binded with the given file path
     * @param file the path to the file; null if no problem is binded to the file
     * @returns the URL of the problem
     */
    static QString getProblemForFile(const QString &file);

    /**
     * @brief get the path to the file binded with the given problem URL
     * @param problem the URL of the problem
     * @returns the path to the file; null if no file is binded to the problem
     */
    static QString getFileForProblem(const QString &problem);

    /**
     * @brief get if a file is binded to some problem
     */
    static bool containsFile(const QString &file);

    /**
     * @brief get if a problem is binded to some file
     */
    static bool containsProblem(const QString &problem);

    /**
     * @brief dump the binding to a QStringList
     */
    static QStringList toList();

    /**
     * @brief load the binding from a QStringList
     * @param list the QStringList to load from
     */
    static void fromList(const QStringList &list);

  private:
    static QMap<QString, QString> fileForProblem;
    static QMap<QString, QString> problemForFile;
};

#endif // FILEPROBLEMBINDER_HPP

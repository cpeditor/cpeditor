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

#include "Settings/FileProblemBinder.hpp"
#include "Core/EventLogger.hpp"
#include <QVariant>

QMap<QString, QString> FileProblemBinder::fileForProblem;
QMap<QString, QString> FileProblemBinder::problemForFile;

void FileProblemBinder::set(const QString &file, const QString &problem)
{
    LOG_INFO(INFO_OF(file) << INFO_OF(problem));
    if (problemForFile.contains(file))
    {
        fileForProblem.remove(problemForFile[file]);
        problemForFile.remove(file);
    }
    if (fileForProblem.contains(problem))
    {
        problemForFile.remove(fileForProblem[problem]);
        fileForProblem.remove(problem);
    }
    if (!file.isEmpty() && !problem.isEmpty())
    {
        problemForFile[file] = problem;
        fileForProblem[problem] = file;
    }
}

QString FileProblemBinder::getProblemForFile(const QString &file)
{
    return problemForFile[file];
}

QString FileProblemBinder::getFileForProblem(const QString &problem)
{
    return fileForProblem[problem];
}

bool FileProblemBinder::containsFile(const QString &file)
{
    return problemForFile.contains(file);
}

bool FileProblemBinder::containsProblem(const QString &problem)
{
    return fileForProblem.contains(problem);
}

QStringList FileProblemBinder::toList()
{
    QStringList res;
    res.reserve(problemForFile.count() * 2);
    for (auto it = problemForFile.constKeyValueBegin(); it != problemForFile.constKeyValueEnd(); ++it)
    {
        res.push_back((*it).first);
        res.push_back((*it).second);
    }
    return res;
}

void FileProblemBinder::fromList(const QStringList &list)
{
    for (int i = 0; i + 1 < list.count(); i += 2)
        set(list[i], list[i + 1]);
}

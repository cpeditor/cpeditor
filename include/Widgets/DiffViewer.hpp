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

#ifndef DIFFVIEWER_HPP
#define DIFFVIEWER_HPP

#include "Core/MessageLogger.hpp"
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QTextEdit>
#include <QVBoxLayout>

class DiffViewer : public QMainWindow
{
    Q_OBJECT

  public:
    static const int MAX_CHARACTERS_FOR_HTML = 100000;

    explicit DiffViewer(QWidget *parent = nullptr);
    void setText(const QString &output, const QString &expected);

  signals:
    void toLongForHtml();

  private:
    QHBoxLayout *layout = nullptr;
    QWidget *widget = nullptr;
    QVBoxLayout *leftLayout = nullptr, *rightLayout = nullptr;
    QLabel *outputLabel = nullptr, *expectedLabel = nullptr;
    QTextEdit *outputEdit = nullptr, *expectedEdit = nullptr;
};

#endif // DIFFVIEWER_HPP

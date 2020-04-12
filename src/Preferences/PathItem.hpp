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

/*
 * A line edit and a tool button for choosing a file path.
 */

#ifndef PATHITEM_HPP
#define PATHITEM_HPP

#include <QHBoxLayout>
#include <QLineEdit>
#include <QToolButton>

class PathItem : public QWidget
{
    Q_OBJECT

  public:
    /**
     * @brief construct a PathItem
     */
    explicit PathItem(const QString &pathFilter, const QString &dialogTitle, QWidget *parent = nullptr);

    /**
     * @brief construct a PathItem use builtin filter and title
     */
    explicit PathItem(int filterId, int titleId, QWidget *parent = nullptr);

    /**
     * @brief get the line edit of the PathItem
     */
    QLineEdit *getLineEdit();

  private slots:
    /**
     * @brief choose the path of the template
     */
    void onButtonClicked();

  private:
    QString filter, title;
    QHBoxLayout *layout = nullptr;
    QLineEdit *lineEdit = nullptr;
    QToolButton *toolButton = nullptr;
};

#endif // CODETEMPLATEITEM_HPP

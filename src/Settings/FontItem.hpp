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
 * A push button for choosing a font.
 */

#ifndef FONTITEM_HPP
#define FONTITEM_HPP

#include <QPushButton>

class FontItem : public QPushButton
{
    Q_OBJECT
  public:
    explicit FontItem(QWidget *parent);

    void setFont(const QString &desc);
    void setFont(QFont newFont);
    QFont getFont();

  signals:
    void fontChanged(QFont font);

  private slots:
    void onButtonClicked();

  private:
    QFont font;
};

#endif // FONTITEM_HPP

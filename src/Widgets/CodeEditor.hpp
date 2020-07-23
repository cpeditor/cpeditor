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

#ifndef CODEEDITOR_HPP
#define CODEEDITOR_HPP

#include <QCodeEditor>

namespace KSyntaxHighlighting
{
class SyntaxHighlighter;
}

class CodeEditor : public QCodeEditor
{
    Q_OBJECT

  public:
    explicit CodeEditor(QWidget *parent = nullptr);

    void applySettings(const QString &language);

  private:
    KSyntaxHighlighting::SyntaxHighlighter *highlighter = nullptr;
};

#endif // CODEEDITOR_HPP

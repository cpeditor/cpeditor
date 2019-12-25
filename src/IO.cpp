/*
 * Copyright (C) 2019 Ashar Khan <ashar786khan@gmail.com>
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
 * Believe Software is "Software" and it isn't not immune to bugs.
 *
 */

#include <IO.hpp>

namespace Core
{
namespace IO
{

InputReader::InputReader(QVector<QPlainTextEdit *> _a)
{
    a = _a;
    for (int i = 0; i < 3; ++i)
    {
        f[i] = new QFile(getInput(i));
    }
}

InputReader::~InputReader()
{
    for (int i = 0; i < 3; ++i)
    {
        if (f[i]->isOpen())
            f[i]->close();
        delete f[i];
    }
}

void InputReader::readToFile()
{
    for (int i = 0; i < 3; ++i)
    {
        if (!f[i]->isOpen())
            f[i]->open(QIODevice::WriteOnly | QFile::Text);
        f[i]->resize(0);
        f[i]->write(a[i]->toPlainText().toStdString().c_str());
        f[i]->close();
    }
}

} // namespace IO
} // namespace Core

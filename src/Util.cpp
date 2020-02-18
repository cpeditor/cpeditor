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

#include "Util.hpp"
#include "Core/EventLogger.hpp"
#include <QFile>
#include <QSaveFile>

namespace Util
{

bool saveFile(const QString &path, const QString &content, const QString &head, bool safe, MessageLogger *log)
{
    if (safe)
    {
        QSaveFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            Core::Log::e("Util/saveFile/safe/" + head) << "Failed to open [" << path << "]" << endl;
            if (log != nullptr)
                log->error(head, "Failed to open [" + path + "]. Do I have write permission?");
            return false;
        }
        file.write(content.toUtf8());
        if (!file.commit())
        {
            Core::Log::e("Util/saveFile/safe/" + head) << "Failed to save to [" << path << "]" << endl;
            if (log != nullptr)
                log->error(head, "Failed to save to [" + path + "]. Do I have write permission?");
            return false;
        }
    }
    else
    {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            Core::Log::e("Util/saveFile/unsafe/" + head) << "Failed to open [" << path << "]" << endl;
            if (log != nullptr)
                log->error(head, "Failed to open [" + path + "]. Do I have write permission?");
            return false;
        }
        if (file.write(content.toUtf8()) == -1)
        {
            Core::Log::e("Util/saveFile/unsafe/" + head) << "Failed to save to [" << path << "]" << endl;
            if (log != nullptr)
                log->error(head, "Failed to save to [" + path + "]. Do I have write permission?");
            return false;
        }
    }
    Core::Log::i("Util/saveFile/" + head) << "Successfully saved to [" << path << "]" << endl;
    return true;
}

} // namespace Util
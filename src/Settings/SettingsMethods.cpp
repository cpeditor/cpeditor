#include "Settings/SettingsMethods.hpp"
#include "Settings/DefaultPathManager.hpp"
#include "Util/FileUtil.hpp"
#include "Util/Util.hpp"
#include <QMessageBox>

bool SettingsMethods::loadSnippet(MapWrapper *widget, bool valid)
{
    if (valid)
        return true;

    QString lang = widget->iter.pre.back();

    auto files =
        DefaultPathManager::getOpenFileNames("Extract And Load Snippets", widget->rootWidget(), tr("Load Snippets"),
                                             Util::fileNameFilter(lang == "C++", lang == "Java", lang == "Python"));
    for (auto const &file : files)
    {
        auto content = Util::readFile(file, "SettingsMethods::loadSnippet");
        if (content.isNull())
        {
            QMessageBox::warning(widget->rootWidget(), tr("Load Snippets"),
                                 tr("Failed to open [%1]. Do I have read permission?").arg(file));
            break;
        }
        auto name = QFileInfo(file).completeBaseName();
        auto i = 1;
        auto ks = widget->keys();
        if (ks.contains(name))
        {
            while (true)
            {
                auto nn = QString("%1_%2").arg(name).arg(i++);
                if (!ks.contains(nn))
                {
                    name = widget->askKey(nn);
                    break;
                }
            }
        }
        if (name.isEmpty())
            break;
        widget->add(name);
        widget->getSub(name)->locate("Code")->setV(content);
    }
    return true;
}

bool SettingsMethods::extractSnippet(MapWrapper *widget, bool valid)
{
    if (valid)
        return widget->keys().length() > 0;

    QString lang = widget->iter.pre.back();

    auto dirPath = DefaultPathManager::getExistingDirectory("Extract And Load Snippets", widget->rootWidget(),
                                                            tr("Extract Snippets"));

    if (dirPath.isEmpty())
        return true;

    auto dir = QDir(dirPath);

    auto names = widget->keys();

    QString suffix;
    if (lang == "C++")
        suffix = "." + Util::cppSuffix.front();
    else if (lang == "Java")
        suffix = "." + Util::javaSuffix.front();
    else if (lang == "Python")
        suffix = "." + Util::pythonSuffix.front();

    for (auto const &name : names)
    {
        auto filePath = dir.filePath(name + suffix);
        if (QFile::exists(filePath))
        {
            filePath =
                QFileDialog::getSaveFileName(widget->rootWidget(), tr("Extract Snippets: %1").arg(name), filePath,
                                             Util::fileNameFilter(lang == "C++", lang == "Java", lang == "Python"));
        }
        if (filePath.isEmpty())
            break;
        if (!Util::saveFile(filePath, widget->getSub(name)->locate("Code")->getV().toString(), "Extract Snippets"))
        {
            QMessageBox::warning(widget->rootWidget(), tr("Extract Snippets"),
                                 tr("Failed to write to [%1]. Do I have write permission?").arg(filePath));
            break;
        }
    }
    return true;
}
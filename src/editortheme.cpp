#include "EditorTheme.hpp"

#include <QFile>
#include <QIODevice>

namespace Themes
{

QSyntaxStyle *EditorTheme::light = nullptr;
QSyntaxStyle *EditorTheme::drakula = nullptr;
QSyntaxStyle *EditorTheme::monkai = nullptr;
QSyntaxStyle *EditorTheme::solarised = nullptr;
QSyntaxStyle *EditorTheme::solarisedDark = nullptr;

QSyntaxStyle *EditorTheme::getLightTheme()
{
    if (light != nullptr)
        return light;
    else
        return QSyntaxStyle::defaultStyle();
}
QSyntaxStyle *EditorTheme::getMonkaiTheme()
{
    if (monkai != nullptr)
        return monkai;
    else
    {
        QFile f(":/styles/monkai.xml");
        f.open(QIODevice::ReadOnly);
        if (!f.isOpen())
            return nullptr;
        monkai = new QSyntaxStyle();
        monkai->load(f.readAll());
        return monkai;
    }
}
QSyntaxStyle *EditorTheme::getDrakulaTheme()
{
    if (drakula != nullptr)
        return drakula;
    else
    {
        QFile f(":/styles/drakula.xml");
        f.open(QIODevice::ReadOnly);
        if (!f.isOpen())
            return nullptr;
        drakula = new QSyntaxStyle();
        drakula->load(f.readAll());
        return drakula;
    }
}
QSyntaxStyle *EditorTheme::getSolarisedTheme()
{
    if (solarised != nullptr)
        return solarised;
    else
    {
        QFile f(":/styles/solarised.xml");
        f.open(QIODevice::ReadOnly);
        if (!f.isOpen())
            return nullptr;
        solarised = new QSyntaxStyle();
        solarised->load(f.readAll());
        return solarised;
    }
}
QSyntaxStyle *EditorTheme::getSolarisedDarkTheme()
{
    if (solarisedDark != nullptr)
        return solarisedDark;
    else
    {
        QFile f(":/styles/solarisedDark.xml");
        f.open(QIODevice::ReadOnly);
        if (!f.isOpen())
            return nullptr;
        solarisedDark = new QSyntaxStyle();
        solarisedDark->load(f.readAll());
        return solarisedDark;
    }
}

void EditorTheme::release()
{
    if (light != nullptr)
        delete light;
    if (drakula != nullptr)
        delete drakula;
    if (monkai != nullptr)
        delete monkai;
    if (solarised != nullptr)
        delete solarised;
    if (solarisedDark != nullptr)
        delete solarisedDark;
}
} // namespace Themes

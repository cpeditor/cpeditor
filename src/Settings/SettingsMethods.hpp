#ifndef SETTINGSMETHODS_HPP
#define SETTINGSMETHODS_HPP

#include "SettingWidget.hpp"

struct SettingsMethods
{
    Q_DECLARE_TR_FUNCTIONS(SettingsMethods)

  public:
    SettingsMethods() = delete;

    static bool loadSnippet(MapWrapper *widget, bool valid);
    static bool extractSnippet(MapWrapper *widget, bool valid);
};

#endif
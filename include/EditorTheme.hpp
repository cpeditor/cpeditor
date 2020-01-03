#ifndef EDITORTHEME_H
#define EDITORTHEME_H

#include <QSyntaxStyle>

namespace Themes
{
class EditorTheme
{
  public:
    EditorTheme() = delete;

  private:
    static QSyntaxStyle *light;
    static QSyntaxStyle *drakula;
    static QSyntaxStyle *monkai;
    static QSyntaxStyle *solarised;
    static QSyntaxStyle *solarisedDark;

  public:
    static QSyntaxStyle *getLightTheme();
    static QSyntaxStyle *getDrakulaTheme();
    static QSyntaxStyle *getMonkaiTheme();
    static QSyntaxStyle *getSolarisedTheme();
    static QSyntaxStyle *getSolarisedDarkTheme();

    static void release();
};
} // namespace Themes

#endif // EDITORTHEME_H

#ifndef PREFERENCESGRIDPAGE_HPP
#define PREFERENCESGRIDPAGE_HPP

#include "Preferences/PreferencesPage.hpp"
#include <QGridLayout>

class PreferencesGridPage : public PreferencesPage
{
    Q_OBJECT
  public:
    explicit PreferencesGridPage(QWidget *parent = nullptr);

    void addRow(QWidget *widget);
    void addRow(QString labelText, QWidget *widget);

    // use template to help registerWidget know the type of widget
    template <typename T> void registerAddRow(T *widget)
    {
        registerWidget(widget);
        addRow(widget);
    }

    template <typename T> void registerAddRow(QString label, T *widget)
    {
        registerWidget(widget);
        addRow(label, widget);
    }

    template <typename T, typename TT> void registerAddRow(QString label, T *widget, TT *data)
    {
        registerWidget(data);
        addRow(label, widget);
    }

  private:
    QGridLayout *gridLayout;
};

#endif
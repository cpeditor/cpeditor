#ifndef PREFERENCEWINDOW_HPP
#define PREFERENCEWINDOW_HPP

#include <QMainWindow>

namespace Ui {
class PreferenceWindow;
}

class PreferenceWindow : public QMainWindow
{
    Q_OBJECT

signals:
    void settingsApplied();
public:
    explicit PreferenceWindow(QWidget *parent = nullptr);
    void resetSettings();
    ~PreferenceWindow();

private:
    Ui::PreferenceWindow *ui;
};

#endif // PREFERENCEWINDOW_HPP

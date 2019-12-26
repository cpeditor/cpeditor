#ifndef APPWINDOW_HPP
#define APPWINDOW_HPP

#include <QMainWindow>

namespace Ui {
class AppWindow;
}

class AppWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AppWindow(QWidget *parent = nullptr);
    ~AppWindow();

private:
    Ui::AppWindow *ui;
};

#endif // APPWINDOW_HPP

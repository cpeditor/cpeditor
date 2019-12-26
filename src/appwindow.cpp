#include "include/appwindow.hpp"
#include "../ui/ui_appwindow.h"

AppWindow::AppWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::AppWindow)
{
    ui->setupUi(this);
}

AppWindow::~AppWindow()
{
    delete ui;
}

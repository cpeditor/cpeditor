#include "include/preferencewindow.hpp"
#include "../ui/ui_preferencewindow.h"

PreferenceWindow::PreferenceWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PreferenceWindow){
    ui->setupUi(this);
    setWindowTitle("Preferences");
}

void PreferenceWindow::resetSettings(){

}

PreferenceWindow::~PreferenceWindow(){
    delete ui;
}

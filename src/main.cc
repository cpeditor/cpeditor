#include <QApplication>

#include "mainwindow.hpp"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  return a.exec();
}

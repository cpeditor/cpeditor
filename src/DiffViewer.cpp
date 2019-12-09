#include "DiffViewer.hpp"

#include "../ui/ui_diffviewer.h"

DiffViewer::DiffViewer(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::DiffViewer) {
  ui->setupUi(this);
}

DiffViewer::~DiffViewer() {
  delete ui;
}

void DiffViewer::on_exit_clicked() {
  this->close();
}

void DiffViewer::on_read_clicked() {}

void DiffViewer::on_update_clicked() {}

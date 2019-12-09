#include "DiffViewer.hpp"

#include "../ui/ui_diffviewer.h"

DiffViewer::DiffViewer(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::DiffViewer) {
  ui->setupUi(this);
}

DiffViewer::~DiffViewer() {
  delete ui;
}

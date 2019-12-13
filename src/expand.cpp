#include <MessageLogger.hpp>
#include <QFileDialog>
#include <expand.hpp>

#include "../ui/ui_expand.h"

Expand::Expand(QWidget* parent) : QMainWindow(parent), ui(new Ui::Expand) {
  ui->setupUi(this);
  ui->plainTextEdit->setWordWrapMode(QTextOption::NoWrap);
}

Expand::Expand(QPlainTextEdit* text) : Expand() {
  this->source = text;
  ui->plainTextEdit->setPlainText(text->toPlainText());
}

Expand::Expand(QTextBrowser* browser) : Expand() {
  this->window()->setWindowTitle("Compiler Messages");
  ui->plainTextEdit->hide();
  current = new QTextBrowser();
  ui->verticalLayout->insertWidget(0, current);

  ui->update->setEnabled(false);
  ui->read->setEnabled(false);
  current->setReadOnly(true);
  current->setOpenExternalLinks(true);
  current->setHtml(browser->toHtml());
}

void Expand::setTitle(QString string) {
  this->setWindowTitle(string);
}

void Expand::setUpdate(bool enabled) {
  ui->plainTextEdit->setReadOnly(!enabled);
  this->ui->update->setEnabled(enabled);
}

void Expand::setReadFile(bool enabled) {
  this->ui->read->setEnabled(enabled);
}

Expand::~Expand() {
  if (current != nullptr)
    delete current;
  delete ui;
}

void Expand::on_exit_clicked() {
  this->close();
}

void Expand::on_update_clicked() {
  if (source != nullptr) {
    source->clear();
    source->setPlainText(ui->plainTextEdit->toPlainText());
  }
  this->close();
}

void Expand::on_read_clicked() {
  QString filename = QFileDialog::getOpenFileName(this, "Choose file", "", "*.*");
  if (filename.isEmpty())
    return;
  QFile* file = new QFile(filename);
  if (!file->open(QIODevice::ReadOnly | QFile::Text)) {
    Log::MessageLogger::info("IO Operation", "Cannot open file for read.");
    delete file;
    return;
  }
  ui->plainTextEdit->setPlainText(file->readAll());
  file->close();
  delete file;
}

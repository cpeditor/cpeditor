#include "DiffViewer.hpp"

#include <MessageLogger.hpp>
#include <QFileDialog>

#include "../ui/ui_diffviewer.h"

DiffViewer::DiffViewer(QWidget *parent) : QMainWindow(parent), ui(new Ui::DiffViewer)
{
    ui->setupUi(this);
}

DiffViewer::DiffViewer(QString *expected, QPlainTextEdit *ui) : DiffViewer()
{
    this->ui->expected->setPlainText(*expected);
    this->ui->resulted->setPlainText(ui->toPlainText());
    res = ui;
    exp = expected;
    this->ui->expected->setWordWrapMode(QTextOption::NoWrap);
    this->ui->resulted->setWordWrapMode(QTextOption::NoWrap);
}

void DiffViewer::setTitle(QString title)
{
    this->setWindowTitle(title);
}

DiffViewer::~DiffViewer()
{
    delete ui;
}

void DiffViewer::on_exit_clicked()
{
    this->close();
}

void DiffViewer::on_read_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Choose file", "", "*.*");
    if (filename.isEmpty())
        return;
    QFile *file = new QFile(filename);
    if (!file->open(QIODevice::ReadOnly | QFile::Text))
    {
        Log::MessageLogger::info("IO Operation", "Cannot open file for read.");
        delete file;
        return;
    }
    this->ui->expected->setPlainText(file->readAll());
    file->close();
    delete file;
}

void DiffViewer::on_update_clicked()
{
    exp->operator=(this->ui->expected->toPlainText());
    this->close();
}

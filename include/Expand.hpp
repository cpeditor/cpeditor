#ifndef PLAINTEXTEDITDIALOG_HPP
#define PLAINTEXTEDITDIALOG_HPP
#include <QMainWindow>
#include <QObject>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QTextBrowser>
#include <QVBoxLayout>

QT_BEGIN_NAMESPACE
namespace Ui {
class Expand;
}
QT_END_NAMESPACE

class Expand : public QMainWindow {
  Q_OBJECT
 public:
  Expand(QPlainTextEdit* target);
  Expand(QTextBrowser* target);

 private:
  Ui::Expand* ui;
  QPushButton* okay;
  QPushButton* cancel;
  QPushButton* chooseFile;
  QPlainTextEdit* text;
  QVBoxLayout* root;
  QHBoxLayout* subroot;
};

#endif  // PLAINTEXTEDITDIALOG_HPP

#ifndef DIFFVIEWER_HPP
#define DIFFVIEWER_HPP

#include <QMainWindow>
#include <QPlainTextEdit>
#include "MessageLogger.hpp"

namespace Ui
{
class DiffViewer;
}

class DiffViewer : public QMainWindow
{
    Q_OBJECT

  public:
    explicit DiffViewer(QWidget *parent = nullptr);
    DiffViewer(QString *expected, QPlainTextEdit *ui, MessageLogger* log);
    void setTitle(QString title);
    ~DiffViewer();

  private slots:
    void on_exit_clicked();

    void on_read_clicked();

    void on_update_clicked();

  private:
    Ui::DiffViewer *ui;
    QString *exp;
    QPlainTextEdit *res;
    MessageLogger * log;
};

#endif // DIFFVIEWER_HPP

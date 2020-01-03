#ifndef EXPAND_HPP
#define EXPAND_HPP

#include "MessageLogger.hpp"
#include <QMainWindow>
#include <QPlainTextEdit>
#include <QTextBrowser>

namespace Ui
{
class Expand;
}

class Expand : public QMainWindow
{
    Q_OBJECT

  public:
    Expand(QPlainTextEdit *editor, MessageLogger *log, QWidget *parent = nullptr);
    Expand(QTextBrowser *browser, MessageLogger *log, QWidget *parent = nullptr);
    explicit Expand(QWidget *parent = nullptr);

    void setTitle(QString);
    void setReadFile(bool enabled);
    void setUpdate(bool enabled);

    QPlainTextEdit *getMainBody();
    ~Expand();

  private slots:
    void on_exit_clicked();

    void on_update_clicked();

    void on_read_clicked();

  private:
    Ui::Expand *ui;
    QPlainTextEdit *source = nullptr;
    QTextBrowser *current = nullptr;
    MessageLogger *log;
};

#endif // EXPAND_HPP

#ifndef DIFFVIEWER_HPP
#define DIFFVIEWER_HPP

#include <QMainWindow>

namespace Ui {
class DiffViewer;
}

class DiffViewer : public QMainWindow {
  Q_OBJECT

 public:
  explicit DiffViewer(QWidget* parent = nullptr);
  ~DiffViewer();

 private slots:
  void on_exit_clicked();

  void on_read_clicked();

  void on_update_clicked();

 private:
  Ui::DiffViewer* ui;
};

#endif  // DIFFVIEWER_HPP

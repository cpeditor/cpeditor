#ifndef DEBUGCONTROL_HPP
#define DEBUGCONTROL_HPP

#include <QDialog>
#include <QMap>
#include <qgdbint.h>

namespace Ui
{
    class DebugControl;
}

class DebugControl : public QDialog
{
    Q_OBJECT

  public:
    enum Status {
        STOP,
        RUN,
        PAUSE
    };
    explicit DebugControl(QString gdb, QString gdbServer, QString program, QStringList arguments, QWidget *parent = nullptr);
    ~DebugControl();

  public slots:
    void rowChanged(int row); // tell debug control the row cursor lies.
    void setInput(QString input);

  signals:
    void currentRowChanged(int row); // tell editor the current running row.

  private slots:
    void onStateChanged(bool running, QString reason);
    void onExited();

  private:
    void updateStatus();

    Ui::DebugControl *ui;
    qgdbint::QGdb* qgdb;
    QString program;
    QStringList arguments;
    Status status;
    int curRow = 1;
    QMap<int, int> bps;
    QList<int> bpCache;
    int port;

    static QSet<int> portUsed;
};

#endif // DEBUGCONTROL_HPP

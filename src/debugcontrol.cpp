#include "debugcontrol.hpp"
#include "../ui/ui_debugcontrol.h"
#include <QMessageBox>

#include <QDebug>
enum
{
    PORT_ENUM_BEGIN = 51400,
    PORT_ENUM_END = 52400
};

QSet<int> DebugControl::portUsed;

DebugControl::DebugControl(QString gdb, QString gdbServer, QString prog, QWidget *parent)
    : QDialog(parent), ui(new Ui::DebugControl), program(prog), status(STOP)
{
    ui->setupUi(this);
    setWindowTitle(QString("DebugControl - %1").arg(prog));
    updateStatus();
    // get port. there should be a better way to get an available port.
    port = -1;
    for (int p = PORT_ENUM_BEGIN; p < PORT_ENUM_END; ++p)
    {
        if (portUsed.find(p) == portUsed.end())
        {
            port = p;
            portUsed.insert(port);
            break;
        }
    }
    if (port == -1)
    {
        throw "too much debug dialog!"; // actually open over 1000 tabs is impossible, I think
    }
    qgdb = new qgdbint::QGdb(gdb, gdbServer, port);
    setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint |
                   Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
    connect(qgdb, &qgdbint::QGdb::positionUpdated, [&](QString, int row) { emit currentRowChanged(row); });
    connect(qgdb, &qgdbint::QGdb::textResponse, ui->log, &QPlainTextEdit::appendPlainText);
    connect(qgdb, &qgdbint::QGdb::errorOccurered, ui->errorLog, &QPlainTextEdit::appendPlainText);
    connect(qgdb, &qgdbint::QGdb::readyStdout, ui->output, &QPlainTextEdit::appendPlainText);
    connect(qgdb, &qgdbint::QGdb::exited, this, &DebugControl::onExited);
    connect(qgdb, &qgdbint::QGdb::stateChanged, this, &DebugControl::onStateChanged);
    connect(ui->startstop, &QPushButton::clicked, [&]() {
        if (status == STOP)
        {
            ui->log->clear();
            ui->errorLog->clear();
            ui->output->clear();
            qgdb->start(program, arguments, ui->input->toPlainText());
            status = PAUSE;
            updateStatus(); // here we add initial breakpoints
            qgdb->cont();
            status = RUN;
            updateStatus();
        }
        else
        {
            qgdb->terminate();
            status = STOP;
            updateStatus();
        }
    });
    connect(ui->cont, &QPushButton::clicked, qgdb, &qgdbint::QGdb::cont);
    connect(ui->stepIn, &QPushButton::clicked, qgdb, &qgdbint::QGdb::stepIn);
    connect(ui->step, &QPushButton::clicked, qgdb, &qgdbint::QGdb::step);
    connect(ui->stepOut, &QPushButton::clicked, qgdb, &qgdbint::QGdb::stepOut);
    connect(ui->addBp, &QPushButton::clicked, [&]() {
        if (status != PAUSE)
        {
            if (bpCache.indexOf(curRow) == -1)
            {
                bpCache.push_back(curRow);
                ui->pendingBp->addItem(QString("%1").arg(curRow));
            }
        }
        else
        {
            if (bps.find(curRow) == bps.end())
            {
                bps[curRow] = qgdb->setBreakpoint(curRow);
                ui->breakpoints->addItem(QString("%1").arg(curRow));
            }
        }
    });
    connect(ui->deleteBp, &QPushButton::clicked, [&]() {
        if (status == PAUSE)
        {
            auto sl = ui->breakpoints->selectedItems();
            if (sl.size() == 0)
            {
                if (QMessageBox::warning(this, "cpeditor", "remove all breakpoints?") == QMessageBox::Ok)
                {
                    bps.clear();
                    ui->breakpoints->clear();
                    qgdb->delAllBreakpoints();
                }
            }
            else
            {
                auto item = sl.front();
                int row = item->text().toInt();
                int id = bps.take(row);
                delete item; // this will remove it from list
                qgdb->delBreakpoint(id);
            }
        }
        else if (status == STOP)
        {
            auto sl = ui->pendingBp->selectedItems();
            if (sl.size() == 0)
            {
                if (QMessageBox::warning(this, "cpeditor", "remove all pending breakpoints?") == QMessageBox::Ok)
                {
                    bpCache.clear();
                    ui->pendingBp->clear();
                }
            }
            else
            {
                auto item = sl.front();
                int row = item->text().toInt();
                bpCache.removeOne(row);
                delete item;
            }
        }
    });
}

DebugControl::~DebugControl()
{
    if (status != STOP)
    {
        qgdb->terminate();
    }
    delete qgdb;
    portUsed.remove(port);
    delete ui;
}

void DebugControl::rowChanged(int row)
{
    curRow = row;
}

void DebugControl::setInput(QString in)
{
    ui->input->clear();
    ui->input->appendPlainText(in);
}

void DebugControl::setArguments(QStringList args)
{
    arguments = args;
}

void DebugControl::onStateChanged(bool running, QString reason)
{
    if (running)
        status = RUN;
    else if (QStringList{"breakpoint-hit", "function-finished", "end-stepping-range"}.contains(reason))
        status = PAUSE;
    else
        status = STOP;
    updateStatus();
}

void DebugControl::onExited()
{
    status = STOP;
    updateStatus();
}

void DebugControl::updateStatus()
{
    switch (status)
    {
    case STOP:
        bpCache = bps.keys();
        std::sort(bpCache.begin(), bpCache.end());
        bps.clear();
        ui->breakpoints->clear();
        for (auto row : bpCache)
            ui->pendingBp->addItem(QString("%1").arg(row));
        break;
    case PAUSE:
        for (auto row : bpCache)
        {
            bps[row] = qgdb->setBreakpoint(row);
            ui->breakpoints->addItem(QString("%1").arg(row));
        }
        bpCache.clear();
        ui->pendingBp->clear();
        break;
    default:;
    }
    ui->startstop->setText(status == STOP ? "start" : "stop");
    ui->cont->setEnabled(status == PAUSE);
    ui->stepIn->setEnabled(status == PAUSE);
    ui->step->setEnabled(status == PAUSE);
    ui->stepOut->setEnabled(status == PAUSE);
    ui->deleteBp->setEnabled(status != RUN);
}

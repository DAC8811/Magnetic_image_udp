#ifndef SCANNERCTR_H
#define SCANNERCTR_H

#include <QThread>
#include <QUdpSocket>
#include "timecount.h"

namespace Ui { class MainWindow; }

class ScannerCtr:public QThread
{
     Q_OBJECT
public:
    ScannerCtr(Ui::MainWindow * ui=nullptr);
    ~ScannerCtr();

    bool network_init();

    void order_write(QByteArray msg);
    void order_receive();
    void order_cycle();

    void thread_exit();

    volatile double timeuse = 0;

private:
    void run();

     volatile bool thread_running = true;

private slots:

private:

    volatile bool time_out = false;

    Ui::MainWindow* ui;
    QUdpSocket* socket;

    TimeCount timecount;

};

#endif // SCANNERCTR_H

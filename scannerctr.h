#ifndef SCANNERCTR_H
#define SCANNERCTR_H

#include <QThread>
#include <QUdpSocket>
#include "timecount.h"

namespace Ui { class MainWindow; }

enum STATE{
    STANDBY,
    TRANSMITTING,
    RECORRECT,
    RESET,
    CONNECT
};

class ScannerCtr:public QThread
{
     Q_OBJECT
public:
    ScannerCtr(Ui::MainWindow * ui=nullptr);
    ~ScannerCtr();

    bool network_init();

    void order_write(QByteArray* msg);
    void order_receive();
    bool order_cycle(uint16_t order_type,int msecs);

    bool image_receive();
    void image_transmit(int order_msecs,int image_msecs);
//    void image_ack(uint16_t err);

    void thread_exit();

    volatile double timeuse = 0;

    volatile STATE state = STANDBY;
    volatile uint32_t max_lines = 200;

private:
    void run();

    volatile bool thread_running = true;

    QByteArray* packet_data;
    QByteArray* image_data;

private slots:

private:

    volatile bool time_out = false;

    Ui::MainWindow* ui;
    QUdpSocket* socket;

    TimeCount timecount;

};

#endif // SCANNERCTR_H

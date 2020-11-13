#ifndef SCANNERCTR_H
#define SCANNERCTR_H

#include <QThread>
#include <QUdpSocket>
#include "timecount.h"
#include "imgoperator.h"
#include <QMutex>

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
    bool order_cycle(uint16_t order_type,int msecs,uint32_t max_lines = 1000);

    bool image_receive();
    void image_transmit(int order_msecs,int image_msecs);
//    void image_ack(uint16_t err);

    void thread_exit();

    volatile double timeuse = 0;

    volatile STATE state = STANDBY;
    volatile uint32_t max_lines = 400;
    QMutex mutex;
    ImgOperator* image = nullptr;

private:
    void run();

    volatile bool thread_running = true;

    QByteArray* packet_data = nullptr;
    QByteArray* image_data = nullptr;

signals:
    void send_info(QString);

private slots:
    void set_maxlines(int maxlines);
    void set_state(int);

private:

    volatile bool time_out = false;

    Ui::MainWindow* ui;
    QUdpSocket* socket = nullptr;

    TimeCount timecount;

};

#endif // SCANNERCTR_H

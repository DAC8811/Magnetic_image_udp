#include "scannerctr.h"
#include <QUdpSocket>
#include <QDebug>
#include "parameters.h"
#include "timecount.h"

ScannerCtr::ScannerCtr(Ui::MainWindow* ui):ui(ui)
{
//    network_init();
}

ScannerCtr::~ScannerCtr(){

    socket->close();
    delete socket;
}

bool ScannerCtr::network_init(){
    socket = new QUdpSocket();
    //connect(socket,SIGNAL(readyRead()),this,SLOT(order_receive()),Qt::DirectConnection);//这个一定要放在bind前面，我也不知道为什么
    if(socket->bind(QHostAddress(LOCAL_ADDRES),LOCAL_PORT)>0)
        return true;
    else{
        qDebug()<<"bind error";
        return false;
    }
}

void ScannerCtr::order_write(QByteArray msg){
//    QByteArray msg = "Hello world";
//    qDebug()<<socket->localAddress();
    if(socket->writeDatagram(msg.data(),msg.size(),QHostAddress(REMOTE_ADDRES),REMOTE_PORT)>0);
//        qDebug()<<"write success";
//    else
//        qDebug()<<"write failed";
}

void ScannerCtr::order_receive(){
    QByteArray data;
    data.resize(socket->bytesAvailable());
    socket->readDatagram(data.data(),data.size());
    qDebug()<<data;
    //qDebug()<<data[0];
}

void ScannerCtr::order_cycle(){
    msg_header* header = new msg_header;
    header->head = CONNECT_ORDER;
    header->client_address = 0xC0A80009;
    header->client_port = LOCAL_PORT;
    QByteArray data;
    data.append((char*)header,sizeof(msg_header));
    order_write(data);
    if(socket->waitForReadyRead(2000)){
        order_receive();
    }
    else
        qDebug()<<"order timeOut";
}

void ScannerCtr::run(){
    network_init();
    int i=0;
    while(thread_running){
        if(i==0){
            timecount.count_start();
        }
        order_cycle();
        i++;
        if(i>=1000){
            timeuse = timecount.count_end();
            qDebug()<<(((double)(i*3.999)/timeuse)*8);
            qDebug()<<"timeuse: "<< timeuse;
            i=0;
        }
    }
}


void ScannerCtr::thread_exit(){ thread_running = false;}


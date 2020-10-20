#include "scannerctr.h"
#include <QUdpSocket>
#include <QDebug>
#include "parameters.h"
#include "timecount.h"
#include <opencv2/opencv.hpp>


ScannerCtr::ScannerCtr(Ui::MainWindow* ui):ui(ui)
{
//    network_init();
    packet_data = new QByteArray;
    image_data = new QByteArray;
}

ScannerCtr::~ScannerCtr(){

    socket->close();
    delete packet_data;
    delete image_data;
    delete socket;
}

bool ScannerCtr::network_init(){
    socket = new QUdpSocket();
    //connect(socket,SIGNAL(readyRead()),this,SLOT(order_receive()),Qt::DirectConnection);//这个一定要放在bind前面，我也不知道为什么
    if(socket->bind(QHostAddress(LOCAL_ADDRES),LOCAL_PORT,QUdpSocket::ShareAddress))
        return true;
    else{
        qDebug()<<"bind error";
        return false;
    }
}

void ScannerCtr::order_write(QByteArray* msg){
//    if(socket->state()==QAbstractSocket::ConnectedState)
    socket->writeDatagram(msg->data(),msg->size(),QHostAddress(REMOTE_ADDRES),REMOTE_PORT);
}

void ScannerCtr::order_receive(){
   // QByteArray* data = new QByteArray;
    packet_data->clear();
    packet_data->resize(socket->bytesAvailable());
    socket->readDatagram(packet_data->data(),packet_data->size());
    ack_header* ack_handler = (ack_header*)packet_data->data();
    //qDebug()<< ack_handler->head;
    //qDebug()<<data;

    //delete data;
}

bool ScannerCtr::order_cycle(uint16_t order_type,int msecs){
    //QByteArray* data = new QByteArray;
    QByteArray data;
    //packet_data->clear();
    switch(order_type){
    case IMAGE_REQUEST:{
        image_request_header* header = new image_request_header;
        header->head = order_type;
        header->max_lines = max_lines;
        data.append((char*)header,sizeof(image_request_header));
        break;
        }
    default:{
        msg_header* header = new msg_header;
        header->head = order_type;
        header->client_address = 0xC0A80009;
        header->client_port = LOCAL_PORT;
        data.append((char*)header,sizeof(msg_header));
        break;
        }
    }
    order_write(&data);

    //delete data;

    if(socket->waitForReadyRead(msecs)){
        order_receive();
        return true;
    }
    else{
        qDebug()<<"order timeOut";
        return false;
    }
}

//void ScannerCtr::image_ack(uint16_t err){
//    QByteArray data;
//    ack_header* ack = new ack_header;
//    ack->head = ACK;
//    ack->error = err;
//    data.append((char*)ack,sizeof(ack));
//    socket->writeDatagram(data.data(),data.size(),QHostAddress(REMOTE_ADDRES),REMOTE_PORT);
//}

void ScannerCtr::image_transmit(int order_msecs,int image_msecs){
    //QByteArray* image_data = new QByteArray;
    image_data->clear();
    image_data->resize(max_lines*IMAGE_SIZE);
    bool finish = false;
    if(!order_cycle(IMAGE_REQUEST,order_msecs)){
        //qDebug()<<"transmit order send failed";
        return;
    }
//    else
//        qDebug()<<"transmit order send succeed";
    while(1){
        if(socket->waitForReadyRead(image_msecs)){
            if(image_receive()){
                finish = true;
                break;
            }
        }
        else{
            qDebug()<<"image order timeOut";
            break;
        }
    }
    if(finish){
        qDebug()<<"image receive finished";
    }
    //delete image_data;
}

bool ScannerCtr::image_receive(){
//    QByteArray* data = new QByteArray;
//    QByteArray data;
    packet_data->clear();
    packet_data->resize(socket->bytesAvailable());
    socket->readDatagram(packet_data->data(),packet_data->size());
    image_recive_header* image_handler = (image_recive_header*)packet_data->data();
    if(image_handler->head!=IMAGE_TRANSMIT){
        //delete data;
        return false;
    }
    else if(image_handler->flag==FLAG_HEAD||image_handler->flag==FLAG_MID){
        memcpy(image_data->data()+image_handler->seq_line*IMAGE_SIZE,packet_data->data()+HEADER_LENGTH,IMAGE_SIZE);
        //qDebug()<<image_handler->seq_line;
        //image_ack(ERROR_NORMAL);
        //delete data;
        return false;
    }
    else if(image_handler->flag==FLAG_TAIL){
        memcpy(image_data->data()+image_handler->seq_line*IMAGE_SIZE,packet_data->data()+HEADER_LENGTH,IMAGE_SIZE);
        //qDebug()<<image_handler->seq_line;
        //image_ack(ERROR_NORMAL);
        //delete data;
        return true;
    }
    //delete data;
    return false;
}

void ScannerCtr::run(){
    network_init();
    while(!order_cycle(ORDER_CONNECT,500));
    int i=0;
    while(thread_running){
//        if(i==0){
//            timecount.count_start();
//        }
        switch(state){
            case STANDBY:{
                usleep(1000);
                //order_cycle(ORDER_CONNECT,1000);
                break;
            }
            case TRANSMITTING:{
                image_transmit(5,1000);
//                i++;
                break;
            }
            case RESET:break;
            case RECORRECT:break;
            case CONNECT:{
                while(!order_cycle(ORDER_CONNECT,500));
                state = STANDBY;
                break;
            }
            default:break;
        };


//        i++;
//        if(i>=100){
//            timeuse = timecount.count_end();
//            qDebug()<<(((double)(i*0.8*200)/timeuse)*8);
//            qDebug()<<"timeuse: "<< timeuse;
//            i=0;
//        }
    }
}


void ScannerCtr::thread_exit(){ thread_running = false;}


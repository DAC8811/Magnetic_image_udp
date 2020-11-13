
#include "scannerctr.h"
#include <QUdpSocket>
#include <QDebug>
#include "parameters.h"
#include "timecount.h"
#include <opencv2/opencv.hpp>
#include <QStringLiteral>


ScannerCtr::ScannerCtr(Ui::MainWindow* ui):ui(ui)
{
//    network_init();
    packet_data = new QByteArray;
    image_data = new QByteArray;
    image = new ImgOperator(ui);
    //connect(this,SIGNAL(send_image_data(const char* ,uint32_t)),image,SLOT(setData(const char* ,uint32_t)));
}

ScannerCtr::~ScannerCtr(){

    socket->close();
    image->quit();
    image->wait();
    delete image;
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
    socket->writeDatagram(msg->data(),msg->size(),QHostAddress(REMOTE_ADDRES),REMOTE_PORT);
}

void ScannerCtr::order_receive(){
    packet_data->clear();
    packet_data->resize(socket->bytesAvailable());
    socket->readDatagram(packet_data->data(),packet_data->size());
    ack_header* ack_handler = (ack_header*)packet_data->data();
}

bool ScannerCtr::order_cycle(uint16_t order_type,int msecs,uint32_t max_lines){
    QByteArray data;
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
    if(socket->waitForReadyRead(msecs)){
        order_receive();
        switch(order_type){
        case IMAGE_REQUEST:break;
        case ORDER_CONNECT:{
            emit send_info(QString::fromLocal8Bit("连接成功"));
            break;
        }
        default:{
            emit send_info(QString::fromLocal8Bit("指令发送成功"));
            break;
        }
        }
        return true;
    }
    else{
        qDebug()<<"order timeOut";
        switch(order_type){
        case IMAGE_REQUEST:break;
        case ORDER_CONNECT:{
            emit send_info(QString::fromLocal8Bit("尝试连接失败,重试..."));
            break;
        }
        default:{
            emit send_info(QString::fromLocal8Bit("指令发送失败,重试..."));
            break;
        }
        }
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
    uint32_t max_line = max_lines;
    image_data->clear();
    image_data->resize(max_line*LINE_SIZE);
    bool finish = false;
    while(!order_cycle(IMAGE_REQUEST,order_msecs,max_line));
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
       // image->wait();
        image->setData(image_data->data(),max_line);
//      emit send_image_data(image_data->data(),max_line);
        image->start();
        image->wait();
    }
    //delete image_data;
}

bool ScannerCtr::image_receive(){
    packet_data->clear();
    packet_data->resize(socket->bytesAvailable());
    socket->readDatagram(packet_data->data(),packet_data->size());
    image_recive_header* image_handler = (image_recive_header*)packet_data->data();
    if(image_handler->head!=IMAGE_TRANSMIT){
        return false;
    }
    else if(image_handler->flag==FLAG_HEAD||image_handler->flag==FLAG_MID){
        memcpy(image_data->data()+image_handler->seq_line*LINE_SIZE,packet_data->data()+HEADER_LENGTH,LINE_SIZE);
        return false;
    }
    else if(image_handler->flag==FLAG_TAIL){
        memcpy(image_data->data()+image_handler->seq_line*LINE_SIZE,packet_data->data()+HEADER_LENGTH,LINE_SIZE);
        return true;
    }
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
                //usleep(1000);
                //order_cycle(ORDER_CONNECT,1000);
                break;
            }
            case TRANSMITTING:{
//                mutex.lock();
                image_transmit(5,1000000);
//                mutex.unlock();
//                i++;
                state = STANDBY;
                break;
            }
            case RESET:{
                while(!order_cycle(ORDER_RESET,500));
                state = STANDBY;
                break;
            }
        case RECORRECT:{
                while(!order_cycle(ORDER_RECORRECT,500));
                state = STANDBY;
                break;
            }
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

void ScannerCtr::set_maxlines(int maxlines){max_lines = maxlines;}

void ScannerCtr::set_state(int state){this->state = STATE(state);}

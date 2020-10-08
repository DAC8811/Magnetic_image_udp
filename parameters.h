#ifndef PARAMETERS_H
#define PARAMETERS_H


#include <QObject>

#define LOCAL_PORT 8089
#define REMOTE_PORT 8675
#define REMOTE_ADDRES "192.168.0.10"
#define LOCAL_ADDRES "192.168.0.9"

#define IMAGE_SIZE 3999
#define ORDER_LENGTH 30

//包头:8bit，其中
//前4bit区分指令发送和图像传输请求
//指令类型:4bit
//其中第一种指令用于告知PC端的IP地址和端口,其他三种指令对应磁图像管的三种操作
#define IMAGE_REQUEST 0x00;

#define CONNECT_ORDER 0x81;//发送PC端的IP地址以及端口号
#define NORMAL_ORDER 0x82
#define RESET_ORDER 0x83;
#define RECORRECT_ORDER 0x84


typedef struct msg_header{
    uint16_t head;
    uint16_t client_port;
    uint32_t client_address;

} msg_header;



#endif // PARAMETERS_H

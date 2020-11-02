#ifndef PARAMETERS_H
#define PARAMETERS_H


#include <QObject>

#define LOCAL_PORT 51925
#define REMOTE_PORT 8675
#define REMOTE_ADDRES "192.168.0.10"
#define LOCAL_ADDRES "192.168.0.9"

#define LINE_SIZE 792
#define HEADER_LENGTH 8

//head宏定义----------------------------------------------------------------------------------------------------------------------
//图像传输相关头
#define IMAGE_REQUEST 0x0000
#define IMAGE_TRANSMIT 0x0001
//指令发送相关头
#define ORDER_CONNECT 0x0081//发送PC端的IP地址以及端口号
#define ORDER_NORMAL 0x0082
#define ORDER_RESET 0x0083
#define ORDER_RECORRECT 0x0084
//应答头
#define ACK 0x0010
//flag宏定义----------------------------------------------------------------------------------------------------------------------
//图像行传输标志
#define FLAG_HEAD 0x0000
#define FLAG_MID 0x0001
#define FLAG_TAIL 0x0002
//error宏定义-----------------------------------------------------------------------------------------------------------------------
//错误状态
#define ERROR_NORMAL 0x0000
#define ERROR_HARDWARE 0x0001
#define ERROR_SOFTWARE 0x0002



typedef struct msg_header{
    uint16_t head;
    uint16_t client_port;//PC端端口
    uint32_t client_address;//PC端IP地址
} msg_header;

typedef struct image_request_header{
    uint16_t head;
    uint16_t reserve;
    uint32_t max_lines;//每次采样的总行数
}image_request_header;

typedef struct image_recive_header{
    uint16_t head;
    uint16_t flag;//标记首尾行
    uint32_t seq_line;//当前行序号
}image_recive_header;

typedef struct ack_header{
    uint16_t head;
    uint16_t error;
    uint32_t reserve;
}ack_header;



#endif // PARAMETERS_H

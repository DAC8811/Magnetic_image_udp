#include "imgoperator.h"
#include <QThread>
#include <QDebug>
#include <QImage>
#include "parameters.h"
#include "ui_mainwindow.h"


ImgOperator::ImgOperator(Ui::MainWindow * ui):ui(ui)
{
}

ImgOperator::~ImgOperator(){
    delete img;
    delete img_data;
}

void ImgOperator::setData(const char* data,uint32_t length){
    if(img_data!=nullptr)
        delete img_data;
    img_data = new QByteArray(data,length*LINE_SIZE);
    this->length = length;
}

void ImgOperator::run(){
    if(img_data==nullptr){
        qDebug()<<"error:data is empty";
        return;
    }
    if(img!=nullptr)
        delete img;
    img = new QImage((const uchar*)img_data->data(), LINE_SIZE, length,QImage::Format_Indexed8);
    //旋转90度
    QMatrix matrix;
    matrix.rotate(-90.0);
    *img = img->transformed(matrix,Qt::FastTransformation);
    ui->imageLabel->setPixmap(QPixmap::fromImage(*img));

    //不旋转的原图（竖着的）
//    ui->imageLabel->setPixmap(QPixmap::fromImage(*img));

    ui->imageLabel->resize(img->size());
    ui->imageLabel->show();
}



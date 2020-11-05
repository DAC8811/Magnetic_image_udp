#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "scannerctr.h"
#include <QIntValidator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QIntValidator* v = new QIntValidator(2, 2000, this);//设置最大行数的输入范围
    ui->lineEdit->setValidator(v);
    scanner = new ScannerCtr(ui);

    connect(this,SIGNAL(send_maxlines(int)),scanner,SLOT(set_maxlines(int)));
    connect(this,SIGNAL(send_state(int)),scanner,SLOT(set_state(int)));
    connect(scanner->image,SIGNAL(send_img(QImage)),this,SLOT(show_img(QImage)));

    scanner->start();
    //scanner->init();
}

MainWindow::~MainWindow()
{
    scanner->thread_exit();
    scanner->quit();
    scanner->exit();
//    delete scanner;
    delete ui;
}


void MainWindow::on_transStartBtn_clicked()
{
   // scanner->state = TRANSMITTING;
    emit send_state(TRANSMITTING);
}

void MainWindow::on_resetBtn_clicked()
{
    //scanner->state = RESET;
    emit send_state(RESET);
}

void MainWindow::on_recorrectBtn_clicked()
{
    //scanner->state = RECORRECT;
    emit send_state(RECORRECT);
}

void MainWindow::on_transStopBtn_clicked()
{
    //scanner->state = STANDBY;
    emit send_state(STANDBY);
}

void MainWindow::on_connectBtn_clicked()
{
    //scanner->state = CONNECT;
    emit send_state(CONNECT);
}

void MainWindow::on_lineConfirmBtn_clicked()
{
//    scanner->mutex.lock();
//    scanner->max_lines = ui->lineEdit->text().toUInt();
//    scanner->mutex.unlock();
    emit send_maxlines(ui->lineEdit->text().toUInt());
}

void MainWindow::show_img(QImage img){
    ui->imageLabel->setPixmap(QPixmap::fromImage(img));
    ui->imageLabel->resize(img.size());
    ui->imageLabel->show();
}

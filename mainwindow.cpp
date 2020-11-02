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
    scanner->state = TRANSMITTING;
}

void MainWindow::on_resetBtn_clicked()
{
    scanner->state = RESET;
}

void MainWindow::on_recorrectBtn_clicked()
{
    scanner->state = RECORRECT;
}

void MainWindow::on_transStopBtn_clicked()
{
    scanner->state = STANDBY;
}

void MainWindow::on_connectBtn_clicked()
{
    scanner->state = CONNECT;
}

void MainWindow::on_lineConfirmBtn_clicked()
{
    scanner->max_lines = ui->lineEdit->text().toUInt();
}

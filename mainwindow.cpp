#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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


void MainWindow::on_sendBtn_clicked()
{
    scanner->order_write("hello world");
}

﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "scannerctr.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_transStartBtn_clicked();

    void on_resetBtn_clicked();

    void on_recorrectBtn_clicked();

    void on_transStopBtn_clicked();
    
    void on_connectBtn_clicked();

    void on_lineConfirmBtn_clicked();

    void show_img(QImage img);

    void show_string(QString info);

    void on_clearInfoBtn_clicked();

private:
    Ui::MainWindow *ui;
    ScannerCtr* scanner;

signals:
    void send_maxlines(int maxlines);
    void send_state(int);


};
#endif // MAINWINDOW_H

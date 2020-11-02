#ifndef IMGOPERATOR_H
#define IMGOPERATOR_H

#include <QThread>

namespace Ui { class MainWindow; }

class ImgOperator:public QThread
{
    Q_OBJECT
public:
    ImgOperator(Ui::MainWindow * ui=nullptr);
    ~ImgOperator();
    void setData(const char* data,uint32_t length);

private:
    void run();

    QByteArray* img_data = nullptr;
    QImage* img = nullptr;
    Ui::MainWindow* ui;
    uint32_t length;
};


#endif // IMGOPERATOR_H

#include "mainwindow.h"
#include <QApplication>
#include <QLabel>
#include <OpenCL/opencl.h>
#include <OpenCLHelper.h>

#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    int width = w.width();
    int height = w.height();

    size_t memLength = width * height * 4;
    uchar data[memLength];

    size_t global_work_size[2];
    global_work_size[0] = width;
    global_work_size[1] = height;

    OpenCLHelper helper("/Users/17aleshka/QtProjects/GraphicsHW/kernel.cl");

    helper.setParameter(0, memLength  * sizeof(uchar), data, true);
    helper.setParameter(1, sizeof(width), &width, false);
    helper.execute(2, global_work_size, nullptr);
    helper.readParameter(0, memLength  * sizeof(uchar), data);

    QImage img(data, width, height, QImage::Format_ARGB32);

    QLabel label(&w);
    label.setMinimumSize(width, height);
    label.setPixmap(QPixmap::fromImage(img));

    w.show();

    return a.exec();
}

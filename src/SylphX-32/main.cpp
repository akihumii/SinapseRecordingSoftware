#include "mainwindow.h"
#include <QApplication>
#include "serialchannel.h"
#include "ringbuffer.h"

ringbuffer<uint8_t> *rb;
SerialChannel *serialChannel;

void *readInput(void *arg){
    serialChannel = new SerialChannel(this);
    serialChannel->connectSylph();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    rb = new ringbuffer<uint8_t>(40960);
    if(rb->getFree() < 0){
        qDebug() << "Ring buffer could not be created!";
    }
    else{
        qDebug() << "Ring buffer successfully created!";
    }
    MainWindow w;
    w.show();

    return a.exec();
}

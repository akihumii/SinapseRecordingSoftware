#include "socketsylph.h"

SocketSylph::SocketSylph(DataProcessor *dataProcessor_){
    dataProcessor = dataProcessor_;
    timer = new QTimer;
    connect(socketAbstract, SIGNAL(readyRead()), this, SLOT(ReadCommand()));
    connect(timer, SIGNAL(timeout()), this, SLOT(updateRate()));
    timer->start(1000);
}

//void SocketSylph::discardData(){
//    for(int i = 0; i < 50; i++){
//        socketAbstract->read(48000);
//        qDebug() << "Discarding";
//    }
//    checked = false;
//}

void SocketSylph::ReadCommand(){
/*    if(initCount < 100){
        discardData();
        initCount++;
    }
    else */if(socketAbstract->bytesAvailable() >= maxSize && checked){
//        qDebug() << "Reading data";
        dataProcessor->parseFrameMarkers(socketAbstract->read(maxSize));
    }
    else if(socketAbstract->bytesAvailable() >= 106 && !checked){
        qDebug() << "checking";
        if(dataProcessor->checkNextFrameMarker(socketAbstract->read(106), 0)){
            checked = true;
            qDebug() << "checked is true";
        }
    }
}

void SocketSylph::updateRate(){
    rate = bytesRead*8/1000;
    if(rate == 0 && checked){
        checked = false;
    }
    bytesRead = 0;
}

int SocketSylph::getRate(){
    return rate;
}

void SocketSylph::setChecked(bool flag){
    checked = flag;
}

void SocketSylph::appendSync(){
    qDebug() << "Sync pulse detected!";
}

void SocketSylph::closeESP(){
    qDebug() << "Closing ESP";
    QByteArray closingMSG = "DISCONNECT!!!!!!";
    socketAbstract->write(closingMSG);
    socketAbstract->waitForBytesWritten(1000);
}

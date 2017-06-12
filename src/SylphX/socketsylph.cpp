#include "socketsylph.h"

SocketSylph::SocketSylph(DataProcessor *dataProcessor_){
    dataProcessor = dataProcessor_;
    connect(socketAbstract, SIGNAL(readyRead()), this, SLOT(ReadCommand()));
}

void SocketSylph::discardData(){
    for(int i = 0; i < 50; i++){
        socketAbstract->read(48000);
        qDebug() << "Discarding";
    }
    checked = false;
}
void SocketSylph::ReadCommand(){
    if(initCount < 100){
        discardData();
        initCount++;
    }
    else if(socketAbstract->bytesAvailable() >= maxSize && checked){
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

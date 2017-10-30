#include "socketsylph.h"

namespace SylphX {

SocketSylph::SocketSylph(DataProcessor *dataProcessor_){
    dataProcessor = dataProcessor_;
    connect(socketAbstract, SIGNAL(readyRead()), this, SLOT(ReadCommand()));
}

void SocketSylph::discardData(){
    for(int i = 0; i < 50; i++){
        socketAbstract->read(48000);
//        qDebug() << "Discarding";
    }
    checked = false;
}
void SocketSylph::ReadCommand(){
    if(initCount < 100){
        discardData();
        initCount++;
    }
    else if(socketAbstract->bytesAvailable() >= maxSize && checked){
        if(dataProcessor->isSmart()){
            dataProcessor->parseFrameMarkersWithChecks(socketAbstract->read(maxSize));
        }
        else{
            dataProcessor->parseFrameMarkers(socketAbstract->read(maxSize));
        }
    }
    else if(socketAbstract->bytesAvailable() >= 89 && !checked){
        qDebug() << "checking";
        if(dataProcessor->checkNextFrameMarker(socketAbstract->read(89), 0)){
            checked = true;
            qDebug() << "checked is true";
        }
    }
}

void SocketSylph::appendSync(){
    qDebug() << "Sync pulse detected!";
//    socketAbstract->write(QByteArray::number(255, 10));
}

void SocketSylph::closeESP(){
    qDebug() << "Closing ESP";
    QByteArray closingMSG = "Closing ESP";
    socketAbstract->write(closingMSG);
}

}

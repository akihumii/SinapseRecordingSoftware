#include "socketsylph.h"

namespace SylphX {

SocketSylph::SocketSylph(DataProcessor *dataProcessor_){
    dataProcessor = dataProcessor_;
    timer = new QTimer;
    connect(socketAbstract, SIGNAL(readyRead()), this, SLOT(ReadCommand()));
    connect(timer, SIGNAL(timeout()), this, SLOT(updateRate()));
    timer->start(1000);
}

void SocketSylph::discardData(){
    for(int i = 0; i < 10; i++){
        socketAbstract->read(maxSize*10);
        qDebug() << "Discarding";
    }
    checked = false;
}
void SocketSylph::ReadCommand(){
    if(initCount < 20){
        discardData();
        initCount++;
    }
    else if(socketAbstract->bytesAvailable() >= maxSize && checked){
        if(dataProcessor->isSmart()){
            bytesRead += dataProcessor->parseFrameMarkersWithChecks(socketAbstract->read(maxSize));
        }
        else{
            bytesRead += dataProcessor->parseFrameMarkers(socketAbstract->read(maxSize));
        }
    }
    else if(socketAbstract->bytesAvailable() >= packetSize+1 && !checked){
        qDebug() << "checking";
        if(dataProcessor->checkNextFrameMarker(socketAbstract->read(packetSize+1), 0)){
            checked = true;
            qDebug() << "checked is true";
        }
    }
}

void SocketSylph::updateRate(){
    rate = bytesRead*8/1000;
    if(rate == 0 && checked){
        initCount = 0;
        checked = false;
    }
    bytesRead = 0;
}

int SocketSylph::getRate(){
    return rate;
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

}

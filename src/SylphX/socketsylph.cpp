#include "socketsylph.h"

namespace SylphX {

SocketSylph::SocketSylph(DataProcessor *dataProcessor_){
    dataProcessor = dataProcessor_;
    timer = new QTimer;
    connect(socketAbstract, SIGNAL(readyRead()), this, SLOT(ReadCommand()));
    connect(timer, SIGNAL(timeout()), this, SLOT(updateRate()));
    timer->start(1000);

}

void SocketSylph::ReadCommand(){
    if(socketAbstract->bytesAvailable() >= maxSize && checked){
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
        else{
            ReadCommand();
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

}

#include "socketsylph.h"

namespace SylphX {

SocketSylph::SocketSylph(DataProcessor *dataProcessor_){
    dataProcessor = dataProcessor_;
    timer = new QElapsedTimer;
    connect(socketAbstract, SIGNAL(readyRead()), this, SLOT(ReadCommand()));
}

void SocketSylph::discardData(){
    for(int i = 0; i < 10; i++){
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
        if(dataProcessor->isSmart()){
            dataProcessor->parseFrameMarkersWithChecks(socketAbstract->read(maxSize));
        }
        else{
//        if(dataProcessor->checkNextFrameMarker(socketAbstract->read(93))){
            dataProcessor->parseFrameMarkers(socketAbstract->read(maxSize));
        }
    }
    else if(socketAbstract->bytesAvailable() >= packetSize+1 && !checked){
        qDebug() << "checking";
        if(dataProcessor->checkNextFrameMarker(socketAbstract->read(packetSize+1), 0)){
            checked = true;
            qDebug() << "checked is true";
        }
    }
    rate = (0.01 * (double) socketAbstract->bytesAvailable()*1000.0/ (double) timer->elapsed()) + (0.99 * rate);
    timer->start();
}

double SocketSylph::getRate(){
    return rate;
}

void SocketSylph::appendSync(){
    qDebug() << "Sync pulse detected!";
}

void SocketSylph::closeESP(){
    qDebug() << "Closing ESP";
    QByteArray closingMSG = "DISCONNECT!!!!!!!";
    socketAbstract->write(closingMSG);
    socketAbstract->waitForBytesWritten(1000);
}

}

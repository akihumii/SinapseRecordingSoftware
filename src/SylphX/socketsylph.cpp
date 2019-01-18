#include "socketsylph.h"

namespace SylphX {

SocketSylph::SocketSylph(DataProcessor *dataProcessor_){
    dataProcessor = dataProcessor_;
    connect(socketAbstract, SIGNAL(readyRead()), this, SLOT(readData()));
}

void SocketSylph::readData(){
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
            readData();
        }
    }
}

}

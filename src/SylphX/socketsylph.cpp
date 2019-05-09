#include "socketsylph.h"

namespace SylphX {

SocketSylph::SocketSylph(DataProcessor *dataProcessor_){
    dataProcessor = dataProcessor_;
    connect(socketAbstract, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(dataProcessor, SIGNAL(dataLost()), this, SLOT(resyncData()));

//    player = new QMediaPlayer;
//    player->setMedia(QUrl::fromLocalFile(QDir::currentPath() +QDir::separator()+ "coins.mp3"));
//    player->setVolume(50);
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

void SocketSylph::writeCommand(QByteArray command){
//    if(command.size() > 1){
//        player->play();
//    }
    if(this->isConnected()){
        socketAbstract->write(command);
        qDebug() << "Sent command of a size" << command.size() << "via TCP socket: " << (quint8) command.at(0) << (quint8) command.at(1);
    }
}

void SocketSylph::resyncData(){
    checked = false;
}

}

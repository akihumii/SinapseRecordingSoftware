#include "socketsylph.h"

SocketSylph::SocketSylph(QObject *parent, DataProcessor *dataProcessor_) : QObject(parent){
    socketSylph = new QTcpSocket(this);
    dataProcessor = dataProcessor_;

    connect(socketSylph, SIGNAL(connected()), this, SLOT(connectedCommandSocket()));
    connect(socketSylph, SIGNAL(disconnected()), this, SLOT(disconnectedCommandSocket()));
    connect(socketSylph, SIGNAL(readyRead()), this, SLOT(ReadCommand()));
}

void SocketSylph::doConnect(QString ipAddress, int port){
    int connectionTries = 0;
    while(socketSylph->state() != QAbstractSocket::ConnectedState){
        qDebug() << "Connecting...";
        socketSylph->connectToHost(ipAddress, port);
        qDebug() << "Waiting...";
        if(!socketSylph->waitForConnected(1000)){
            qDebug() << "Error: " << socketSylph->errorString();
        }
        connectionTries++;
        if(connectionTries>2)
            return;
    }
    for(int i = 0; i < 10; i++){
        socketSylph->flush();
        socketSylph->read(48000);
        qDebug() << "Discarding";
    }
    checked = false;
}

void SocketSylph::connectedCommandSocket(){
    qDebug() << "Command Socket Connected!";
}

void SocketSylph::disconnectedCommandSocket(){
    qDebug() << "Command Socket Disconnected";
}

void SocketSylph::ReadCommand(){
    if(/*socketSylph->bytesAvailable() >= maxSize &&*/ checked){
        qDebug() << "Reading data";
        dataProcessor->parseFrameMarkers(socketSylph->read(maxSize));
    }
    else if(socketSylph->bytesAvailable() >= 106 && !checked){
        qDebug() << "checking";
        if(dataProcessor->checkNextFrameMarker(socketSylph->read(106), 0)){
            checked = true;
            qDebug() << "checked is true";
        }
    }
}

void SocketSylph::doDisconnect(){
    if(socketSylph->state() == QAbstractSocket::ConnectedState){
        socketSylph->flush();
        socketSylph->disconnectFromHost();
        socketSylph->close();
        qDebug()<<"Disconnect clicked";
    }
}

bool SocketSylph::isConnected(){
    if (socketSylph->state() == QAbstractSocket::ConnectedState){
        return true;
    }
    else return false;
}

QString SocketSylph::getError(){
    return socketSylph->errorString();
}

#include "socketsylph.h"

SocketSylph::SocketSylph(QObject *parent, DataProcessor *dataProcessor_) : QObject(parent){
    socketSylph = new QTcpSocket(this);
    dataProcessor = dataProcessor_;

    connect(socketSylph, SIGNAL(connected()), this, SLOT(connectedCommandSocket()));
    connect(socketSylph, SIGNAL(disconnected()), this, SLOT(disconnectedCommandSocket()));
    connect(socketSylph, SIGNAL(readyRead()), this, SLOT(ReadCommand()));
}

void SocketSylph::doConnect(QString ipAddress, int port){
    while(socketSylph->state() != QAbstractSocket::ConnectedState){
        qDebug() << "Connecting...";
        socketSylph->connectToHost(ipAddress, port);
        qDebug() << "Waiting...";
        if(!socketSylph->waitForConnected(5000)){
            qDebug() << "Error: " << socketSylph->errorString();
        }
    }
}

void SocketSylph::connectedCommandSocket(){
    qDebug() << "Command Socket Connected!";
}

void SocketSylph::disconnectedCommandSocket(){
    qDebug() << "Command Socket Disconnected";
}

void SocketSylph::ReadCommand(){
    if(socketSylph->bytesAvailable() >= 10*maxSize/10){
        dataProcessor->parseFrameMarkers(socketSylph->read(maxSize));
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

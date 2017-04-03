#include "socketedison.h"

SocketEdison::SocketEdison(QObject *parent, DataProcessor *dataProcessor_) : QObject(parent){
    socketCommand = new QTcpSocket(this);
    dataProcessor = dataProcessor_;

    connect(socketCommand, SIGNAL(connected()), this, SLOT(connectedCommandSocket()));
    connect(socketCommand, SIGNAL(disconnected()), this, SLOT(disconnectedCommandSocket()));
    connect(socketCommand, SIGNAL(readyRead()), this, SLOT(ReadCommand()));
}

void SocketEdison::doConnect(QString ipAddress, int port){
    while(socketCommand->state() != QAbstractSocket::ConnectedState){
        qDebug() << "Connecting...";
        socketCommand->connectToHost(ipAddress, port);
        qDebug() << "Waiting...";
        if(!socketCommand->waitForConnected(5000)){
            qDebug() << "Error: " << socketCommand->errorString();
        }
    }
}

void SocketEdison::connectedCommandSocket(){
    qDebug() << "Command Socket Connected!";
}

void SocketEdison::disconnectedCommandSocket(){
    qDebug() << "Command Socket Disconnected";
}

void SocketEdison::ReadCommand(){
    if(socketCommand->bytesAvailable() > 5*maxSize/10){
        dataProcessor->parseFrameMarkers(socketCommand->read(maxSize));
    }
}

void SocketEdison::doDisconnect(){
    if(socketCommand->state() == QAbstractSocket::ConnectedState){
        socketCommand->flush();
        socketCommand->disconnectFromHost();
        socketCommand->close();
        qDebug()<<"Disconnect clicked";
    }
}

bool SocketEdison::isConnected(){
    if (socketCommand->state() == QAbstractSocket::ConnectedState){
        return true;
    }
    else return false;
}

QString SocketEdison::getError(){
    return socketCommand->errorString();
}

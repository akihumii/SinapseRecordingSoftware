#include "socketabstract.h"

SocketAbstract::SocketAbstract(){
    socketAbstract = new QTcpSocket(this);
}

bool SocketAbstract::doConnect(QString ipAddress, int port){
    int connectionTries = 0;
    while(socketAbstract->state() != QAbstractSocket::ConnectedState){
        qDebug() << "Connecting...";
        socketAbstract->connectToHost(ipAddress, port);
        qDebug() << "Waiting...";
        if(!socketAbstract->waitForConnected(1000)){
            qDebug() << "Error: " << socketAbstract->errorString();
        }
        connectionTries++;
        if(connectionTries>2)
            return false;
    }
    return true;
}

void SocketAbstract::doDisconnect(){
    if(socketAbstract->state() == QAbstractSocket::ConnectedState){
        socketAbstract->flush();
        socketAbstract->disconnectFromHost();
        socketAbstract->close();
    }
}

bool SocketAbstract::isConnected(){
    return (socketAbstract->state() == QAbstractSocket::ConnectedState);
}

QString SocketAbstract::getError(){
    return socketAbstract->errorString();
}

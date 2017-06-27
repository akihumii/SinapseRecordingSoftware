#include "socketabstract.h"

SocketAbstract::SocketAbstract(){
    socketAbstract = new QTcpSocket(this);

    connect(socketAbstract, SIGNAL(connected()), this, SLOT(connectedAbstractSocket()));
    connect(socketAbstract, SIGNAL(disconnected()), this, SLOT(disconnectedAbstractSocket()));
}

void SocketAbstract::connectedAbstractSocket(){
    qDebug() << "Abstract Socket Connected!";
}

void SocketAbstract::disconnectedAbstractSocket(){
    qDebug() << "Abstract Socket Disconnected";
}

void SocketAbstract::doConnect(QString ipAddress, int port){
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
            return;
    }
}

void SocketAbstract::doDisconnect(){
    if(socketAbstract->state() == QAbstractSocket::ConnectedState){
        socketAbstract->flush();
        socketAbstract->disconnectFromHost();
        socketAbstract->close();
        qDebug()<<"Disconnect clicked";
    }
}

bool SocketAbstract::isConnected(){
    return (socketAbstract->state() == QAbstractSocket::ConnectedState);
}

QString SocketAbstract::getError(){
    return socketAbstract->errorString();
}

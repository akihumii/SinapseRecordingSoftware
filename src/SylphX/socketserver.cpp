#include "socketserver.h"

SocketServer::SocketServer(){
    server= new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(on_dynoPort_connect()));

    server->listen(QHostAddress::LocalHost, 45454)? qDebug() << "Server is started" : qDebug() << "Server could not started";
}

void SocketServer::getDynoData(){
    float d;
    QDataStream stream(socketDyno->readAll());
    stream.setByteOrder(QDataStream::BigEndian);
    stream >> d;
//    qDebug() << d;
    emit dynoDataReady(d);
}

void SocketServer::on_dynoPort_connect(){
    socketDyno = new QTcpSocket(this);
    socketDyno = server->nextPendingConnection();
    connect(socketDyno, SIGNAL(disconnected()), socketDyno, SLOT(deleteLater()));
    connect(socketDyno, SIGNAL(readyRead()), this, SLOT(getDynoData()));
    qDebug() << "Connected";
}

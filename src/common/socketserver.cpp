#include "socketserver.h"

SocketServer::SocketServer(){
    server= new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(on_port_connect()));

//    server->listen(QHostAddress::LocalHost, 45454)? qDebug() << "Server is started" : qDebug() << "Server could not started";
}

void SocketServer::getData(){
    float d;
    QDataStream stream(socket->readAll());
    stream.setByteOrder(QDataStream::BigEndian);
    stream >> d;
//    qDebug() << d;
    emit dataReady(d);
}

void SocketServer::on_port_connect(){
    socket = new QTcpSocket(this);
    socket = server->nextPendingConnection();
    connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(getData()));
    qDebug() << "Connected";
}

void SocketServer::doListen(){
    if(QString::compare(ipAddress, "LocalHost", Qt::CaseInsensitive) == 0){
        server->listen(QHostAddress::LocalHost, portNumber)? qDebug() << "Server is started" : qDebug() << "Server could not started";
    }
    else{
        server->listen(QHostAddress(ipAddress), portNumber)? qDebug() << "Server is started" : qDebug() << "Server could not started";
    }
}

void SocketServer::setIpAddress(QString address){
    ipAddress = address;
}

void SocketServer::setPortNumber(int value){
    portNumber = value;
}

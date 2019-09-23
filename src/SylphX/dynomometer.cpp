#include "dynomometer.h"

Dynomometer::Dynomometer(){
    server= new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(on_dynoPort_connect()));

    server->listen(QHostAddress::LocalHost, 45454)? qDebug() << "Server is started" : qDebug() << "Server could not started";
}

void Dynomometer::getDynoData(){
    float d;
    QDataStream stream(socketDyno->readAll());
    stream.setByteOrder(QDataStream::BigEndian);
    stream >> d;
//    qDebug() << d;
    emit dynoDataReady(d);
}

void Dynomometer::on_dynoPort_connect(){
    socketDyno = new QTcpSocket(this);
    socketDyno = server->nextPendingConnection();
    connect(socketDyno, SIGNAL(disconnected()), socketDyno, SLOT(deleteLater()));
    connect(socketDyno, SIGNAL(readyRead()), this, SLOT(getDynoData()));
    qDebug() << "Dynamometer is connected...";
}

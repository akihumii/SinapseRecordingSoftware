#include "socketforce.h"

namespace SylphX {

SocketForce::SocketForce(){
    out = new QDataStream(socketAbstract);
}

void SocketForce::streamData(double data){
    *out << data;
}

void SocketForce::streamData(QVector<double> data){
    for(int i = 0; i < data.size(); i++){
        *out << data.at(i);
    }
}

void SocketForce::setServer(int port){
    server= new QTcpServer(this);
    socketForce = new QTcpSocket(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(on_forcePort_connect()));

    qDebug() << "creating localhost server with port: " << port;
    server->listen(QHostAddress::LocalHost, port)? qDebug() << "Force server is started" : qDebug() << "Force server could not started";
}

void SocketForce::on_forcePort_connect(){
    socketForce = new QTcpSocket(this);
    socketForce = server->nextPendingConnection();
    out = new QDataStream(socketForce);
    connect(socketForce, SIGNAL(disconnected()), this, SLOT(on_disconnect()));
    connect(socketForce, SIGNAL(disconnected()), socketForce, SLOT(deleteLater()));
    qDebug() << "Force Sensor is connected...";
}

void SocketForce::on_disconnect(){
    out = new QDataStream(socketAbstract);
    socketForce = new QTcpSocket(this);
}

bool SocketForce::isConnectedSocketForce(){
    return socketForce->isOpen();
}
}

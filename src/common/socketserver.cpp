#include "socketserver.h"

SocketServer::SocketServer(){
    server= new QTcpServer(this);
    connect(server, &QTcpServer::newConnection, this, &SocketServer::on_port_connect);

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

void SocketServer::writeData(QByteArray data){
    socket->write(data);
    socket->waitForBytesWritten();
}

void SocketServer::streamData(QVector<QString> *data){
    if(connectedFlag){
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        for(int i = 1; i < data->size(); i++){
            out << data->at(i);
        }
        socket->write(block);
        socket->disconnectFromHost();
    }
}

void SocketServer::on_port_connect(){
    socket = new QTcpSocket(this);
    socket = server->nextPendingConnection();
    connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
    connect(socket, SIGNAL(disconnected()), socket, SLOT(disableConnectedFlag()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(getData()));
    qDebug() << ipAddress << "::" << portNumber << " is connected...";
    connectedFlag = true;
}

void SocketServer::doListen(){
    if(QString::compare(ipAddress, "LocalHost", Qt::CaseInsensitive) == 0){
        qDebug() << "ipAddress: " << ipAddress << " port number: " << portNumber;
        server->listen(QHostAddress::LocalHost, portNumber)?
                    qDebug() << "Server is started!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" : qDebug() << "Server could not started!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
    }
    else{
        QString ip;
        QList<QHostAddress> ipAddressList = QNetworkInterface::allAddresses();
        // use the first non-localhost IPv4 address
        for(int i = 0; i < ipAddressList.size(); i++){
            if(ipAddressList.at(i) != QHostAddress::LocalHost && ipAddressList.at(i).toIPv4Address()){
                ipAddress = ipAddressList.at(i).toString();
                break;
            }
        }
        qDebug() << "ipAddress: " << ipAddress << " port number: " << portNumber;
        server->listen(QHostAddress(ipAddress), portNumber)?
                    qDebug() << "Server is started!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" : qDebug() << "Server could not started!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
    }
}

void SocketServer::setIpAddress(QString address){
    ipAddress = address;
}

void SocketServer::setPortNumber(int value){
    portNumber = value;
}

void SocketServer::disableConnectedFlag(){
    connectedFlag = false;
}

bool SocketServer::isConnected(){
    return connectedFlag;
}

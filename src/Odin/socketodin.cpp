#include "socketodin.h"

SocketOdin::SocketOdin(QObject *parent) : QObject(parent){
    socketOdin = new QTcpSocket(this);

    connect(socketOdin, SIGNAL(connected()), this, SLOT(connectedCommandSocket()));
    connect(socketOdin, SIGNAL(disconnected()), this, SLOT(disconnectedCommandSocket()));
}

void SocketOdin::doConnect(QString ipAddress, int port){
    int connectionTries = 0;
    while(socketOdin->state() != QAbstractSocket::ConnectedState){
        qDebug() << "Connecting...";
        socketOdin->connectToHost(ipAddress, port);
        qDebug() << "Waiting...";
        if(!socketOdin->waitForConnected(1000)){
            qDebug() << "Error: " << socketOdin->errorString();
        }
        connectionTries++;
        if(connectionTries>2)
            return;
    }
}

void SocketOdin::connectedCommandSocket(){
    qDebug() << "Command Socket Connected!";
}

void SocketOdin::disconnectedCommandSocket(){
    qDebug() << "Command Socket Disconnected";
}

bool SocketOdin::isConnected(){
    if (socketOdin->state() == QAbstractSocket::ConnectedState){
        return true;
    }
    else return false;
}

void SocketOdin::initOdin(){
    socketOdin->write(QByteArray::fromHex("F8"));
    timer = new QElapsedTimer;
    timer->start();
    while (timer->elapsed() < 2500);
    qDebug() << "Odin Initialised!";
}

void SocketOdin::writeCommand(QByteArray command){
    for(int i = 0; i < 16; i++){
        qDebug() << "Sending Byte " << i;
        QByteArray sending;
        sending.clear();
        qDebug("%x", (quint8) command.at(i));
        sending.append(command.at(i));
        socketOdin->write(sending);
        QThread::msleep(15);
    }
}

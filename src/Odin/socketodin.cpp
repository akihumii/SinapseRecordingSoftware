#include "socketodin.h"

SocketOdin::SocketOdin(QObject *parent) : QObject(parent){
    socketOdin = new QTcpSocket(this);

    connect(&commandTimer, SIGNAL(timeout()), this, SLOT(sendCommand()));
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
//    socketOdin->write(QByteArray::fromHex("F8"));
    timer.start();
    while (timer.elapsed() < 1700);
    qDebug() << "Odin Initialised!";
}

void SocketOdin::writeCommand(QByteArray command){
    qDebug() << "Started command timer";
    outgoingCommand = command;
    commandTimer.start(15);
}

void SocketOdin::sendCommand(){
    qDebug() << "Sending Byte " << commandCount;
    QByteArray sending;
    sending.append(outgoingCommand.at(commandCount));
    socketOdin->write(sending);
    commandCount++;
    if(commandCount >= 16){
        commandTimer.stop();
        commandCount = 0;
        qDebug() << "Finished sending command";
    }
}

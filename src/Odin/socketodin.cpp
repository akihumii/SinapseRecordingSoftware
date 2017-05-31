#include "socketodin.h"

SocketOdin::SocketOdin(QObject *parent) : QObject(parent){
    socketOdin = new QTcpSocket(this);

    connect(socketOdin, SIGNAL(connected()), this, SLOT(connectedCommandSocket()));
    connect(socketOdin, SIGNAL(disconnected()), this, SLOT(disconnectedCommandSocket()));

    connect(&commandTimer, SIGNAL(timeout()), this, SLOT(sendCommand()));
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
    emit odinDisconnected();
    qDebug() << "Command Socket Disconnected";
}

bool SocketOdin::isConnected(){
    if (socketOdin->state() == QAbstractSocket::ConnectedState){
        qDebug() << "Checked connection state";
        return true;
    }
    else return false;
}

void SocketOdin::initOdin(){
    qDebug() << "Odin Initialised!";
}

void SocketOdin::writeCommand(QByteArray command){
    qDebug() << "Started command timer";
    outgoingCommand = command;
    socketOdin->flush();
    commandTimer.start(15);
}

void SocketOdin::sendCommand(){
    qDebug() << "Sending Byte " << commandCount << "of byte" << outgoingCommand.size();
    QByteArray sending;
    sending.clear();
    qDebug("%x", (quint8) outgoingCommand.at(commandCount));
    sending.append(outgoingCommand.at(commandCount));
    socketOdin->write(sending);
    commandCount++;
    if(commandCount >= outgoingCommand.size()){
        commandTimer.stop();
        commandCount = 0;
        qDebug() << "Finished sending command";
    }
}

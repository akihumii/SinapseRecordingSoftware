#include "socketodin.h"

namespace Odin {

SocketOdin::SocketOdin(){
    udpSocket = new QUdpSocket(this);

    udpSocket->bind(QHostAddress::Broadcast, 45454);

    qDebug() << "Binded UDP Socket";
    connect(udpSocket, SIGNAL(disconnected()), this, SLOT(on_socketDisconnected()));

    player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile(QDir::currentPath() +QDir::separator()+ "coins.mp3"));
    player->setVolume(50);

    serverMatlabPredict = new QTcpServer(this);
    connect(serverMatlabPredict, SIGNAL(newConnection()), this, SLOT(on_predictionPort_connect()));
    if(serverMatlabPredict->listen(QHostAddress::LocalHost, port)){
        qDebug() << "Server for prediction is started";
    }
    else{
        qDebug() << "Server for prediction could not started";
    }
}
SocketOdin::~SocketOdin(){
}

void SocketOdin::on_predictionPort_connect(){
    socketMatlabPredict = new QTcpSocket(this);
    socketMatlabPredict = serverMatlabPredict->nextPendingConnection();
    connect(socketMatlabPredict, SIGNAL(disconnected()), socketMatlabPredict, SLOT(deleteLater()));
    connect(socketMatlabPredict, SIGNAL(readyRead()), this, SLOT(readPrediction()));
}

void SocketOdin::readPrediction(){
    if(socketMatlabPredict->bytesAvailable() > 0){
        writeCommand(socketMatlabPredict->readAll());
    }
}

void SocketOdin::writeCommand(QByteArray command){
    if(command.size() > 1){
        player->play();
    }
    outgoingCommand = command;
    this->isConnected()? socketAbstract->write(command) : udpSocket->writeDatagram(command, command.size(), QHostAddress::Broadcast, 45454);
    qDebug() << "Sent command of a size" << command.size() << "via udp socket: " << (quint8) command.at(0) << (quint8) command.at(1);
}

void SocketOdin::sendDisconnectSignal(){
    qDebug() << "Sending disconnect signals";
    QByteArray temp;
    temp.append("DISCONNECT!!!!!!!");
    socketAbstract->write(temp);
    socketAbstract->waitForBytesWritten(1000);
}

void SocketOdin::on_socketDisconnected(){
    emit odinDisconnected();
}

QByteArray SocketOdin::getIncomingCommand(){
    return incomingCommand;
}

QByteArray SocketOdin::getOutgoingCommand(){
    return outgoingCommand;
}

void SocketOdin::setReadDelay(int delay){
    readDelay = delay;
}

}

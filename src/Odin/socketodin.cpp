#include "socketodin.h"

namespace Odin {

SocketOdin::SocketOdin(){
    udpSocket = new QUdpSocket(this);

    udpSocket->bind(QHostAddress::Broadcast, 45454);

    qDebug() << "Binded UDP Socket";

    connect(&commandTimer, SIGNAL(timeout()), this, SLOT(sendCommand()));
    connect(udpSocket, SIGNAL(disconnected()), this, SLOT(on_socketDisconnected()));
    connect(socketAbstract, SIGNAL(readyRead()), this, SLOT(readCommand()));

    player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile(QDir::currentPath() +QDir::separator()+ "coins.mp3"));
    player->setVolume(50);
}
SocketOdin::~SocketOdin(){
}

void SocketOdin::writeCommand(QByteArray command){
    if(command.size() > 1){
        player->play();
    }
    outgoingCommand = command;
    udpSocket->writeDatagram(command, command.size(), QHostAddress::Broadcast, 45454);
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

void SocketOdin::readCommand(){
    qDebug() << "Reading";
//    if(timeToRead){
        incomingCommand.append(socketAbstract->readAll());
        if(incomingCommand.size() >= 16){
            qDebug() << incomingCommand.toHex();
            for(int i = 0; i < incomingCommand.size(); i++){
                if((quint8) incomingCommand.at(0) != (quint8) 0xAA){
                    incomingCommand.remove(0, 1);
                }
                else{
                    break;
                }
            }
            if(incomingCommand.size() >= 16){
                for(int i = 0; i < 16; i++){
                    if((quint8) outgoingCommand.at(i) == (quint8) incomingCommand.at(i)){
                        qDebug() << "Byte " << i << " is correct";
                        emit commandReceived(true);
                    }
                    else{
                        qDebug() << "There is a wrong byte!";
                        emit commandReceived(false);
                        break;
                    }
                }
                incomingCommand.clear();
            }
            incomingCommand.clear();
        }
    incomingCommand.clear();
//    }
//    else{
    socketAbstract->readAll();
//    }
}

QByteArray SocketOdin::getIncomingCommand(){
    return incomingCommand;
}

QByteArray SocketOdin::getOutgoingCommand(){
    return outgoingCommand;
}

void SocketOdin::sendCommand(){
//    qDebug() << "Sending Byte " << commandCount << "of " << outgoingCommand.size() << " total byte";
//    QByteArray sending;
//    sending.clear();
//    qDebug("%x", (quint8) outgoingCommand.at(commandCount));
//    sending.append(outgoingCommand.at(commandCount));
////    while(!udpSocket->isWritable());
//    udpSocket->writeDatagram(sending, 1, QHostAddress::Broadcast, 8888);
//    commandCount++;
//    if(commandCount >= outgoingCommand.size()){
//        commandTimer.stop();
//        commandCount = 0;
//        qDebug() << "Finished sending command";
//        QTimer::singleShot(readDelay, [=] {
//                timeToRead = true;
//        });
//    }
}

void SocketOdin::setReadDelay(int delay){
    readDelay = delay;
}

}

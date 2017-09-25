#include "socketodin.h"

namespace Odin {

SocketOdin::SocketOdin(){
    udpSocket = new QUdpSocket(this);

//    udpSocket->bind(QHostAddress::Broadcast, 45454);

    qDebug() << "Binded UDP Socket";

    connect(&commandTimer, SIGNAL(timeout()), this, SLOT(sendCommand()));
    connect(udpSocket, SIGNAL(disconnected()), this, SLOT(on_socketDisconnected()));
    connect(udpSocket, SIGNAL(readyRead()), this, SLOT(readCommand()));

    player = new QMediaPlayer;
    player->setMedia(QUrl::fromLocalFile(QDir::currentPath() + "/coins.mp3"));
    player->setVolume(50);
}

void SocketOdin::writeCommand(QByteArray command){
//    qDebug() << "Started command timer";
//    outgoingCommand = command;
////    udpSocket->readAll();
////    udpSocket->flush();
//    timeToRead = false;
//    incomingCommand.clear();
//    if(command.size() > 1){
//        player->play();
//    }
//    commandTimer.start(15);
    udpSocket->writeDatagram(command, command.size(), QHostAddress::Broadcast, 45454);
}

void SocketOdin::on_socketDisconnected(){
    emit odinDisconnected();
}

void SocketOdin::readCommand(){
    if(timeToRead){
//        incomingCommand.append(socketAbstract->readAll());
        if(incomingCommand.size() >= 16){
//            qDebug() << incomingCommand.toHex();
            for(int i = 0; i < incomingCommand.size(); i++){
                if((quint8) incomingCommand.at(0) != (quint8) 0xAA){
                    incomingCommand.remove(0, 1);
                }
                else{
                    break;
                }
            }
            for(int i = 0; i < 16; i++){
                if(outgoingCommand.at(i) == incomingCommand.at(i)){
//                    qDebug() << "Byte " << i << " is correct";
                    emit commandReceived(true);
                }
                else{
//                    qDebug() << "There is a wrong byte!";
                    emit commandReceived(false);
                    break;
                }
            }
            incomingCommand.clear();
        }
    }
    else{
//        socketAbstract->readAll();
    }
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

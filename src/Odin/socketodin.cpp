#include "socketodin.h"

SocketOdin::SocketOdin(){
    connect(&commandTimer, SIGNAL(timeout()), this, SLOT(sendCommand()));
    connect(socketAbstract, SIGNAL(disconnected()), this, SLOT(on_socketDisconnected()));
    connect(socketAbstract, SIGNAL(readyRead()), this, SLOT(readCommand()));
}

void SocketOdin::writeCommand(QByteArray command){
//    qDebug() << "Started command timer";
    outgoingCommand = command;
    socketAbstract->readAll();
    socketAbstract->flush();
    timeToRead = false;
    incomingCommand.clear();
    commandTimer.start(15);
}

void SocketOdin::on_socketDisconnected(){
    emit odinDisconnected();
}

void SocketOdin::readCommand(){
    if(timeToRead){
        incomingCommand.append(socketAbstract->readAll());
        if(incomingCommand.size() >= 16){
//            qDebug() << incomingCommand.toHex();
            for(int i = 0; i < incomingCommand.size(); i++){
                if((quint8) incomingCommand.at(i) != (quint8) 0xAA){
                    incomingCommand.remove(0, 1);
                }
                else{
                    break;
                }
            }
            for(int i = 0; i < 16; i++){
                if(outgoingCommand.at(i) == incomingCommand.at(i)){
                    emit commandReceived(true);
                }
                else{
                    emit commandReceived(false);
                    break;
                }
            }
        }
    }
    else{
        socketAbstract->readAll();
    }
}

QByteArray SocketOdin::getIncomingCommand(){
    return incomingCommand;
}

void SocketOdin::sendCommand(){
//    qDebug() << "Sending Byte " << commandCount << "of " << outgoingCommand.size() << " total byte";
    QByteArray sending;
    sending.clear();
//    qDebug("%x", (quint8) outgoingCommand.at(commandCount));
    sending.append(outgoingCommand.at(commandCount));
    while(!socketAbstract->isWritable());
    socketAbstract->write(sending);
    commandCount++;
    if(commandCount >= outgoingCommand.size()){
        commandTimer.stop();
        commandCount = 0;
//        qDebug() << "Finished sending command";
        QTimer::singleShot(readDelay, [=] {
                timeToRead = true;
        });
    }
}

void SocketOdin::setReadDelay(int delay){
    readDelay = delay;
}

#include "socketodin.h"

SocketOdin::SocketOdin(){
    connect(&commandTimer, SIGNAL(timeout()), this, SLOT(sendCommand()));
    connect(socketAbstract, SIGNAL(disconnected()), this, SLOT(on_socketDisconnected()));
}

void SocketOdin::writeCommand(QByteArray command){
    qDebug() << "Started command timer";
    outgoingCommand = command;
    socketAbstract->flush();
    commandTimer.start(15);
}

void SocketOdin::on_socketDisconnected(){
    emit odinDisconnected();
}

void SocketOdin::sendCommand(){
    qDebug() << "Sending Byte " << commandCount << "of " << outgoingCommand.size() << " total byte";
    QByteArray sending;
    sending.clear();
    qDebug("%x", (quint8) outgoingCommand.at(commandCount));
    sending.append(outgoingCommand.at(commandCount));
    while(!socketAbstract->isWritable());
    socketAbstract->write(sending);
    commandCount++;
    if(commandCount >= outgoingCommand.size()){
        commandTimer.stop();
        commandCount = 0;
        qDebug() << "Finished sending command";
    }
}

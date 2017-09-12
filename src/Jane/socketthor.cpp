#include "socketthor.h"

SocketThor::SocketThor(QObject *parent, Command *ThorCommand_, DataProcessor *ThorData_, Channel *ThorChannel_){
    ThorCommand = ThorCommand_;
    ThorData = ThorData_;
    ThorChannel = ThorChannel_;

    connect(socketAbstract, SIGNAL(readyRead()), this, SLOT(ReadCommand()));
}

void SocketThor::ReadCommand(){
    if(socketAbstract->bytesAvailable() > 5*maxSize/10 && wifiEnabled){
        qDebug() << socketAbstract->read(maxSize);
        if(ThorData->isPlotEnabled()){
            //qDebug() << "Got 8 bit data";
            ThorData->MultiplexChannelData(ThorData->ParseFrameMarkers8bits(socketAbstract->read(maxSize)));
        }
    }
//    QByteArray temp = socketAbstract->readAll();
//    for(int i = 0; i < temp.size(); i++){
//        qDebug() << (quint8) temp.at(i);
//    }
}

bool SocketThor::writeCommand(QByteArray Command){
    lastSentCommand = Command;
    if(socketAbstract->state() == QAbstractSocket::ConnectedState){
//        if(Command.size()>5){
//            ThorData->setBitMode(true);
//            ThorData->setPlotEnabled(true);
//            ThorData->clearallChannelData();
//            ThorChannel->setNumChannels(getNumChannels(Command));
//        }
        socketAbstract->write(Command);         //write the command itself
        return socketAbstract->waitForBytesWritten();
    }
    else
        return false;
    return true;
}

int SocketThor::getNumChannels(QByteArray lastCommand){
    numChannels = 0;
    for(int i=7;i<17;i++){
        if (lastCommand.at(i) == (const char) CHANNEL_ON){
            numChannels++;
        }
    }
    return numChannels;
}

QByteArray SocketThor::getlastCommand(){
    return lastSentCommand;
}

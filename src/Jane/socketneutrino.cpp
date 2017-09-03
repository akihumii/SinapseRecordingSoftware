#include "socketneutrino.h"

SocketNeutrino::SocketNeutrino(Command *NeutrinoCommand_, DataProcessor *NeutrinoData_, Channel *NeutrinoChannel_){
    NeutrinoCommand = NeutrinoCommand_;
    NeutrinoData = NeutrinoData_;
    NeutrinoChannel = NeutrinoChannel_;

    connect(socketAbstract, SIGNAL(readyRead()), this, SLOT(ReadCommand()));
}

void SocketNeutrino::ReadCommand(){
    if(socketAbstract->bytesAvailable() > 5*maxSize/10 && wifiEnabled){
        if(NeutrinoData->isPlotEnabled()){
            if(Mode_8Bit == true){
                //qDebug() << "Got 8 bit data";
                NeutrinoData->MultiplexChannelData(NeutrinoData->ParseFrameMarkers8bits(socketAbstract->read(maxSize)));
            }
            else if(Mode_8Bit == false){
                //qDebug() << "Got 10 bit data";
                NeutrinoData->MultiplexChannelData(NeutrinoData->ParseFrameMarkers10bits(socketAbstract->read(maxSize)));
            }
        }
    }
//    QByteArray temp = socketAbstract->readAll();
//    for(int i = 0; i < temp.size(); i++){
//        qDebug() << (quint8) temp.at(i);
//    }
}

bool SocketNeutrino::writeCommand(QByteArray Command){
    lastSentCommand = Command;
    if(socketAbstract->state() == QAbstractSocket::ConnectedState){
        if(Command.size()>5){
            if(Command.at(6) == (char) WL_8){
                Mode_8Bit = true;
                NeutrinoData->setBitMode(true);
                NeutrinoData->setPlotEnabled(true);
                NeutrinoData->clearallChannelData();
                qDebug() << "8 Bit Mode";
                NeutrinoChannel->setNumChannels(getNumChannels(Command));
            }
            else if(Command.at(6) == (char) WL_10){
                Mode_8Bit = false;
                NeutrinoData->setBitMode(false);
                NeutrinoData->setPlotEnabled(true);
                NeutrinoData->clearallChannelData();
                qDebug() << "10 Bit Mode";
                NeutrinoChannel->setNumChannels(getNumChannels(Command));
            }
            else {
                NeutrinoData->setPlotEnabled(false);
            }
        }
        socketAbstract->write(Command);         //write the command itself
        return socketAbstract->waitForBytesWritten();
    }
    else
        return false;
}

int SocketNeutrino::getNumChannels(QByteArray lastCommand){
    numChannels = 0;
    for(int i=7;i<17;i++){
        if (lastCommand.at(i) == (const char) CHANNEL_ON){
            numChannels++;
        }
    }
    return numChannels;
}

QByteArray SocketNeutrino::getlastCommand(){
    return lastSentCommand;
}

#include "socketedison.h"

SocketEdison::SocketEdison(QObject *parent, Command *NeutrinoCommand_, DataProcessor *NeutrinoData_, Channel *NeutrinoChannel_) : QObject(parent){
    socketCommand = new QTcpSocket(this);
    socketZentri = new QTcpSocket(this);
    NeutrinoCommand = NeutrinoCommand_;
    NeutrinoData = NeutrinoData_;
    NeutrinoChannel = NeutrinoChannel_;

    connect(socketCommand, SIGNAL(connected()), this, SLOT(connectedCommandSocket()));
    connect(socketCommand, SIGNAL(disconnected()), this, SLOT(disconnectedCommandSocket()));
    connect(socketCommand, SIGNAL(readyRead()), this, SLOT(ReadCommand()));

    connect(socketZentri, SIGNAL(connected()), this, SLOT(connectedZentriSocket()));
    connect(socketZentri, SIGNAL(disconnected()), this, SLOT(disconnectedZentriSocket()));
    connect(socketZentri, SIGNAL(readyRead()), this, SLOT(ReadZentri()));

}

void SocketEdison::doConnect(QString ipAddress, int port){
    qDebug() << "Connecting to Command...";
    socketCommand->connectToHost(ipAddress, port);
    qDebug() << "Waiting...";
    if(!socketCommand->waitForConnected(1000)){
        qDebug() << "Error: " << socketCommand->errorString();
    }
    qDebug() << "Connecting to Zentri...";
    socketZentri->connectToHost("192.168.0.100", 3000);
    qDebug() << "Waiting...";
    if(!socketZentri->waitForConnected(1000)){
        qDebug() << "Error: " << socketZentri->errorString();
    }
}

void SocketEdison::connectedCommandSocket(){
    qDebug() << "Command Socket Connected!";
}

void SocketEdison::disconnectedCommandSocket(){
    qDebug() << "Command Socket Disconnected";
}

void SocketEdison::connectedZentriSocket(){
    qDebug() << "Zentri Socket Connected!";
}

void SocketEdison::disconnectedZentriSocket(){
    qDebug() << "Zentri Socket Disconnected";
}

void SocketEdison::ReadCommand(){
    if(socketCommand->bytesAvailable() > maxSize-1 && wifiEnabled){
        if(NeutrinoData->isPlotEnabled()){
            if(Mode_8Bit == true){
                NeutrinoData->MultiplexChannelData(NeutrinoData->ParseFrameMarkers8bits(socketCommand->read(maxSize)));
            }
            else if(Mode_8Bit == false){
                NeutrinoData->MultiplexChannelData(NeutrinoData->ParseFrameMarkers10bits(socketCommand->read(maxSize)));
            }
        }
    }
}

void SocketEdison::ReadZentri(){
//    NeutrinoData->MultiplexChannelData(NeutrinoData->ParseFrameMarkers10bits(socketZentri->read(maxSize)));
    if(socketZentri->bytesAvailable() > maxSize-1 && wifiEnabled){
        if(NeutrinoData->isPlotEnabled()){
            if(Mode_8Bit == true){
                NeutrinoData->MultiplexChannelData(NeutrinoData->ParseFrameMarkers8bits(socketZentri->read(maxSize)));
            }
            else if(Mode_8Bit == false){
                NeutrinoData->MultiplexChannelData(NeutrinoData->ParseFrameMarkers10bits(socketZentri->read(maxSize)));
            }
        }
    }
}

void SocketEdison::doDisconnect(){
    if(socketCommand->state() == QAbstractSocket::ConnectedState){
        socketCommand->flush();
        socketCommand->disconnectFromHost();
        socketCommand->close();
        qDebug()<<"Command Socket disconnected";
    }
    if(socketZentri->state() == QAbstractSocket::ConnectedState){
        socketZentri->flush();
        socketZentri->disconnectFromHost();
        socketZentri->close();
        qDebug()<<"Zentri Socket disconnected";
    }
}

bool SocketEdison::isConnected(){
    if (socketCommand->state() == QAbstractSocket::ConnectedState){
        return true;
    }
    else return false;
}

bool SocketEdison::writeCommand(QByteArray Command){
    lastSentCommand = Command;
    if(socketCommand->state() == QAbstractSocket::ConnectedState){
        if(Command.size()>5){
            if(Command.at(6) == (char) BitMode_8){
                Mode_8Bit = true;
                NeutrinoData->setBitMode(true);
                NeutrinoData->setPlotEnabled(true);
                NeutrinoData->clearallChannelData();
                qDebug() << "8 Bit Mode";
                NeutrinoChannel->setNumChannels(getNumChannels(Command));
            }
            else if(Command.at(6) == (char) BitMode_10){
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
        socketCommand->write(Command);
        return socketCommand->waitForBytesWritten();
    }
    else
        return false;
}

QString SocketEdison::getError(){
    return socketCommand->errorString();
}

int SocketEdison::getNumChannels(QByteArray lastCommand){
    numChannels = 0;
    for(int i=7;i<17;i++){
        if (lastCommand.at(i) == (const char) CHANNEL_ON){
            numChannels++;
        }
    }
    return numChannels;
}

QByteArray SocketEdison::getlastCommand(){
    return lastSentCommand;
}

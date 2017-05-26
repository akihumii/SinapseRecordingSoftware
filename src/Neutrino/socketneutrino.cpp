#include "socketneutrino.h"

SocketNeutrino::SocketNeutrino(QObject *parent, Command *NeutrinoCommand_, DataProcessor *NeutrinoData_, Channel *NeutrinoChannel_) : QObject(parent){
    socketNeutrino = new QTcpSocket(this);
    NeutrinoCommand = NeutrinoCommand_;
    NeutrinoData = NeutrinoData_;
    NeutrinoChannel = NeutrinoChannel_;

    connect(socketNeutrino, SIGNAL(connected()), this, SLOT(connectedCommandSocket()));
    connect(socketNeutrino, SIGNAL(disconnected()), this, SLOT(disconnectedCommandSocket()));
    connect(socketNeutrino, SIGNAL(readyRead()), this, SLOT(ReadCommand()));
}

void SocketNeutrino::doConnect(QString ipAddress, int port){
    int connectionTries = 0;
    while(socketNeutrino->state() != QAbstractSocket::ConnectedState){
        qDebug() << "Connecting...";
        socketNeutrino->connectToHost(ipAddress, port);
        qDebug() << "Waiting...";
        if(!socketNeutrino->waitForConnected(1000)){
            qDebug() << "Error: " << socketNeutrino->errorString();
        }
        connectionTries++;
        if(connectionTries>2)
            return;
    }
}

void SocketNeutrino::connectedCommandSocket(){
    qDebug() << "Command Socket Connected!";
}

void SocketNeutrino::disconnectedCommandSocket(){
    qDebug() << "Command Socket Disconnected";
}

void SocketNeutrino::ReadCommand(){
    if(socketNeutrino->bytesAvailable() > 5*maxSize/10 && wifiEnabled){
        if(NeutrinoData->isPlotEnabled()){
            if(Mode_8Bit == true){
//                qDebug() << "Got 8 bit data";
                NeutrinoData->MultiplexChannelData(NeutrinoData->ParseFrameMarkers8bits(socketNeutrino->read(maxSize)));
            }
            else if(Mode_8Bit == false){
//                qDebug() << "Got 10 bit data";
                NeutrinoData->MultiplexChannelData(NeutrinoData->ParseFrameMarkers10bits(socketNeutrino->read(maxSize)));
            }
        }
    }
}

void SocketNeutrino::doDisconnect(){
    if(socketNeutrino->state() == QAbstractSocket::ConnectedState){
        socketNeutrino->flush();
        socketNeutrino->disconnectFromHost();
        socketNeutrino->close();
        qDebug()<<"Disconnect clicked";
    }
}

bool SocketNeutrino::isConnected(){
    return (socketNeutrino->state() == QAbstractSocket::ConnectedState);
}

bool SocketNeutrino::writeCommand(QByteArray Command){
    lastSentCommand = Command;
    if(socketNeutrino->state() == QAbstractSocket::ConnectedState){
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
        socketNeutrino->write(Command);         //write the command itself
        return socketNeutrino->waitForBytesWritten();
    }
    else
        return false;
}

QString SocketNeutrino::getError(){
    return socketNeutrino->errorString();
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

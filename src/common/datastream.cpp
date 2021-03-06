#include "datastream.h"

DataStream::DataStream(QObject *parent) : QObject(parent = Q_NULLPTR){
    connectionMapper = new QSignalMapper(this);
    connect(connectionMapper, SIGNAL(mapped(int)), this, SLOT(on_newConnection(int)));
    for(int i = 0; i < 10; i++){
        serverMatlab[i] = new QTcpServer(this);
        connectionMapper->setMapping(serverMatlab[i], i);
        connect(serverMatlab[i], SIGNAL(newConnection()), connectionMapper, SLOT(map()));
        if(serverMatlab[i]->listen(QHostAddress::LocalHost, serverPort[i])){
            qDebug() << "Server " << i+1 << "is started";
        }
        else{
            qDebug() << "Server " << i+1 << "could not be started";
        }
    }
    socketMapper = new QSignalMapper(this);
    connect(socketMapper, SIGNAL(mapped(int)), this, SLOT(disconnect(int)));
}

void DataStream::on_newConnection(int connected){
    qDebug() << "Port " << serverPort[connected] << " is connected!";
    socketMatlab[connected] = new QTcpSocket(this);
    socketMatlab[connected] = serverMatlab[connected]->nextPendingConnection();
    socketMapper->setMapping(socketMatlab[connected], connected);
    connect(socketMatlab[connected], SIGNAL(readyRead()), socketMapper, SLOT(map()));
    streamConnected[connected] = true;
    clearChannelData(connected);
    out[connected] = new QDataStream(socketMatlab[connected]);
}

void DataStream::disconnect(int i){
    streamConnected[i] = false;
    socketMatlab[i]->disconnectFromHost();
    socketMatlab[i]->deleteLater();
}

bool DataStream::getStreamConnected(int channel){
    return streamConnected[channel];
}

void DataStream::disableStream(int channel){
    streamConnected[channel] = false;
}

void DataStream::streamData(int channel){
    if(socketMatlab[channel]->isOpen()){
        for(int i = 0; i < ChannelData[channel].size(); i++){
//            *out[channel] << QString::number(ChannelData[channel].at(i));
            *out[channel] << ChannelData[channel].at(i);
        }
    }
    else{
        socketMatlab[channel]->disconnectFromHost();
        streamConnected[channel] = false;
    }
    clearChannelData(channel);
}

void DataStream::appendData(int channel, double data){
    ChannelData[channel].append(data);
}

void DataStream::appendData(int channel, QVector<double> data){
    ChannelData[channel].append(data);
}

void DataStream::clearChannelData(int ChannelIndex){
    ChannelData[ChannelIndex].remove(0,(ChannelData[ChannelIndex].size()));
    ChannelData[ChannelIndex].resize(0);
}

int DataStream::getChannelSize(int channel){
    return ChannelData[channel].size();
}

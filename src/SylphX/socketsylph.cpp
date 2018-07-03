#include "socketsylph.h"

namespace SylphX {

SocketSylph::SocketSylph(DataProcessor *dataProcessor_){
    dataProcessor = dataProcessor_;
    timer = new QTimer;
    connect(socketAbstract, SIGNAL(readyRead()), this, SLOT(ReadCommand()));
    connect(timer, SIGNAL(timeout()), this, SLOT(updateRate()));
    timer->start(1000);
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
}

void SocketSylph::discardData(){
    for(int i = 0; i < 10; i++){
        socketAbstract->read(maxSize*10);
        qDebug() << "Discarding";
    }
    checked = false;
}
void SocketSylph::ReadCommand(){
    if(initCount < 20){
        discardData();
        initCount++;
    }
    else if(socketAbstract->bytesAvailable() >= maxSize && checked){
        if(dataProcessor->isSmart()){
            bytesRead += dataProcessor->parseFrameMarkersWithChecks(socketAbstract->read(maxSize));
        }
        else{
            bytesRead += dataProcessor->parseFrameMarkers(socketAbstract->read(maxSize));
        }
    }
    else if(socketAbstract->bytesAvailable() >= packetSize+1 && !checked){
        qDebug() << "checking";
        if(dataProcessor->checkNextFrameMarker(socketAbstract->read(packetSize+1), 0)){
            checked = true;
            qDebug() << "checked is true";
        }
    }
}

void SocketSylph::updateRate(){
    rate = bytesRead*8/1000;
    if(rate == 0 && checked){
        initCount = 0;
        checked = false;
    }
    bytesRead = 0;
}

int SocketSylph::getRate(){
    return rate;
}

void SocketSylph::appendSync(){
    qDebug() << "Sync pulse detected!";
}

void SocketSylph::closeESP(){
    qDebug() << "Closing ESP";
    QByteArray closingMSG = "DISCONNECT!!!!!!";
    socketAbstract->write(closingMSG);
    socketAbstract->waitForBytesWritten(1000);
}

void SocketSylph::on_newConnection(int connected){
    qDebug() << "Port " << serverPort[connected] << " is connected!";
    socketMatlab[connected] = new QTcpSocket(this);
    socketMatlab[connected] = serverMatlab[connected]->nextPendingConnection();
    connect(socketMatlab[connected], SIGNAL(disconnected()), socketMatlab[connected], SLOT(deleteLater()));
    streamConnected[connected] = true;
}

bool SocketSylph::getStreamConnected(int channel){
    return streamConnected[channel];
}

void SocketSylph::disableStream(int channel){
    streamConnected[channel] = false;
}

void SocketSylph::streamData(int channel, QVector<double> rawData){
    QByteArray data;
//    QDataStream stream(&data, QIODevice::WriteOnly);
//    for (auto x : rawData){
//        stream << x;
//        qDebug() << x;
//    }
    for (int i = 0; i < rawData.size(); i++) {
        data = data.append(QString::number(rawData[i])+"\n");
    }
    if(socketMatlab[channel]->isOpen()){
        socketMatlab[channel]->write(data);
    }
    else{
        socketMatlab[channel]->disconnectFromHost();
        streamConnected[channel] = false;
    }
}

}

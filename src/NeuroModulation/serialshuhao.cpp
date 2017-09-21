#include "serialshuhao.h"

SerialShuHao::SerialShuHao(){
    shuHaoPort = new QSerialPort(this);
}

void SerialShuHao::connectShuHao(){
    portInfo = QSerialPortInfo::availablePorts();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        qDebug() << "Name : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();
    }
    for(int i = 0; i < portInfo.size(); i++){
        if(portInfo.at(i).manufacturer().contains("Silicon", Qt::CaseInsensitive)){
            shuHaoPort->setPortName(portInfo.at(i).portName());
            qDebug() << "Connected to port " << portInfo.at(i).portName();
            connectedPortName = portInfo.at(i).portName();
            shuHaoPort->setBaudRate(2400);
            shuHaoPort->setDataBits(QSerialPort::Data8);
            shuHaoPort->setParity(QSerialPort::NoParity);
            shuHaoPort->setStopBits(QSerialPort::OneStop);
            shuHaoPort->setFlowControl(QSerialPort::NoFlowControl);
        }
    }
    if(shuHaoPort->open(QIODevice::ReadWrite)){
        shuHaoSerialConnected = true;
        qDebug() << "Connected to NeuroModulation Serial Port!";
    }
}

QString SerialShuHao::getConnectedPort(){
    return connectedPortName;
}

void SerialShuHao::checkConnectivity(){
    if(shuHaoSerialConnected){
        portInfo = QSerialPortInfo::availablePorts();
        foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
            if(connectedPortName == info.portName())
                return;
        }
        emit shuHaoDisconnected();
    }
}

void SerialShuHao::closeShuHaoSerial(){
    shuHaoPort->flush();
    shuHaoPort->close();
}

bool SerialShuHao::isShuHaoSerialConnected(){
    return shuHaoSerialConnected;
}

void SerialShuHao::writeCommand(QByteArray command){
    shuHaoPort->write(command);
}

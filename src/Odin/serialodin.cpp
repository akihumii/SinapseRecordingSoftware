#include "serialodin.h"

SerialOdin::SerialOdin(QObject *parent) : QObject(parent = Q_NULLPTR){
    odinPort = new QSerialPort(this);

    serialTimer = new QTimer;
    serialTimer->start(100);
    connect(serialTimer, SIGNAL(timeout()), this, SLOT(checkConnectivity()));
    connect(&commandTimer, SIGNAL(timeout()), this, SLOT(sendCommand()));
    connect(odinPort, SIGNAL(readyRead()), this, SLOT(readCommand()));
}

void SerialOdin::connectOdin(){
    portInfo = QSerialPortInfo::availablePorts();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        qDebug() << "Name : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();
    }
    for(int i = 0; i < portInfo.size(); i++){
        if(portInfo.at(i).manufacturer().contains("Silicon", Qt::CaseInsensitive)){
            odinPort->setPortName(portInfo.at(i).portName());
            qDebug() << "Connected to port " << portInfo.at(i).portName();
            connectedPortName = portInfo.at(i).portName();
            odinPort->setBaudRate(2400);
            odinPort->setDataBits(QSerialPort::Data8);
            odinPort->setParity(QSerialPort::NoParity);
            odinPort->setStopBits(QSerialPort::OneStop);
            odinPort->setFlowControl(QSerialPort::NoFlowControl);
        }
    }
    if(odinPort->open(QIODevice::ReadWrite)){
        odinSerialConnected = true;
        qDebug() << "Connected to Odin serial";
    }
}

QString SerialOdin::getConnectedPort(){
    return connectedPortName;
}

void SerialOdin::readCommand(){
    QByteArray temp = odinPort->read(1);
    qDebug("Read: %x", (quint8) temp.at(0));
    emit commandReceived(true);
}

void SerialOdin::checkConnectivity(){
    if(odinSerialConnected){
        portInfo = QSerialPortInfo::availablePorts();
        foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
            if(connectedPortName == info.portName())
                return;
        }
        emit odinDisconnected();
    }
}

void SerialOdin::closeOdinSerial(){
    odinPort->flush();
    odinPort->close();
}

bool SerialOdin::isOdinSerialConnected(){
    return odinSerialConnected;
}

void SerialOdin::initOdin(){
    qDebug() << "Odin Initialised!";
}

void SerialOdin::writeCommand(QByteArray command){
    qDebug() << "Started command timer";
    outgoingCommand = command;
    commandTimer.start(15);
}

void SerialOdin::sendCommand(){
    qDebug() << "Sending Byte " << commandCount;
    QByteArray sending;
    sending.clear();
    qDebug("%x", (quint8) outgoingCommand.at(commandCount));
    sending.append(outgoingCommand.at(commandCount));
    odinPort->write(sending);
    commandCount++;
    if(commandCount >= 16){
        commandTimer.stop();
        commandCount = 0;
        qDebug() << "Finished sending command";
    }
}

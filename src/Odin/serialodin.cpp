#include "serialodin.h"

SerialOdin::SerialOdin(QObject *parent) : QObject(parent = Q_NULLPTR){
    odinPort = new QSerialPort(this);
    connect(&commandTimer, SIGNAL(timeout()), this, SLOT(sendCommand()));
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

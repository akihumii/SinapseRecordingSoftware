#include "serialodin.h"

SerialOdin::SerialOdin(QObject *parent) : QObject(parent = Q_NULLPTR){
    odinPort = new QSerialPort(this);
}

void SerialOdin::connectOdin(){
    portInfo = QSerialPortInfo::availablePorts();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        qDebug() << "Name : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();
    }
    for(int i = 0; i < portInfo.size(); i++){
        if(portInfo.at(i).manufacturer() == "Silicon Labs"){
            odinPort->setPortName(portInfo.at(i).portName());
            odinPort->setBaudRate(2400);
            odinPort->setDataBits(QSerialPort::Data8);
            odinPort->setParity(QSerialPort::NoParity);
            odinPort->setStopBits(QSerialPort::OneStop);
            odinPort->setFlowControl(QSerialPort::NoFlowControl);
        }
    }
    if(odinPort->open(QIODevice::ReadWrite)){
        qDebug() << "Connected to Odin serial";
    }
}

void SerialOdin::initOdin(){
    odinPort->write(QByteArray::fromHex("F8"));
    QMessageBox *mbox = new QMessageBox;
    QTimer::singleShot(3000, mbox, SLOT(hide()));
    mbox->setText("Initialising Odin.. Please wait...");
    mbox->show();
    qDebug() << "Odin Initialised!";
}

void SerialOdin::writeCommand(QByteArray command){
    for(int i = 0; i < 16; i++){
        qDebug() << "Sending Byte " << i;
        QByteArray sending;
        sending.clear();
        qDebug("%x", (quint8) command.at(i));
        sending.append(command.at(i));
        odinPort->write(sending);
        QThread::msleep(15);
    }
}

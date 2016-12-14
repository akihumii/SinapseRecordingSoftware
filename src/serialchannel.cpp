#include "serialchannel.h"

SerialChannel::SerialChannel(QObject *parent, DataProcessor_MA *dataProcessor_) : QObject(parent = Q_NULLPTR)
{
    implantPort = new QSerialPort(this);
    ADCPort = new QSerialPort(this);

    dataProcessor = dataProcessor_;

    connect(implantPort, SIGNAL(readyRead()), this, SLOT(ReadImplantData()));
    connect(ADCPort, SIGNAL(readyRead()), this, SLOT(ReadADCData()));
}

void SerialChannel::ReadImplantData(){
    dataProcessor->parseFrameMarkers(implantPort->read(2000));
}

void SerialChannel::closeImplantPort(){
    implantPort->close();
}

void SerialChannel::closeADCPort(){
    ADCPort->close();
}

void SerialChannel::ReadADCData(){
    dataProcessor->sortADCData(ADCPort->read(2000));
}

bool SerialChannel::enableImplantPort(QString portName){
    implantPort->setPortName(portName);
    implantPort->setBaudRate(1333333);
    implantPort->setDataBits(QSerialPort::Data8);
    implantPort->setParity(QSerialPort::NoParity);
    implantPort->setStopBits(QSerialPort::OneStop);
    implantPort->setFlowControl(QSerialPort::NoFlowControl);
    implantPort->setReadBufferSize(2000);

    if (implantPort->open(QIODevice::ReadOnly)) {
        return 1;
    }
    else{
        return 0;
    }
}

bool SerialChannel::enableADCPort(QString portName){
    ADCPort->setPortName(portName);
    ADCPort->setBaudRate(1333333);
    ADCPort->setDataBits(QSerialPort::Data8);
    ADCPort->setParity(QSerialPort::NoParity);
    ADCPort->setStopBits(QSerialPort::OneStop);
    ADCPort->setFlowControl(QSerialPort::NoFlowControl);
    ADCPort->setReadBufferSize(2000);

    if (ADCPort->open(QIODevice::ReadWrite)) {
        return 1;
    }
    else{
        return 0;
    }
}

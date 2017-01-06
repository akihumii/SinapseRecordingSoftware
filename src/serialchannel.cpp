#include "serialchannel.h"

SerialChannel::SerialChannel(QObject *parent, DataProcessor *dataProcessor_) : QObject(parent = Q_NULLPTR)
{
    implantPort = new QSerialPort(this);
    ADCPort = new QSerialPort(this);

    dataProcessor = dataProcessor_;

    connect(implantPort, SIGNAL(readyRead()), this, SLOT(ReadImplantData()));
    connect(ADCPort, SIGNAL(readyRead()), this, SLOT(ReadADCData()));
}

SerialChannel::SerialChannel(QObject *parent, Command *NeutrinoCommand_, DataProcessor *NeutrinoData_, Channel *NeutrinoChannel_) : QObject(parent)
{
    serial = new QSerialPort(this);
    NeutrinoCommand = NeutrinoCommand_;
    NeutrinoData = NeutrinoData_;
    NeutrinoChannel = NeutrinoChannel_;

    connect(serial, SIGNAL(readyRead()), this, SLOT(ReadData()));
}

void SerialChannel::ReadImplantData(){
#ifdef SYLPH
    dataProcessor->parseFrameMarkers(implantPort->read(500));
    dataProcessor->sortADCData(ADCPort->read(500));
#endif
}

void SerialChannel::closeImplantPort(){
    implantPort->close();
}

void SerialChannel::closeADCPort(){
    ADCPort->close();
}

void SerialChannel::ReadADCData(){

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

    if (ADCPort->open(QIODevice::ReadOnly)) {

        return 1;
    }
    else{
        return 0;
    }
}

void SerialChannel::ReadData(){
    qDebug() << NeutrinoChannel->getNumChannels();
    if(serialenabled){
        QByteArray buffer;
        buffer = serial->read(2048);
        for(int i=0;i<buffer.size();i++){
                qDebug() << QString::number((uint8_t) buffer.at(i),2);
        }
        buffer.clear();
    }
}

void SerialChannel::closePort(){
    serial->close();
}

bool SerialChannel::doConnect(){
    serial->setPortName("ttyUSB0");
    serial->setBaudRate(3000000);
    serial->setDataBits(QSerialPort::Data5);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    serial->setReadBufferSize(128);

    if (serial->open(QIODevice::ReadWrite)) {
        qDebug() << "Connected via USB!";
        return 1;
    }
    else{
        return 0;
    }
}

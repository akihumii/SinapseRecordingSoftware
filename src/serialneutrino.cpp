#include "serialneutrino.h"

SerialNeutrino::SerialNeutrino(QObject *parent, Command *NeutrinoCommand_, DataProcessor_KA *NeutrinoData_, Channel *NeutrinoChannel_) : QObject(parent)
{
    serial = new QSerialPort(this);
    NeutrinoCommand = NeutrinoCommand_;
    NeutrinoData = NeutrinoData_;
    NeutrinoChannel = NeutrinoChannel_;

    connect(serial, SIGNAL(readyRead()), this, SLOT(ReadData()));
}

void SerialNeutrino::ReadData(){
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

void SerialNeutrino::closePort(){
    serial->close();
}

bool SerialNeutrino::doConnect(){
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

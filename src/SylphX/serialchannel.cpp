#include "serialchannel.h"

namespace SylphX {

SerialChannel::SerialChannel(QObject *parent, DataProcessor *dataProcessor_) : QObject(parent = Q_NULLPTR)
{
    implantPort = new QSerialPort(this);
    ADCPort = new QSerialPort(this);

    dataProcessor = dataProcessor_;

    connect(implantPort, SIGNAL(readyRead()), this, SLOT(ReadImplantData()));
    connect(ADCPort, SIGNAL(readyRead()), this, SLOT(ReadADCData()));

    if(!ForceSensorFlag){
        timer = new QTimer;
        connect(timer, SIGNAL(timeout()), this, SLOT(updateRate()));
        timer->start(1000);
    }
}

void SerialChannel::ReadImplantData(){
    if(!ForceSensorFlag){  // FTDI recording
        if(implantPort->bytesAvailable() >= maxSize && checked){
            bytesRead += dataProcessor->parseFrameMarkersWithChecks(implantPort->read(maxSize));
        }
        else if(implantPort->bytesAvailable() >= packetSize+1 && !checked){
            qDebug() << "checking";
            if(dataProcessor->checkNextFrameMarker(implantPort->read(packetSize+1), 0)){
                checked = true;
                qDebug() << "checked is true";
            }
            else{
                ReadImplantData();
            }
        }
    }
    else{  // force sensor
        if(implantPort->bytesAvailable() >= maxSizeSerial && checked){
            dataProcessor->parseFrameMarkers(implantPort->read(maxSizeSerial));
            emit receiveForceSignal();
        }
        else if(implantPort->bytesAvailable() >= packetSize+1 && !checked){
            qDebug() << "checking";
            if(dataProcessor->checkNextFrameMarker(implantPort->read(packetSize+1), 0)){
                checked = true;
                qDebug() << "checked is true";
            }
        }
    }
}

void SerialChannel::closeImplantPort(){
    implantPort->flush();
    implantPort->close();
}

void SerialChannel::closeADCPort(){
    ADCPort->flush();
    ADCPort->close();
}

void SerialChannel::updateRate(){
    rate = bytesRead*8/1000;
    if(rate == 0 && checked){
//        implantPort->flush();
        checked = false;
    }
    bytesRead = 0;
}

int SerialChannel::getRate(){
    return rate;
}

void SerialChannel::setChecked(bool flag){
    checked = flag;
}

void SerialChannel::ReadADCData(){
    qDebug() << "reading adc data";
    dataProcessor->sortADCData(ADCPort->read(25200));
}

bool SerialChannel::enableImplantPort(QString portName){
    implantPort->setPortName(portName);
    implantPort->setBaudRate(500000);
    implantPort->setDataBits(QSerialPort::Data8);
    implantPort->setParity(QSerialPort::NoParity);
    implantPort->setStopBits(QSerialPort::OneStop);
    implantPort->setFlowControl(QSerialPort::NoFlowControl);
    implantPort->setReadBufferSize(maxSizeSerial);

    if (implantPort->open(QIODevice::ReadOnly)) {
        return 1;
    }
    else{
        return 0;
    }
}

bool SerialChannel::enableADCPort(QString portName){
    ADCPort->setPortName(portName);
    ADCPort->setBaudRate(500000);
    ADCPort->setDataBits(QSerialPort::Data8);
    ADCPort->setParity(QSerialPort::NoParity);
    ADCPort->setStopBits(QSerialPort::OneStop);
    ADCPort->setFlowControl(QSerialPort::NoFlowControl);
    ADCPort->setReadBufferSize(maxSizeSerial);

    if (ADCPort->open(QIODevice::ReadOnly)) {
        return 1;
    }
    else{
        return 0;
    }
}

void SerialChannel::connectSylph(){
    if(!ForceSensorFlag){    // FTDI recording
        portInfo = QSerialPortInfo::availablePorts();
        foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
            qDebug() << "Name : " << info.portName();
            qDebug() << "Description : " << info.description();
            qDebug() << "Manufacturer: " << info.manufacturer();
            qDebug() << "Serial Number: " << info.serialNumber();
        }
        for(int i = 0; i < portInfo.size()-1; i++){
            if(portInfo.at(i).manufacturer() == "FTDI" && portInfo.at(i+1).manufacturer() == "FTDI"){
                if(portInfo.at(i+1).portName().at(portInfo.at(i+1).portName().size()-1).digitValue()
                    > portInfo.at(i).portName().at(portInfo.at(i+1).portName().size()-1).digitValue()){
                    implantPort->setPortName(portInfo.at(i+1).portName());
                    implantPort->setBaudRate(500000);

                    ADCPort->setPortName(portInfo.at(i).portName());
                    ADCPort->setBaudRate(500000);
                }
                else{
                    implantPort->setPortName(portInfo.at(i).portName());
                    implantPort->setBaudRate(500000);

                    ADCPort->setPortName(portInfo.at(i+1).portName());
                    ADCPort->setBaudRate(500000);
                }
                implantPort->setDataBits(QSerialPort::Data8);
                implantPort->setParity(QSerialPort::NoParity);
                implantPort->setStopBits(QSerialPort::OneStop);
                implantPort->setFlowControl(QSerialPort::NoFlowControl);
                implantPort->setReadBufferSize(maxSize);

                ADCPort->setDataBits(QSerialPort::Data8);
                ADCPort->setParity(QSerialPort::NoParity);
                ADCPort->setStopBits(QSerialPort::OneStop);
                ADCPort->setFlowControl(QSerialPort::NoFlowControl);
                break;
            }
        }
        if (implantPort->open(QIODevice::ReadWrite)){
            implantConnected = true;
            for(int i = 0; i < 3000; i++){
                implantPort->readAll();
            }
            checked = false;
            qDebug() << "Implant Port connnected!";
        }
        if (ADCPort->open(QIODevice::ReadWrite)){
            ADCConnected = true;
            for(int i = 0; i < 3000; i++){
                ADCPort->readAll();
            }
            qDebug() << "ADC Port connnected!";
        }
    }
    else{    // Force Sensor
        portInfo = QSerialPortInfo::availablePorts();
        foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
            qDebug() << "Name : " << info.portName();
            qDebug() << "Description : " << info.description();
            qDebug() << "Manufacturer: " << info.manufacturer();
        }
        for(int i = 0; i < portInfo.size(); i++){
            if(portInfo.at(i).manufacturer().contains("Silicon", Qt::CaseInsensitive)){
                implantPort->setPortName(portInfo.at(i).portName());
                qDebug() << "Connected to port " << portInfo.at(i).portName();
    //            connectedPortName = portInfo.at(i).portName();
                implantPort->setBaudRate(500000);
                implantPort->setDataBits(QSerialPort::Data8);
                implantPort->setParity(QSerialPort::NoParity);
                implantPort->setStopBits(QSerialPort::OneStop);
                implantPort->setFlowControl(QSerialPort::NoFlowControl);
            }
        }
        if(implantPort->open(QIODevice::ReadWrite)){
            implantConnected = true;
            qDebug() << "Connected to Sylph serial";
        }
    }
}

bool SerialChannel::isImplantConnected(){
    return implantConnected;
}

bool SerialChannel::isADCConnected(){
    return ADCConnected;
}

bool SerialChannel::isConnected(){
    return implantConnected || ADCConnected;
}

void SerialChannel::swapPort(){
    implantPort->close();
    ADCPort->close();
    portInfo = QSerialPortInfo::availablePorts();
    for(int i = 0; i < portInfo.size(); i++){
        if(portOrder == 1){
            ADCPort->setPortName(portInfo.at(i+1).portName());
            ADCPort->setBaudRate(500000);
            ADCPort->setDataBits(QSerialPort::Data8);
            ADCPort->setParity(QSerialPort::NoParity);
            ADCPort->setStopBits(QSerialPort::OneStop);
            ADCPort->setFlowControl(QSerialPort::NoFlowControl);
            ADCPort->setReadBufferSize(maxSizeSerial);

            implantPort->setPortName(portInfo.at(i).portName());
            implantPort->setBaudRate(500000);
            implantPort->setDataBits(QSerialPort::Data8);
            implantPort->setParity(QSerialPort::NoParity);
            implantPort->setStopBits(QSerialPort::OneStop);
            implantPort->setFlowControl(QSerialPort::NoFlowControl);
            implantPort->setReadBufferSize(maxSizeSerial);
            portOrder = 2;
            break;
        }
        else if(portOrder == 2){
            ADCPort->setPortName(portInfo.at(i).portName());
            ADCPort->setBaudRate(500000);
            ADCPort->setDataBits(QSerialPort::Data8);
            ADCPort->setParity(QSerialPort::NoParity);
            ADCPort->setStopBits(QSerialPort::OneStop);
            ADCPort->setFlowControl(QSerialPort::NoFlowControl);
            ADCPort->setReadBufferSize(maxSizeSerial);

            implantPort->setPortName(portInfo.at(i+1).portName());
            implantPort->setBaudRate(500000);
            implantPort->setDataBits(QSerialPort::Data8);
            implantPort->setParity(QSerialPort::NoParity);
            implantPort->setStopBits(QSerialPort::OneStop);
            implantPort->setFlowControl(QSerialPort::NoFlowControl);
            implantPort->setReadBufferSize(maxSizeSerial);
            portOrder = 1;
            break;
        }
    }
    ADCPort->open(QIODevice::ReadWrite);
    implantPort->open(QIODevice::ReadWrite);
}

void SerialChannel::flushADC(){
    for(int i = 0; i < 300000; i++){
        ADCPort->readAll();
    }
}

}

#include "serialchannel.h"

SerialChannel::SerialChannel(QObject *parent, DataProcessor *dataProcessor_) : QObject(parent = Q_NULLPTR)
{
    implantPort = new QSerialPort(this);
    ADCPort = new QSerialPort(this);
    arduinoPort = new QSerialPort(this);

    dataProcessor = dataProcessor_;

    connect(implantPort, SIGNAL(readyRead()), this, SLOT(ReadImplantData()));
    connect(ADCPort, SIGNAL(readyRead()), this, SLOT(ReadADCData()));
    connect(dataProcessor, SIGNAL(groupIsignal()), this, SLOT(sendMovementOne()));
    connect(dataProcessor, SIGNAL(groupJsignal()), this, SLOT(sendMovementTwo()));
}

void SerialChannel::ReadImplantData(){
    if(implantPort->bytesAvailable() >= 210 && checked){
        dataProcessor->parseFrameMarkers(implantPort->read(210));
    }
    else if(implantPort->bytesAvailable() >= 26 && !checked){
        qDebug() << "checking";
        if(dataProcessor->checkNextFrameMarker(implantPort->read(26), 0)){
            checked = true;
            qDebug() << "checked is true";
        }
    }
    dataProcessor->sortADCData(ADCPort->read(500));
}

void SerialChannel::closeImplantPort(){
    implantPort->flush();
    implantPort->close();
}

void SerialChannel::closeADCPort(){
    ADCPort->flush();
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
    implantPort->setReadBufferSize(210);

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
    ADCPort->setReadBufferSize(210);

    if (ADCPort->open(QIODevice::ReadOnly)) {
        return 1;
    }
    else{
        return 0;
    }
}

void SerialChannel::connectSylph(){
    portInfo = QSerialPortInfo::availablePorts();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        qDebug() << "Name : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();
        qDebug() << "Serial Number: " << info.serialNumber();
    }
    arduinoPort->setPortName("COM7");
    arduinoPort->setBaudRate(9600);
    arduinoPort->setDataBits(QSerialPort::Data8);
    arduinoPort->setParity(QSerialPort::NoParity);
    arduinoPort->setStopBits(QSerialPort::OneStop);
    arduinoPort->setFlowControl(QSerialPort::NoFlowControl);
    for(int i = 0; i < portInfo.size(); i++){
//        if(portInfo.at(i).serialNumber() !="A50550HNA"){
            if(portInfo.at(i).manufacturer() == "FTDI" && portInfo.at(i+1).manufacturer() == "FTDI"){
                if(portInfo.at(i+1).portName().at(portInfo.at(i+1).portName().size()-1).digitValue()
                        == portInfo.at(i).portName().at(portInfo.at(i+1).portName().size()-1).digitValue()+1){
                    implantPort->setPortName(portInfo.at(i+1).portName());
                    qDebug() << "Implant port connected to " << portInfo.at(i+1).portName();
                    implantPort->setBaudRate(1333333);

                    ADCPort->setPortName(portInfo.at(i).portName());
                    qDebug() << "ADC port connected to " << portInfo.at(i).portName();
                    ADCPort->setBaudRate(1333333);
                }
                else{
                    implantPort->setPortName(portInfo.at(i).portName());
                    qDebug() << "Implant port connected to " << portInfo.at(i).portName();
                    implantPort->setBaudRate(1333333);

                    ADCPort->setPortName(portInfo.at(i+1).portName());
                    qDebug() << "ADC port connected to " << portInfo.at(i+1).portName();
                    ADCPort->setBaudRate(1333333);
                }
                implantPort->setDataBits(QSerialPort::Data8);
                implantPort->setParity(QSerialPort::NoParity);
                implantPort->setStopBits(QSerialPort::OneStop);
                implantPort->setFlowControl(QSerialPort::NoFlowControl);
                implantPort->setReadBufferSize(2100);

                ADCPort->setDataBits(QSerialPort::Data8);
                ADCPort->setParity(QSerialPort::NoParity);
                ADCPort->setStopBits(QSerialPort::OneStop);
                ADCPort->setFlowControl(QSerialPort::NoFlowControl);
                break;
            }
//        }
//        else{
//        }
    }
    if (implantPort->open(QIODevice::ReadWrite)){
        implantConnected = true;
        for(int i = 0; i < 30; i++){
            implantPort->read(48000);
        }
        checked = false;
        qDebug() << "Implant Port connnected!";
    }
    if (ADCPort->open(QIODevice::ReadWrite)){
        ADCConnected = true;
        qDebug() << "ADC Port connnected!";
    }
    if (arduinoPort->open(QIODevice::ReadWrite)){
        qDebug() << "Arduino Port connnected!";
    }
}

void SerialChannel::sendMovementOne(){
    QString temp;
    temp = "0";
    arduinoPort->write(temp.toLocal8Bit());
}

void SerialChannel::sendMovementTwo(){
    QString temp;
    temp = "1";
    arduinoPort->write(temp.toLocal8Bit());
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
            ADCPort->setBaudRate(1333333);
            ADCPort->setDataBits(QSerialPort::Data8);
            ADCPort->setParity(QSerialPort::NoParity);
            ADCPort->setStopBits(QSerialPort::OneStop);
            ADCPort->setFlowControl(QSerialPort::NoFlowControl);
            ADCPort->setReadBufferSize(2100);

            implantPort->setPortName(portInfo.at(i).portName());
            implantPort->setBaudRate(1333333);
            implantPort->setDataBits(QSerialPort::Data8);
            implantPort->setParity(QSerialPort::NoParity);
            implantPort->setStopBits(QSerialPort::OneStop);
            implantPort->setFlowControl(QSerialPort::NoFlowControl);
            implantPort->setReadBufferSize(2100);
            portOrder = 2;
            break;
        }
        else if(portOrder == 2){
            ADCPort->setPortName(portInfo.at(i).portName());
            ADCPort->setBaudRate(1333333);
            ADCPort->setDataBits(QSerialPort::Data8);
            ADCPort->setParity(QSerialPort::NoParity);
            ADCPort->setStopBits(QSerialPort::OneStop);
            ADCPort->setFlowControl(QSerialPort::NoFlowControl);
            ADCPort->setReadBufferSize(2100);

            implantPort->setPortName(portInfo.at(i+1).portName());
            implantPort->setBaudRate(1333333);
            implantPort->setDataBits(QSerialPort::Data8);
            implantPort->setParity(QSerialPort::NoParity);
            implantPort->setStopBits(QSerialPort::OneStop);
            implantPort->setFlowControl(QSerialPort::NoFlowControl);
            implantPort->setReadBufferSize(2100);
            portOrder = 1;
            break;
        }
    }
    ADCPort->open(QIODevice::ReadWrite);
    implantPort->open(QIODevice::ReadWrite);
}

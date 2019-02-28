#include "serialchannel.h"

SerialChannel::SerialChannel(QObject *parent, Command *NeutrinoCommand_, DataProcessor *NeutrinoData_, Channel *NeutrinoChannel_) : QObject(parent){
    serialData = new QSerialPort(this);
    serialCommand = new QSerialPort(this);
    NeutrinoCommand = NeutrinoCommand_;
    NeutrinoData = NeutrinoData_;
    NeutrinoChannel = NeutrinoChannel_;

    File = new QFile;

    connect(serialData, SIGNAL(readyRead()), this, SLOT(readData()));
}

void SerialChannel::readData(){
    switch (NeutrinoCommand->getOPMode()){
        case 2:{
            if(NeutrinoData->isPlotEnabled()){
//                NeutrinoData->MultiplexChannelData(NeutrinoData->ParseFrameMarkers8bits(serialData->read(2048)));
            }
            break;
        }
        case 5:
        case 7:{
            QByteArray temp;
            temp = serialData->read(2048);
            if(record){
                for(int i = 0; i < temp.size(); i++){
                    *out << (uint8_t) temp.at(i) << "\n";
                }
            }
            emit singleByteReady(NeutrinoData->signalReconstruction((char) temp.at(0)));
            break;
        }
        case 10:
        case 11:{
            QByteArray temp;
            temp = serialData->read(2048);
            quint32 tempChar = temp.at(0) + temp.at(1) + temp.at(2) + temp.at(3);
            tempChar = tempChar/4;
            currentByte = (char) tempChar;
            break;
        }
        default:
            break;
    }
}

void SerialChannel::setRecordEnabled(bool flag){
    if(flag){
        if(NeutrinoCommand->getOPMode() == 5){
            fileName = directory + QDateTime::currentDateTime().toString("'Analog_'yyyyMMdd_HHmmss'.csv'");
        }
        else if(NeutrinoCommand->getOPMode() == 7){
            fileName = directory + QDateTime::currentDateTime().toString("'BioImpedance_'yyyyMMdd_HHmmss'.csv'");
        }
        File->setFileName(fileName);
        File->open(QIODevice::WriteOnly|QIODevice::Text)? qDebug()<< "File opened" : qDebug() << "File failed to open";
        out = new QTextStream(File);
    }
    else{
        File->close();
        qDebug() << "File closed";
    }
    record = flag;
}

char SerialChannel::getCurrentByte(){
    return currentByte;
}

void SerialChannel::closePort(){
    serialData->close();
}

bool SerialChannel::doConnect(){
    portInfo = QSerialPortInfo::availablePorts();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        qDebug() << "Name : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();
        qDebug() << "Serial Number: " << info.serialNumber();
    }
    for(int i = 0; i < portInfo.size(); i++){
        if(portInfo.at(i).manufacturer() == "FTDI" && portInfo.at(i+1).manufacturer() == "FTDI"){
            if(portInfo.at(i+1).portName().at(portInfo.at(i+1).portName().size()-1).digitValue()
                > portInfo.at(i).portName().at(portInfo.at(i+1).portName().size()-1).digitValue()){
                serialData->setPortName(portInfo.at(i).portName());
                serialData->setBaudRate(3000000);
                qDebug() << "Data connected to " << portInfo.at(i).portName();

                serialCommand->setPortName(portInfo.at(i+1).portName());
                serialCommand->setBaudRate(19200);
                qDebug() << "Command connected to " << portInfo.at(i+1).portName();
                serialData->setDataBits(QSerialPort::Data8);
                serialData->setParity(QSerialPort::NoParity);
                serialData->setStopBits(QSerialPort::OneStop);
                serialData->setFlowControl(QSerialPort::NoFlowControl);
                serialData->setReadBufferSize(2048);

                serialCommand->setDataBits(QSerialPort::Data8);
                serialCommand->setParity(QSerialPort::EvenParity);
                serialCommand->setStopBits(QSerialPort::OneStop);
                serialCommand->setFlowControl(QSerialPort::NoFlowControl);
                break;
            }
        }
    }
    if (serialData->open(QIODevice::ReadWrite) && serialCommand->open(QIODevice::ReadWrite)) {
        connected = true;
        qDebug() << "Connected via USB!";
        return 1;
    }
    else{
        return 0;
    }
}

bool SerialChannel::isConnected(){
    return connected;
}

void SerialChannel::setBaudRate(int baud){
    serialCommand->setBaudRate(baud);
    qDebug() << "Baud rate set to: " << baud;
}

bool SerialChannel::writeCommand(QByteArray Command){
    if(connected){
        serialCommand->write(Command);         //write the command itself
        return true;
    }
    else
        return false;
}

void SerialChannel::swapPort(){
    serialCommand->close();
    serialData->close();
    portInfo = QSerialPortInfo::availablePorts();
    for(int i = 0; i < portInfo.size(); i++){
        if(portOrder == 1){
            serialData->setPortName(portInfo.at(i+1).portName());
            serialData->setBaudRate(3000000);
            serialData->setDataBits(QSerialPort::Data8);
            serialData->setParity(QSerialPort::NoParity);
            serialData->setStopBits(QSerialPort::OneStop);
            serialData->setFlowControl(QSerialPort::NoFlowControl);
            serialData->setReadBufferSize(512);

            serialCommand->setPortName(portInfo.at(i).portName());
            serialCommand->setBaudRate(19200);
            serialCommand->setDataBits(QSerialPort::Data8);
            serialCommand->setParity(QSerialPort::EvenParity);
            serialCommand->setStopBits(QSerialPort::OneStop);
            serialCommand->setFlowControl(QSerialPort::NoFlowControl);
            portOrder = 2;
            break;
        }
        else if(portOrder == 2){
            serialData->setPortName(portInfo.at(i).portName());
            serialData->setBaudRate(3000000);
            serialData->setDataBits(QSerialPort::Data8);
            serialData->setParity(QSerialPort::NoParity);
            serialData->setStopBits(QSerialPort::OneStop);
            serialData->setFlowControl(QSerialPort::NoFlowControl);
            serialData->setReadBufferSize(512);

            serialCommand->setPortName(portInfo.at(i+1).portName());
            serialCommand->setBaudRate(19200);
            serialCommand->setDataBits(QSerialPort::Data8);
            serialCommand->setParity(QSerialPort::EvenParity);
            serialCommand->setStopBits(QSerialPort::OneStop);
            serialCommand->setFlowControl(QSerialPort::NoFlowControl);
            portOrder = 1;
            break;
        }
    }
    serialData->open(QIODevice::ReadWrite);
    serialCommand->open(QIODevice::ReadWrite);
}

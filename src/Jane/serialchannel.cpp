#include "serialchannel.h"

SerialChannel::SerialChannel(QObject *parent, Command *ThorCommand_, DataProcessor *ThorData_, Channel *ThorChannel_) : QObject(parent)
{
    serialData = new QSerialPort(this);
    serialCommand = new QSerialPort(this);
    ThorCommand = ThorCommand_;
    ThorData = ThorData_;
    ThorChannel = ThorChannel_;

    connect(serialData, SIGNAL(readyRead()), this, SLOT(ReadData()));
}

void SerialChannel::ReadData(){
    switch (ThorCommand->getOPMode()){
    case 1:{        //BER
        qDebug()<<"BER read";
    }
    case 2: {       //JTAG
        qDebug()<<"jtag read";
    }
    case 7:{        //analog
        qDebug()<<"analog read";
        if(isData){
            emit singleByteReady(ThorData->signalReconstruction(serialData->read(1)));
            serialData->readAll();
        }
        if((!isData) && (ThorData->first_AnalogMeasurementFrameMarker(serialData->readAll())!=0)){
            isData=true;
        }
        break;
    }
    case 8:{        //bioimp
        qDebug()<<"bioimp read";
        emit singleByteReady(ThorData->signalReconstruction(serialData->read(1)));
        serialData->readAll();
         qDebug() << serialData->readAll();
        break;
    }
    default:
        break;
    }
}

void SerialChannel::closePort(){
    serialData->close();
}

bool SerialChannel::doConnect(){
    ListAllPort();
    InitializePort(PORT_TYPE_CMD,PORT_MANUF_CMD);
//    InitializePort(PORT_TYPE_DATA,PORT_DATA);
    return (ConnectPort(PORT_TYPE_CMD) && ConnectPort(PORT_TYPE_DATA));
}

bool SerialChannel::isConnected(){
    return connected;
}

bool SerialChannel::writeCommand(QByteArray Command){
        serialCommand->write(Command);         //write the command itself
        return true;
}

int SerialChannel::getNumChannels(QByteArray lastCommand){
    int numChannels = 0;
    for(int i=7;i<17;i++){
        if (lastCommand.at(i) == (const char) CHANNEL_ON){
            numChannels++;
        }
    }
    return numChannels;
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
            serialCommand->setBaudRate(48000);
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
            serialCommand->setBaudRate(48000);
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

void SerialChannel::ListAllPort()
{
    qDebug() << "Listing all serial ports available:";

    portInfo = QSerialPortInfo::availablePorts();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        qDebug() << "Name : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();
        qDebug() << "Serial Number: " << info.serialNumber();
    }
}

void SerialChannel::InitializePort(QString portType, QString manufacturer)
{
    qDebug() << "Finding USB port for " << portType;
    for(int i = 0; i < portInfo.size(); i++){
        if(portInfo.at(i).manufacturer() == manufacturer){
            qDebug() << "setting up USB port for " <<portType;
            serialCommand->setPortName(portInfo.at(i).portName());
            serialCommand->setBaudRate(48000);

            serialCommand->setDataBits(QSerialPort::Data8);
            serialCommand->setParity(QSerialPort::EvenParity);
            serialCommand->setStopBits(QSerialPort::OneStop);
            serialCommand->setFlowControl(QSerialPort::NoFlowControl);
            qDebug() << portType << serialCommand->portName() << serialCommand->dataBits()
                     <<serialCommand->parity() << serialCommand->stopBits() << serialCommand->flowControl();
            break;
        }
    }

    return;
}

bool SerialChannel::ConnectPort(QString portType)
{
    qDebug() << "Attempt to connect for" << portType;
    if (portType == PORT_TYPE_DATA){
        return true;
    }
    if (serialCommand->open(QIODevice::ReadWrite)) {
        connected = true;
        qDebug() << "Connected via USB!";
        return true;
    }
    else{
        qDebug() << "Failed to connect via USB!";
        connected = false;
        return false;
    }

    qDebug() << "No such portType as " << portType;
    return false;

}

#include "serialchannel.h"

SerialChannel::SerialChannel(QObject *parent, Command *NeutrinoCommand_, DataProcessor *NeutrinoData_, Channel *NeutrinoChannel_) : QObject(parent)
{
    serialData = new QSerialPort(this);
    serialCommand = new QSerialPort(this);
    NeutrinoCommand = NeutrinoCommand_;
    NeutrinoData = NeutrinoData_;
    NeutrinoChannel = NeutrinoChannel_;

    File = new QFile;

    connect(serialData, SIGNAL(readyRead()), this, SLOT(ReadData()));
}

void SerialChannel::ReadData(){
    switch (NeutrinoCommand->getOPMode()){
        case 2:{
            if(NeutrinoData->isPlotEnabled()){
                NeutrinoData->MultiplexChannelData(NeutrinoData->ParseFrameMarkers8bits(serialData->read(2048)));
                break;
            }
        }
        case 5:{
            if(record){
                QByteArray temp;
                temp = serialData->read(2048);
                for(int i = 0; i < temp.size(); i++){
                    *out << (int) temp.at(i) << "\n";
                }
                emit singleByteReady(NeutrinoData->signalReconstruction((char) temp.at(0)));
            }
            else{
                QByteArray temp;
                temp = serialData->read(2048);
                emit singleByteReady(NeutrinoData->signalReconstruction((char) temp.at(0)));
            }
            break;
        }
        case 7:{
            emit singleByteReady(NeutrinoData->signalReconstruction((char) serialData->read(1).at(0)));
            serialData->readAll();
            break;
        }
        case 10:{
            currentByte = (char) serialData->read(2048).at(0);
            break;
        }
        case 11:{
            currentByte = (char) serialData->read(2048).at(0);
            break;
        }
    default:
        break;
    }
}

void SerialChannel::setRecordEnabled(bool flag){
    if(flag){
        fileName = directory + QDateTime::currentDateTime().toString("'Analog_'yyyyMMdd_HHmmss'.csv'");
        File->setFileName(fileName);
        if(File->open(QIODevice::WriteOnly|QIODevice::Text)){
            qDebug()<< "File opened";
        }
        else{
            qDebug() << "File failed to open";
        }
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

bool SerialChannel::writeCommand(QByteArray Command){
    if(connected){
        if(Command.size()>5){
            if(Command.at(6) == (char) WL_8){
                NeutrinoData->setBitMode(true);
                NeutrinoData->setPlotEnabled(true);
                NeutrinoData->clearallChannelData();
                qDebug() << "8 Bit Mode";
                NeutrinoChannel->setNumChannels(getNumChannels(Command));
            }
            else if(Command.at(6) == (char) WL_10){
                NeutrinoData->setBitMode(false);
                NeutrinoData->setPlotEnabled(true);
                NeutrinoData->clearallChannelData();
                qDebug() << "10 Bit Mode";
                NeutrinoChannel->setNumChannels(getNumChannels(Command));
            }
            else {
                NeutrinoData->setPlotEnabled(false);
            }
        }
        serialCommand->write(Command);         //write the command itself
        return true;
    }
    else
        return false;
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

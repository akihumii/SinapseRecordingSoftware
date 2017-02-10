#include "serialchannel.h"

SerialChannel::SerialChannel(QObject *parent, Command *NeutrinoCommand_, DataProcessor *NeutrinoData_, Channel *NeutrinoChannel_) : QObject(parent)
{
    serialData = new QSerialPort(this);
    serialCommand = new QSerialPort(this);
    NeutrinoCommand = NeutrinoCommand_;
    NeutrinoData = NeutrinoData_;
    NeutrinoChannel = NeutrinoChannel_;

    connect(serialData, SIGNAL(readyRead()), this, SLOT(ReadData()));
}

void SerialChannel::ReadData(){
    if(NeutrinoCommand->getOPMode() == 2 || NeutrinoCommand->getOPMode() == 3 && !readBER){
        NeutrinoData->MultiplexChannelData(NeutrinoData->ParseFrameMarkers8bits(serialData->read(2048)));
    }
    else if (NeutrinoCommand->getOPMode() == 1 && readBER){
        if(!NeutrinoData->isBERlocked()){
            NeutrinoData->processBER(NeutrinoCommand->getBERbyte(),
                                     NeutrinoData->lockBERstream(NeutrinoCommand->getBERbyte(),
                                                                 serialData->read(2048)));
        }
        else{
            NeutrinoData->processBER(NeutrinoCommand->getBERbyte(), serialData->read(2048));
        }
    }
    else{
        serialData->read(2048);
    }
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
    }
    for(int i = 0; i < portInfo.size(); i++){
        if(portInfo.at(i).manufacturer() == "FTDI" && portInfo.at(i+1).manufacturer() == "FTDI"){
            if(portInfo.at(i+1).portName().split("COM")[1].toInt() < portInfo.at(i).portName().split("COM")[1].toInt()){
                serialData->setPortName(portInfo.at(i+1).portName());
                serialData->setBaudRate(3000000);

                serialCommand->setPortName(portInfo.at(i).portName());
                serialCommand->setBaudRate(19200);
            }
            else{
                serialData->setPortName(portInfo.at(i).portName());
                serialData->setBaudRate(3000000);

                serialCommand->setPortName(portInfo.at(i+1).portName());
                serialCommand->setBaudRate(19200);
            }
            serialData->setDataBits(QSerialPort::Data8);
            serialData->setParity(QSerialPort::EvenParity);
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
                readBER = false;
            }
            else if(Command.at(6) == (char) WL_10){
                NeutrinoData->setBitMode(false);
                NeutrinoData->setPlotEnabled(true);
                NeutrinoData->clearallChannelData();
                qDebug() << "10 Bit Mode";
                NeutrinoChannel->setNumChannels(getNumChannels(Command));
                readBER = false;
            }
            else if(Command.at(6) == (char) DATA_BER_MODE){
                readBER = true;
                // Invoke BER Counter Dialog here //
//                BERDialog berDialog(NeutrinoData);
//                berDialog.exec();
            }
            else {
                NeutrinoData->setPlotEnabled(false);
                readBER = false;
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

void SerialChannel::restartBER(){
    NeutrinoData->resetBER();
}

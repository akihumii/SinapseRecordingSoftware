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
    serialData = new QSerialPort(this);
    serialCommand = new QSerialPort(this);
    NeutrinoCommand = NeutrinoCommand_;
    NeutrinoData = NeutrinoData_;
    NeutrinoChannel = NeutrinoChannel_;

    connect(serialData, SIGNAL(readyRead()), this, SLOT(ReadData()));
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
    NeutrinoData->MultiplexChannelData(NeutrinoData->ParseFrameMarkers8bits(serialData->read(2048)));
}

void SerialChannel::closePort(){
    serialData->close();
}

void SerialChannel::connectSylph(){
    portInfo = QSerialPortInfo::availablePorts();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        qDebug() << "Name : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();
    }
    for(int i = 0; i < portInfo.size(); i++){
        if(portInfo.at(i).manufacturer() == "FTDI" && portInfo.at(i+1).manufacturer() == "FTDI"){
            if(portInfo.at(i+1).portName().split("COM")[1].toInt() > portInfo.at(i).portName().split("COM")[1].toInt()){
                implantPort->setPortName(portInfo.at(i+1).portName());
                implantPort->setBaudRate(1333333);

                ADCPort->setPortName(portInfo.at(i).portName());
                ADCPort->setBaudRate(1333333);
            }
            else{
                implantPort->setPortName(portInfo.at(i).portName());
                implantPort->setBaudRate(1333333);

                ADCPort->setPortName(portInfo.at(i+1).portName());
                ADCPort->setBaudRate(1333333);
            }
            implantPort->setDataBits(QSerialPort::Data8);
            implantPort->setParity(QSerialPort::NoParity);
            implantPort->setStopBits(QSerialPort::OneStop);
            implantPort->setFlowControl(QSerialPort::NoFlowControl);
            implantPort->setReadBufferSize(2048);

            ADCPort->setDataBits(QSerialPort::Data8);
            ADCPort->setParity(QSerialPort::NoParity);
            ADCPort->setStopBits(QSerialPort::OneStop);
            ADCPort->setFlowControl(QSerialPort::NoFlowControl);
            break;
        }
    }
    if (implantPort->open(QIODevice::ReadWrite)){
        implantConnected = true;
        qDebug() << "Implant Port connnected!";
    }
    if (ADCPort->open(QIODevice::ReadWrite)){
        ADCConnected = true;
        qDebug() << "ADC Port connnected!";
    }
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
    if (serialData->open(QIODevice::ReadWrite) && serialCommand->open(QIODevice::ReadWrite)) {
        connected = true;
        qDebug() << "Connected via USB!";
        return 1;
    }
    else{
        return 0;
    }

}
bool SerialChannel::isImplantConnected(){
    return implantConnected;
}

bool SerialChannel::isADCConnected(){
    return ADCConnected;
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
#ifdef NEUTRINO_II
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
#endif

#ifdef SYLPH
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
            ADCPort->setReadBufferSize(2048);

            implantPort->setPortName(portInfo.at(i).portName());
            implantPort->setBaudRate(1333333);
            implantPort->setDataBits(QSerialPort::Data8);
            implantPort->setParity(QSerialPort::NoParity);
            implantPort->setStopBits(QSerialPort::OneStop);
            implantPort->setFlowControl(QSerialPort::NoFlowControl);
            implantPort->setReadBufferSize(2048);
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
            ADCPort->setReadBufferSize(2048);

            implantPort->setPortName(portInfo.at(i+1).portName());
            implantPort->setBaudRate(1333333);
            implantPort->setDataBits(QSerialPort::Data8);
            implantPort->setParity(QSerialPort::NoParity);
            implantPort->setStopBits(QSerialPort::OneStop);
            implantPort->setFlowControl(QSerialPort::NoFlowControl);
            implantPort->setReadBufferSize(2048);
            portOrder = 1;
            break;
        }
    }
    ADCPort->open(QIODevice::ReadWrite);
    implantPort->open(QIODevice::ReadWrite);
#endif
}
#ifndef SERIALCHANNEL_H
#define SERIALCHANNEL_H

#include "qtincludes.h"
#include "dataprocessor.h"
#include "command.h"
#include "channel.h"


class SerialChannel : public QObject{
    Q_OBJECT
public:
    SerialChannel(QObject *parent, DataProcessor *dataProcessor_);
    SerialChannel(QObject *parent, Command *NeutrinoCommand_, DataProcessor *NeutrinoData_, Channel *NeutrinoChannel_);
    void closeImplantPort();
    void closeADCPort();
    bool enableImplantPort(QString portName);
    bool enableADCPort(QString portName);
    bool serialenabled = true;
    void closePort();
    bool doConnect();
    bool isConnected();
    bool writeCommand(QByteArray Command);
public slots:
    void ReadImplantData();
    void ReadADCData();
    void ReadData();
private:
    QSerialPort *serialData;
    QSerialPort *serialCommand;
    QSerialPort *implantPort;
    QSerialPort *ADCPort;
    DataProcessor *dataProcessor;

    Channel *NeutrinoChannel;
    Command *NeutrinoCommand;
    DataProcessor *NeutrinoData;

    QList<QSerialPortInfo> portInfo;
    bool connected = false;
    int getNumChannels(QByteArray lastCommand);
};

#endif // SerialChannel_H

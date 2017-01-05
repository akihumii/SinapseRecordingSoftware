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
    bool serialenabled = false;
    void closePort();
    bool doConnect();
public slots:
    void ReadImplantData();
    void ReadADCData();
    void ReadData();
private:
    QSerialPort *serial;
    QSerialPort *implantPort;
    QSerialPort *ADCPort;
    DataProcessor *dataProcessor;

    Channel *NeutrinoChannel;
    Command *NeutrinoCommand;
    DataProcessor *NeutrinoData;


};

#endif // SerialChannel_H

#ifndef SERIALCHANNEL_H
#define SERIALCHANNEL_H

#include "qtincludes.h"
#include "dataprocessor_ma.h"


class SerialChannel : public QObject{
    Q_OBJECT
public:
    SerialChannel(QObject *parent, DataProcessor_MA *dataProcessor_);
    void closeImplantPort();
    void closeADCPort();
    bool enableImplantPort(QString portName);
    bool enableADCPort(QString portName);
public slots:
    void ReadImplantData();
    void ReadADCData();
private:

    QSerialPort *serial;
    QSerialPort *implantPort;
    QSerialPort *ADCPort;
    DataProcessor_MA *dataProcessor;
};

#endif // SerialChannel_H

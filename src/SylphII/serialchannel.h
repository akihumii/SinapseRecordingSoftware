#ifndef SERIALCHANNEL_H
#define SERIALCHANNEL_H

#include "../common/qtincludes.h"
#include "dataprocessor.h"


class SerialChannel : public QObject{
    Q_OBJECT
public:
    SerialChannel(QObject *parent, DataProcessor *dataProcessor_);
    void closeImplantPort();
    void closeADCPort();
    void closeSyncPort();
    bool enableImplantPort(QString portName);
    bool enableADCPort(QString portName);
    bool serialenabled = true;
    bool doConnect();
    bool isConnected();
    void swapPort();
    void connectSylph();
    bool connectSyncPort();
    bool isImplantConnected();
    bool isADCConnected();
public slots:
    void ReadImplantData();
    void ReadADCData();
    void flushSyncPort();
private:
    QSerialPort *implantPort;
    QSerialPort *ADCPort;
    QSerialPort *syncPort;
    DataProcessor *dataProcessor;

    bool checked = false;

    QList<QSerialPortInfo> portInfo;
    bool connected = false;
    bool implantConnected = false;
    bool ADCConnected = false;
    int portOrder =2;
};

#endif // SerialChannel_H

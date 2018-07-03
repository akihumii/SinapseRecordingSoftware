#ifndef SERIALCHANNEL_H
#define SERIALCHANNEL_H

#include "../common/qtincludes.h"
#include "dataprocessor.h"

namespace SylphX {

class SerialChannel : public QObject{
    Q_OBJECT
public:
    SerialChannel(QObject *parent, DataProcessor *dataProcessor_);
    void closeImplantPort();
    void closeADCPort();
    bool enableImplantPort(QString portName);
    bool enableADCPort(QString portName);
    bool serialenabled = true;
    bool doConnect();
    bool isConnected();
    void swapPort();
    void connectSylph();
    bool isImplantConnected();
    bool isADCConnected();
    void flushADC();
public slots:
    void ReadImplantData();
    void ReadADCData();
private:
    QSerialPort *implantPort;
    QSerialPort *ADCPort;
    DataProcessor *dataProcessor;

    bool checked = false;

    QList<QSerialPortInfo> portInfo;
    bool connected = false;
    bool implantConnected = false;
    bool ADCConnected = false;
    int portOrder = 1;
    qint64 packetSize = 25;
    qint64 maxSize = packetSize*60;
};

}

#endif // SerialChannel_H

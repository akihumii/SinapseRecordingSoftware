#ifndef SERIALCHANNEL_H
#define SERIALCHANNEL_H

#include "../common/qtincludes.h"

namespace SylphX {

#define BAUDRATE 2000000

class SerialChannel : public QObject{
    Q_OBJECT
public:
    SerialChannel(QObject *parent);
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
    int getRate();
    void setChecked(bool flag);
public slots:
    void ReadImplantData();
    void ReadADCData();
    void updateRate();
private:
    QSerialPort *implantPort;
    QSerialPort *ADCPort;
    QTimer *timer;

    bool checked = false;

    QList<QSerialPortInfo> portInfo;
    bool connected = false;
    bool implantConnected = false;
    bool ADCConnected = false;
    int portOrder = 1;
    qint64 packetSize = 25;
    qint64 maxSize = packetSize*100;
    int bytesRead = 0;
    int temp;
    double rate = 0.0;
};

}

#endif // SerialChannel_H

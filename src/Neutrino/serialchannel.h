#ifndef SERIALCHANNEL_H
#define SERIALCHANNEL_H

#include "../common/qtincludes.h"
#include "dataprocessor.h"
#include "command.h"
#include "channel.h"


class SerialChannel : public QObject{
    Q_OBJECT
public:
    SerialChannel(QObject *parent, Command *NeutrinoCommand_, DataProcessor *NeutrinoData_, Channel *NeutrinoChannel_);
    bool serialenabled = true;
    void closePort();
    bool doConnect();
    bool isConnected();
    bool writeCommand(QByteArray Command);
    void swapPort();
public slots:
    void ReadData();
private:
    QSerialPort *serialData;
    QSerialPort *serialCommand;
    DataProcessor *dataProcessor;

    Channel *NeutrinoChannel;
    Command *NeutrinoCommand;
    DataProcessor *NeutrinoData;

    QList<QSerialPortInfo> portInfo;
    bool connected = false;
    int getNumChannels(QByteArray lastCommand);
    int portOrder =2;
};

#endif // SerialChannel_H

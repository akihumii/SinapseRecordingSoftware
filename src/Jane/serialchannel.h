#ifndef SERIALCHANNEL_H
#define SERIALCHANNEL_H

#include "../common/qtincludes.h"
#include "dataprocessor.h"
#include "command.h"
#include "channel.h"

#define PORT_CMD "COM1"
#define PORT_CMD_TYPE "PORT_COMMAND"
#define PORT_DATA "COM1"
#define PORT_DATA_TYPE "PORT_DATA"



class SerialChannel : public QObject{

    Q_OBJECT

public:
    SerialChannel(QObject *parent, DataProcessor *ThorData_, Channel *ThorChannel_);
    bool serialenabled = true;
    void closePort();
    bool doConnect();
    bool isConnected();
    bool writeCommand(QByteArray Command);
    void swapPort();

public slots:
    void ReadData();
private:
    QList<QSerialPortInfo> portInfo;

    QSerialPort *serialData;
    QSerialPort *serialCommand;
    DataProcessor *dataProcessor;

    Channel *ThorChannel;
    Command *ThorCommand;
    DataProcessor *ThorData;

    bool connected = false;
    int getNumChannels(QByteArray lastCommand);
    int portOrder =2;

    void ListAllPort();
    void InitializePort(QString,QString);
    bool ConnectPort(QString);

signals:
    void singleByteReady(double temp);
};

#endif // SerialChannel_H

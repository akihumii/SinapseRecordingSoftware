#ifndef SERIALCHANNEL_H
#define SERIALCHANNEL_H

#include "../common/qtincludes.h"
#include "dataprocessor.h"
#include "command.h"
#include "channel.h"



#define PORT_TYPE_CMD "CMD"
#define PORT_TYPE_DATA "DATA"
#define PORT_CMD "COM8"
#define PORT_DATA "COM3"


class SerialChannel : public QObject{
    Q_OBJECT
public:



    SerialChannel(QObject *parent, Command *ThorCommand_, DataProcessor *ThorData_, Channel *ThorChannel_);
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
    bool isData = false;
    int getNumChannels(QByteArray lastCommand);
    int portOrder =2;

    void ListAllPort();
    void InitializePort(QString,QString);
    bool ConnectPort(QString);

signals:
    void singleByteReady(double temp);
};

#endif // SerialChannel_H

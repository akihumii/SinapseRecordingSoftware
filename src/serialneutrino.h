#ifndef SERIALNEUTRINO_H
#define SERIALNEUTRINO_H

#include "qtincludes.h"
#include "dataprocessor_ka.h"
#include "command.h"
#include "channel.h"


class SerialNeutrino : public QObject
{
    Q_OBJECT
public:
    SerialNeutrino(QObject *parent, Command *NeutrinoCommand_, DataProcessor_KA *NeutrinoData_, Channel *NeutrinoChannel_);
    bool serialenabled = false;
    void closePort();
    bool doConnect();
public slots:
    void ReadData();
private:
    QSerialPort *serial;
    Channel *NeutrinoChannel;
    Command *NeutrinoCommand;
    DataProcessor_KA *NeutrinoData;
};

#endif // SerialNeutrino_H

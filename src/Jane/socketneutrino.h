#ifndef SOCKETNEUTRINO_H
#define SOCKETNEUTRINO_H

#include "../common/qtincludes.h"
#include "command.h"
#include "dataprocessor.h"
#include "../common/socketabstract.h"

class Command;

class SocketNeutrino : public SocketAbstract {
    Q_OBJECT
public:
    SocketNeutrino(Command *NeutrinoCommand_, DataProcessor *NeutrinoData_, Channel *NeutrinoChannel_);
    bool writeCommand(QByteArray Command);
    bool wifiEnabled = true;
    QByteArray getlastCommand();

private:
    //QTcpSocket *socketNeutrino;
    Channel *NeutrinoChannel;
    Command *NeutrinoCommand;
    DataProcessor *NeutrinoData;

    qint64 maxSize = 20480;
    int numChannels;
    QByteArray lastSentCommand;

    bool Mode_8Bit = false;

    int getNumChannels(QByteArray lastCommand);

private slots:
    void ReadCommand();
};

#endif // SocketNeutrino_H

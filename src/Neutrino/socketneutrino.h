#ifndef SOCKETNEUTRINO_H
#define SOCKETNEUTRINO_H

#include "../common/qtincludes.h"
#include "command.h"
#include "dataprocessor.h"
#include "../common/socketabstract.h"

class SocketNeutrino : public SocketAbstract {
    Q_OBJECT
public:
    SocketNeutrino(Command *NeutrinoCommand_, DataProcessor *NeutrinoData_, Channel *NeutrinoChannel_);
    bool writeCommand(QByteArray Command);
    bool wifiEnabled = true;
    QByteArray getlastCommand();
    char getCurrentByte();

private:
    QTcpSocket *socketNeutrino;
    Channel *NeutrinoChannel;
    Command *NeutrinoCommand;
    DataProcessor *NeutrinoData;

    qint64 maxSize = 40960;
    int numChannels;
    QByteArray lastSentCommand;

    bool Mode_8Bit = false;
    char currentByte;
    int getNumChannels(QByteArray lastCommand);

private slots:
    void ReadCommand();
signals:
    void singleByteReady(double temp);
};

#endif // SocketNeutrino_H

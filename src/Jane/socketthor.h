#ifndef SOCKETTHOR_H
#define SOCKETTHOR_H

#include "../common/qtincludes.h"
#include "command.h"
#include "dataprocessor.h"
#include "../common/socketabstract.h"
#include "channel.h"

class SocketThor : public SocketAbstract {
    Q_OBJECT
public:
    SocketThor(QObject *parent, Command *ThorCommand_, DataProcessor *ThorData_, Channel *ThorChannel_);
    bool writeCommand(QByteArray Command);
    bool wifiEnabled = true;
    QByteArray getlastCommand();

private:
    //QTcpSocket *socketThor;
    Channel *ThorChannel;
    Command *ThorCommand;
    DataProcessor *ThorData;

    qint64 maxSize = 20480;
    int numChannels;
    QByteArray lastSentCommand;

    int getNumChannels(QByteArray lastCommand);

private slots:
    void ReadCommand();
};

#endif // SocketThor_H

#ifndef SOCKETEDISON_H
#define SOCKETEDISON_H

#include "../common/qtincludes.h"
#include "command.h"
#include "dataprocessor.h"

class SocketEdison : public QObject {
    Q_OBJECT
public:
    SocketEdison(QObject *parent, Command *NeutrinoCommand_, DataProcessor *NeutrinoData_, Channel *NeutrinoChannel_);
    void doConnect(QString ipAddress, int port);
    bool writeCommand(QByteArray Command);
    bool isConnected();
    void doDisconnect();
    QString getError();
    bool wifiEnabled = false;
    QByteArray getlastCommand();

private:
    QTcpSocket *socketCommand;
    Channel *NeutrinoChannel;
    Command *NeutrinoCommand;
    DataProcessor *NeutrinoData;

    qint64 maxSize = 2048;
    int numChannels;
    QByteArray lastSentCommand;

    bool Mode_8Bit;

    int getNumChannels(QByteArray lastCommand);

private slots:
    void connectedCommandSocket();
    void disconnectedCommandSocket();
    void ReadCommand();
};

#endif // SOCKETEDISON_H

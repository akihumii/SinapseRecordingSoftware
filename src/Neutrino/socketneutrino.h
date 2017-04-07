#ifndef SOCKETNEUTRINO_H
#define SOCKETNEUTRINO_H

#include "../common/qtincludes.h"
#include "command.h"
#include "dataprocessor.h"

class SocketNeutrino : public QObject {
    Q_OBJECT
public:
    SocketNeutrino(QObject *parent, Command *NeutrinoCommand_, DataProcessor *NeutrinoData_, Channel *NeutrinoChannel_);
    void doConnect(QString ipAddress, int port);
    bool writeCommand(QByteArray Command);
    bool isConnected();
    void doDisconnect();
    QString getError();
    bool wifiEnabled = true;
    QByteArray getlastCommand();

private:
    QTcpSocket *socketNeutrino;
    Channel *NeutrinoChannel;
    Command *NeutrinoCommand;
    DataProcessor *NeutrinoData;

    qint64 maxSize = 40960;
    int numChannels;
    QByteArray lastSentCommand;

    bool Mode_8Bit = false;

    int getNumChannels(QByteArray lastCommand);

private slots:
    void connectedCommandSocket();
    void disconnectedCommandSocket();
    void ReadCommand();
};

#endif // SocketNeutrino_H

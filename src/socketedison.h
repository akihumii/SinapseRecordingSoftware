#ifndef SOCKETEDISON_H
#define SOCKETEDISON_H

#include "qtincludes.h"
#include "command.h"
#include "dataprocessor_ka.h"
//#include "serialneutrino.h"

class SocketEdison : public QObject
{
    Q_OBJECT
public:

    SocketEdison(QObject *parent, Command *NeutrinoCommand_, DataProcessor_KA *NeutrinoData_, Channel *NeutrinoChannel_);//, SerialNeutrino *serialNeutrino_);
    void doConnect(QString ipAddress, int port);
    bool writeCommand(QByteArray Command);
    bool isConnected();
    void doDisconnect();
    QString getError();
    QString printRead();
    bool wifiEnabled = true;
    QByteArray getlastCommand();

private:

    QTcpSocket *socketCommand;
    Channel *NeutrinoChannel;
    Command *NeutrinoCommand;
    DataProcessor_KA *NeutrinoData;


    qint64 maxSize = 8192;
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

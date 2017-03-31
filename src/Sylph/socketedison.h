#ifndef SOCKETEDISON_H
#define SOCKETEDISON_H

#include "../common/qtincludes.h"
#include "dataprocessor.h"

class SocketEdison : public QObject {
    Q_OBJECT
public:
    SocketEdison(QObject *parent, DataProcessor *dataProcessor_);
    void doConnect(QString ipAddress, int port);
    bool isConnected();
    void doDisconnect();
    QString getError();
    bool wifiEnabled = true;

private:
    QTcpSocket *socketCommand;
    DataProcessor *dataProcessor;

    qint64 maxSize = 40960;

private slots:
    void connectedCommandSocket();
    void disconnectedCommandSocket();
    void ReadCommand();
};

#endif // SOCKETEDISON_H

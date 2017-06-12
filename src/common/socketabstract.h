#ifndef SOCKETABSTRACT_H
#define SOCKETABSTRACT_H

#include "qtincludes.h"

class SocketAbstract : public QObject{
    Q_OBJECT
public:
    SocketAbstract();
    void doConnect(QString ipAddress, int port);
    void doDisconnect();
    bool isConnected();
    QString getError();

protected:
    QTcpSocket *socketAbstract;

private slots:
    void connectedAbstractSocket();
    void disconnectedAbstractSocket();
};

#endif // SOCKETABSTRACT_H

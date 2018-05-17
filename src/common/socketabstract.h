#ifndef SOCKETABSTRACT_H
#define SOCKETABSTRACT_H

#include "qtincludes.h"

class SocketAbstract : public QObject{
    Q_OBJECT
public:
    SocketAbstract();
    bool doConnect(QString ipAddress, int port);
    void doDisconnect();
    bool isConnected();
    QString getError();

protected:
    QTcpSocket *socketAbstract;
};

#endif // SOCKETABSTRACT_H

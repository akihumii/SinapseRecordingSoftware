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
    int getRate();
    void setChecked(bool flag);
    void sendDisconnectSignal();
    void write(QByteArray command);

protected:
    QTcpSocket *socketAbstract;
    int bytesRead = 0;
    double rate = 0.0;
    bool checked = false;
    QTimer *timer;

protected slots:
    void updateRate();
};

#endif // SOCKETABSTRACT_H

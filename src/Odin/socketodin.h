#ifndef SOCKETODIN_H
#define SOCKETODIN_H

#include "../common/qtincludes.h"

class SocketOdin: public QObject {
    Q_OBJECT
public:
    SocketOdin(QObject *parent);
    void initOdin();
    void writeCommand(QByteArray command);
    void doConnect(QString ipAddress, int port);
    bool isConnected();
private slots:
    void connectedCommandSocket();
    void disconnectedCommandSocket();
private:
    QTcpSocket *socketOdin;
    QElapsedTimer *timer;
};

#endif // SOCKETODIN_H

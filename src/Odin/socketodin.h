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
    void sendCommand();
    void connectedCommandSocket();
    void disconnectedCommandSocket();
private:
    QTcpSocket *socketOdin;
    QElapsedTimer timer;
    QTimer commandTimer;
    QByteArray outgoingCommand;
    int commandCount = 0;
};

#endif // SOCKETODIN_H

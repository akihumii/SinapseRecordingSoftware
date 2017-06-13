#ifndef SOCKETODIN_H
#define SOCKETODIN_H

#include "../common/qtincludes.h"
#include "../common/socketabstract.h"

class SocketOdin: public SocketAbstract{
    Q_OBJECT
public:
    SocketOdin();
    void writeCommand(QByteArray command);
    void setReadDelay(int delay);
    QByteArray getIncomingCommand();
    QByteArray getOutgoingCommand();
signals:
    void odinDisconnected();
    void commandReceived(bool);
private slots:
    void sendCommand();
    void on_socketDisconnected();
    void readCommand();
private:
    QTcpSocket *socketOdin;
    QTimer commandTimer;
    QByteArray outgoingCommand;
    QByteArray incomingCommand;
    int commandCount = 0;
    bool timeToRead = false;
    int readDelay = 3000;
};

#endif // SOCKETODIN_H

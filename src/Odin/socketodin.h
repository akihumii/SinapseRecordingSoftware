#ifndef SOCKETODIN_H
#define SOCKETODIN_H

#include "../common/qtincludes.h"
#include "../common/socketabstract.h"

class SocketOdin: public SocketAbstract{
    Q_OBJECT
public:
    SocketOdin();
    void writeCommand(QByteArray command);
signals:
    void odinDisconnected();
private slots:
    void sendCommand();
    void on_socketDisconnected();
private:
    QTcpSocket *socketOdin;
    QTimer commandTimer;
    QByteArray outgoingCommand;
    int commandCount = 0;
};

#endif // SOCKETODIN_H

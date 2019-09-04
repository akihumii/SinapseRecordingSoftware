#ifndef SOCKETFORCE_H
#define SOCKETFORCE_H

#include "../common/qtincludes.h"
#include "../common/socketabstract.h"

namespace SylphX {

class SocketForce : public SocketAbstract{
    Q_OBJECT

public:
    SocketForce();
    void streamData(double data);
    void streamData(QVector<double> data);
    void setServer(int port);
    bool isConnectedSocketForce();

private:
    QTcpServer *server;
    QTcpSocket *socketForce;
    QDataStream *out;

private slots:
    void on_forcePort_connect();
    void on_disconnect();
};

}
#endif // SOCKETFORCE_H

#ifndef SOCKETODIN_H
#define SOCKETODIN_H

#include "../common/qtincludes.h"
#include "../common/socketabstract.h"

namespace Odin {

class SocketOdin: public SocketAbstract{
    Q_OBJECT
public:
    SocketOdin();
    ~SocketOdin();
    void writeCommand(QByteArray command);
    void setReadDelay(int delay);
    QByteArray getIncomingCommand();
    QByteArray getOutgoingCommand();
signals:
    void odinDisconnected();
private slots:
    void on_socketDisconnected();
    void readPrediction();
    void on_predictionPort_connect();
private:
    QTcpSocket *socketOdin;
    QTcpSocket *socketSylph;
    QUdpSocket *udpSocket;
    QByteArray outgoingCommand;
    QByteArray incomingCommand;
    QMediaPlayer *player;
    int commandCount = 0;
    bool timeToRead = false;
    int readDelay = 3000;
    QTcpServer *serverMatlabPredict;
    QTcpSocket *socketMatlabPredict;
    quint16 port = 1300;
};

}

#endif // SOCKETODIN_H

#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#include <QObject>
#include "socketabstract.h"
#include "qtincludes.h"

class SocketServer : public SocketAbstract {
    Q_OBJECT
public:
    SocketServer();
    void setIpAddress(QString address);
    void setPortNumber(int value);
    void doListen();
    void writeData(QByteArray data);
    void appendData(QVector<QString> data);
    void streamData();
    void clearData();
    bool isConnected();

private:
    QTcpServer *server;
    QTcpSocket *socket;
    QVector<QString> data;
    QDataStream *out;

    QString ipAddress;
    int portNumber;

    bool connectedFlag = false;

signals:
    void dataReady(double data);

private slots:
    void on_port_connect();
    void getData();
    void disableConnectedFlag();

public slots:

};

#endif // SOCKETSERVER_H

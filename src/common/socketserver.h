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
private:
    QTcpServer *server;
    QTcpSocket *socket;
    QVector<double> data;

    QString ipAddress;
    int portNumber;

signals:
    void dataReady(double data);

private slots:
    void on_port_connect();
    void getData();

public slots:

};

#endif // SOCKETSERVER_H

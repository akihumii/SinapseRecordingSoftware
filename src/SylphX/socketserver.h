#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#include <QObject>
#include "../common/socketabstract.h"
#include "../common/qtincludes.h"

class SocketServer : public SocketAbstract {
    Q_OBJECT
public:
    SocketServer();
private:
    QTcpServer *server;
    QTcpSocket *socketDyno;
    QVector<double> dynoData;

signals:
    void dynoDataReady(double data);

private slots:
    void on_dynoPort_connect();
    void getDynoData();

public slots:

};

#endif // SOCKETSERVER_H

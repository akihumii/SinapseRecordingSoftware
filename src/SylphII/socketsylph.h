#ifndef SOCKETSYLPH_H
#define SOCKETSYLPH_H

#include "../common/qtincludes.h"
#include "dataprocessor.h"

class SocketSylph : public QObject {
    Q_OBJECT
public:
    SocketSylph(QObject *parent, DataProcessor *dataProcessor_);
    void doConnect(QString ipAddress, int port);
    bool isConnected();
    void doDisconnect();
    QString getError();
    bool wifiEnabled = true;
    void discardData();

private:
    QTcpSocket *socketSylph;
    DataProcessor *dataProcessor;

    qint64 maxSize = 25200;

    bool checked = false;
    int initCount = 0;

private slots:
    void connectedCommandSocket();
    void disconnectedCommandSocket();
    void ReadCommand();
};

#endif // SOCKETSYLPH_H

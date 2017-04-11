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

private:
    QTcpSocket *socketSylph;
    DataProcessor *dataProcessor;

    qint64 maxSize = 21750;

    bool checked = false;

private slots:
    void connectedCommandSocket();
    void disconnectedCommandSocket();
    void ReadCommand();
};

#endif // SOCKETSYLPH_H

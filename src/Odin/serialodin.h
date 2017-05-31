#ifndef SERIALODIN_H
#define SERIALODIN_H

#include "../common/qtincludes.h"
#include <QObject>

class SerialOdin: public QObject{
    Q_OBJECT
public:
    SerialOdin(QObject *parent);
    void connectOdin();
    void closeOdinSerial();
    bool isOdinSerialConnected();
    void initOdin();
    void writeCommand(QByteArray command);

signals:
    void odinDisconnected();

private slots:
    void sendCommand();
    void disconnectedCommandSerial();

private:
    QSerialPort *odinPort;
    QList<QSerialPortInfo> portInfo;
    QTimer commandTimer;
    QByteArray outgoingCommand;
    int commandCount = 0;
    bool odinSerialConnected = false;
};

#endif // SERIALODIN_H

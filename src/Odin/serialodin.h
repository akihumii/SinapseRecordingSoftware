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
    void setReadDelay(int delay);
    QString getConnectedPort();
    QByteArray getIncomingCommand();

signals:
    void odinDisconnected();
    void commandReceived(bool);

private slots:
    void sendCommand();
    void checkConnectivity();
    void readCommand();

private:
    QSerialPort *odinPort;
    QList<QSerialPortInfo> portInfo;
    QTimer commandTimer;
    QTimer *serialTimer;
    QByteArray outgoingCommand;
    QByteArray incomingCommand;
    QString connectedPortName;
    int commandCount = 0;
    bool odinSerialConnected = false;
    bool timeToRead = false;
    int readDelay = 3000;
};

#endif // SERIALODIN_H

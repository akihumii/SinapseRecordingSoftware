#ifndef SERIALODIN_H
#define SERIALODIN_H

#include "../common/qtincludes.h"
#include <QObject>

class SerialOdin: public QObject{
    Q_OBJECT
public:
    SerialOdin(QObject *parent);
    void connectOdin();
    bool connectSyncPort();
    void closeOdinSerial();
    void closeSyncPort();
    bool isOdinSerialConnected();
    void initOdin();
    void writeCommand(QByteArray command);
    void setReadDelay(int delay);
    QString getConnectedPort();
    QByteArray getIncomingCommand();
    QByteArray getOutgoingCommand();

signals:
    void odinDisconnected();
    void commandReceived(bool);

private slots:
    void sendCommand();
    void checkConnectivity();
    void readCommand();
    void flushSyncPort();

private:
    QSerialPort *odinPort;
    QSerialPort *syncPort;
    QList<QSerialPortInfo> portInfo;
    QTimer commandTimer;
    QTimer *serialTimer;
    QByteArray outgoingCommand;
    QByteArray incomingCommand;
    QString connectedPortName;
    QMediaPlayer *player;
    int commandCount = 0;
    bool odinSerialConnected = false;
    bool timeToRead = false;
    int readDelay = 3000;
};

#endif // SERIALODIN_H

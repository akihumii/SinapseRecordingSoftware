#ifndef SERIALSHUHAO_H
#define SERIALSHUHAO_H

#include "../common/qtincludes.h"
#include <QObject>

class SerialShuHao: public QObject{
    Q_OBJECT
public:
    SerialShuHao();
    void connectShuHao();
    void closeShuHaoSerial();
    bool isShuHaoSerialConnected();
    void writeCommand(QByteArray command);
    QString getConnectedPort();
    void sendStop();

signals:
    void shuHaoDisconnected();

private slots:
    void checkConnectivity();

private:
    QSerialPort *shuHaoPort;
    QList<QSerialPortInfo> portInfo;
    QByteArray outgoingCommand;
    QString connectedPortName;

    bool shuHaoSerialConnected = false;
};

#endif // SERIALSHUHAO_H

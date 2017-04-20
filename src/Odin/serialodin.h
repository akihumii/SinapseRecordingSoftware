#ifndef SERIALODIN_H
#define SERIALODIN_H

#include "../common/qtincludes.h"
#include <QObject>

class SerialOdin: public QObject{
    Q_OBJECT
public:
    SerialOdin(QObject *parent);
    void connectOdin();
    void initOdin();
    void writeCommand(QByteArray command);

private slots:
    void sendCommand();
private:
    QSerialPort *odinPort;
    QList<QSerialPortInfo> portInfo;
    QElapsedTimer timer;
    QTimer commandTimer;
    QByteArray outgoingCommand;
    int commandCount = 0;
};

#endif // SERIALODIN_H

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

private:
    QSerialPort *odinPort;
    QList<QSerialPortInfo> portInfo;
    QElapsedTimer *timer;
};

#endif // SERIALODIN_H

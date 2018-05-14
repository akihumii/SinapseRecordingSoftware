#ifndef SOCKETSYLPH_H
#define SOCKETSYLPH_H

#include "../common/qtincludes.h"
#include "../common/socketabstract.h"
#include "dataprocessor.h"

namespace SylphX {

class SocketSylph : public SocketAbstract {
    Q_OBJECT
public:
    SocketSylph(DataProcessor *dataProcessor_);
    bool wifiEnabled = true;
    void discardData();
    void closeESP();
    int getRate();

public slots:
    void appendSync();

private:
    QTcpSocket *socketSylph;
    DataProcessor *dataProcessor;
    QTimer *timer;

    qint64 packetSize = 25;
    qint64 maxSize = packetSize*60;
    int bytesRead = 0;
    double rate = 0.0;

    bool checked = false;
    int initCount = 0;

private slots:
    void ReadCommand();
    void updateRate();
};

}

#endif // SOCKETSYLPH_H

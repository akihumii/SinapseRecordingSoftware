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

public slots:
    void appendSync();

private:
    QTcpSocket *socketSylph;
    DataProcessor *dataProcessor;

    qint64 packetSize = 11;
    qint64 maxSize = packetSize*16;

    bool checked = false;
    int initCount = 0;

private slots:
    void ReadCommand();
};

}

#endif // SOCKETSYLPH_H

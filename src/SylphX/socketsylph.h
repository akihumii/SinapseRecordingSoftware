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
    void closeESP();
    int getRate();
    void setChecked(bool flag);

public slots:
    void appendSync();

private:
    QTcpSocket *socketSylph;
    DataProcessor *dataProcessor;
    QTimer *timer;

    qint64 packetSize = 25;
    qint64 maxSize = packetSize*370;
    int bytesRead = 0;
    double rate = 0.0;

    bool checked = false;

private slots:
    void ReadCommand();
    void updateRate();
};

}

#endif // SOCKETSYLPH_H

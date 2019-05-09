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
    void writeCommand(QByteArray command);

private:
    QTcpSocket *socketSylph;
    DataProcessor *dataProcessor;
//    QMediaPlayer *player;

    qint64 packetSize = 25;
    qint64 maxSize = packetSize*65;

private slots:
    void readData();
    void resyncData();
};

}

#endif // SOCKETSYLPH_H

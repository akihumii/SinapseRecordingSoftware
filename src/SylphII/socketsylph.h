#ifndef SOCKETSYLPH_H
#define SOCKETSYLPH_H

#include "../common/qtincludes.h"
#include "../common/socketabstract.h"
#include "dataprocessor.h"

class SocketSylph : public SocketAbstract {
    Q_OBJECT
public:
    SocketSylph(DataProcessor *dataProcessor_);
    bool wifiEnabled = true;
    void discardData();

private:
    QTcpSocket *socketSylph;
    DataProcessor *dataProcessor;

    qint64 maxSize = 25200;

    bool checked = false;
    int initCount = 0;

private slots:
    void ReadCommand();
};

#endif // SOCKETSYLPH_H

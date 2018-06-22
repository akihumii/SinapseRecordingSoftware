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
    bool getStreamConnected(int channel);
    void streamData(int channel, QVector<double> rawData);
    void disableStream(int channel);

public slots:
    void appendSync();

private:
    QTcpSocket *socketSylph;
    QTcpServer *serverMatlab[10];
    quint16 serverPort[10] = { 1340,
                               1341,
                               1342,
                               1343,
                               1344,
                               1345,
                               1346,
                               1347,
                               1348,
                               1349};
    QTcpSocket *socketMatlab[10];
    bool streamConnected[10] = { false,
                                  false,
                                  false,
                                  false,
                                  false,
                                  false,
                                  false,
                                  false,
                                  false,
                                  false};
    QSignalMapper *connectionMapper;
    DataProcessor *dataProcessor;
    QTimer *timer;

    qint64 packetSize = 25;
    qint64 maxSize = packetSize*50;
    int bytesRead = 0;
    double rate = 0.0;

    bool checked = false;
    int initCount = 0;

private slots:
    void ReadCommand();
    void updateRate();
    void on_newConnection(int connected);
};

}

#endif // SOCKETSYLPH_H

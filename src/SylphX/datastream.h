#ifndef DATASTREAM_H
#define DATASTREAM_H

#include "../common/qtincludes.h"

namespace SylphX {

class DataStream : public QObject
{
    Q_OBJECT
public:
    DataStream(QObject *parent);
    bool getStreamConnected(int channel);
    void streamData(int channel);
    void disableStream(int channel);
    void appendData(int channel, double data);

private:
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
    QVector<double> ChannelData[10];
    QDataStream *out[10];

    void clearChannelData(int ChannelIndex);
private slots:
    void on_newConnection(int connected);
};

}

#endif // DATASTREAM_H

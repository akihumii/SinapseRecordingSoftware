#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include "../common/qtincludes.h"
#include "../common/data.h"
#include "../common/signalaudio.h"
#include "time.h"
#include "../Odin/socketodin.h"
#include "../Odin/odinwindow.h"
#include "../common/datastream.h"

#define NUM_CHANNELS 10
#define NUM_BYTES_PER_CHANNEL 2
#define NUM_BYTES_COUNTER 2
#define NUM_BYTES_SYNC 1
#define NUM_BYTES_FRAME 2

#define END_OF_LINE 2779058

class DataProcessor :  public Data, public SignalAudio
{
    Q_OBJECT
public:
    DataProcessor(DataStream *dataStream_);
    DataStream *dataStream;
    SignalAudio *signalAudio;

    int parseFrameMarkers(QByteArray rawData);
    int parseFrameMarkersWithChecks(QByteArray rawData);
    bool checkNextFrameMarker(QByteArray data, int mark);
    int getResyncCounter();
    void sortADCData(QByteArray adcData);
    QVector<quint8> ADC_Data;

signals:
    void dataLost();

public slots:
    void appendDynoData(double data);
private:
    int findfirstFrameMarkers(QByteArray rawData);
    int findlastFrameMarkers(QByteArray rawData);

    qint64 packetSize = NUM_CHANNELS*NUM_BYTES_PER_CHANNEL + NUM_BYTES_COUNTER + NUM_BYTES_SYNC + NUM_BYTES_FRAME;
    bool thresholdEnable = true;

    int index = 0;
    int dyno_index = 0;
    int resync_count = 0;
    int sync_index = 0;
    double dyno_data = 0.0;
    QVector<double> dyno_store;
    bool dyno_start = false;
    bool forceSensorFlag = true;
    int biasFullWord = 0;
    int biasMultiplier= 0;
//signals:
//    void channelACrossed();
//    void channelBCrossed();
};

#endif // DATAPROCESSOR_KA_H

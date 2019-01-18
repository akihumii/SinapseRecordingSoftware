#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include "../common/qtincludes.h"
#include "../common/data.h"
#include "../common/signalaudio.h"
#include "time.h"
#include "../Odin/socketodin.h"
#include "../Odin/odinwindow.h"
#include "datastream.h"

#define NUM_CHANNELS 10
#define NUM_BYTES_PER_CHANNEL 2
#define NUM_BYTES_COUNTER 2
#define NUM_BYTES_SYNC 1
#define NUM_BYTES_FRAME 2

namespace SylphX {

#define END_OF_LINE 2779058

class DataProcessor : public Data
{
    Q_OBJECT
public:
    DataProcessor(DataStream *dataStream_);
    DataStream *dataStream;
//    SignalAudio *signalAudio;

    int parseFrameMarkers(QByteArray rawData);
    int parseFrameMarkersWithChecks(QByteArray rawData);
    bool checkNextFrameMarker(QByteArray data, int mark);

public slots:

private:
    int findfirstFrameMarkers(QByteArray rawData);
    int findlastFrameMarkers(QByteArray rawData);

    qint64 packetSize = NUM_CHANNELS*NUM_BYTES_PER_CHANNEL + NUM_BYTES_COUNTER + NUM_BYTES_SYNC + NUM_BYTES_FRAME;
    bool thresholdEnable = true;

    int index = 0;
//signals:
//    void channelACrossed();
//    void channelBCrossed();
};

}

#endif // DATAPROCESSOR_KA_H

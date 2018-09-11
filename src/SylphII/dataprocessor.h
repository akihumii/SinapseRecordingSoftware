#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include "../common/qtincludes.h"
#include "../common/data.h"
#include "../common/signalaudio.h"
#include "time.h"

#define NUM_CHANNELS 2
#define NUM_BYTES_PER_CHANNEL 2
#define NUM_BYTES_COUNTER 2
#define NUM_BYTES_SYNC 0
#define NUM_BYTES_FRAME 1

#define END_OF_LINE 2779058

class DataProcessor : public SignalAudio, public Data
{
public:
    DataProcessor(float samplingRate_);
    Data *data;
    SignalAudio *signalAudio;

    int parseFrameMarkers(QByteArray rawData);
    int parseFrameMarkersWithChecks(QByteArray rawData);
    bool checkNextFrameMarker(QByteArray data, int mark);
    void sortADCData(QByteArray adcData);
    void setADCRecordEnabled(bool enableFlag);
    int findfirstFrameMarkers(QByteArray rawData);
    int findlastFrameMarkers(QByteArray rawData);
    bool isADCRecordEnabled();
    qint16 fullWord_rawData;
    QVector<quint8> ADC_Data;
    void setScale(int value);
    int getScale();
    void setSmartDataProcessor(bool flag);
    bool isSmart();

private:
    QFile *File;
    QTextStream *out;

    QString fileName;
    bool ADCEnabled = false;
    bool ADCRecordEnabled = false;
    bool smartDataProcessor = false;

    qint64 packetSize = NUM_CHANNELS*NUM_BYTES_PER_CHANNEL + NUM_BYTES_COUNTER + NUM_BYTES_SYNC + NUM_BYTES_FRAME;
    int firstFrameMarker;
    quint8 currentFrameMarker;
    int currentFrameMarkerIndex;
    int lastFrameMarker;
    QByteArray leftOverData;
    float samplingRate;
    float period;
    int index = 0;
    int multiplier = 1;
};

#endif // DATAPROCESSOR_KA_H

#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include "../common/qtincludes.h"
#include "../common/data.h"
#include "../common/signalaudio.h"
#include "time.h"
#include "../Odin/socketodin.h"

#define NUM_CHANNELS 6
#define NUM_BYTES_PER_CHANNEL 2
#define NUM_BYTES_COUNTER 2
#define NUM_BYTES_SYNC 1
#define TOTAL_BYTES_PER_PACKET (NUM_CHANNELS * NUM_BYTES_PER_CHANNEL + NUM_BYTES_COUNTER + NUM_BYTES_SYNC)

namespace SylphX {

#define END_OF_LINE 2779058

class DataProcessor : public SignalAudio, public Data
{
    Q_OBJECT
public:
    DataProcessor(float samplingRate_, QProcess *process_);
    Data *data;
    SignalAudio *signalAudio;

    void parseFrameMarkers(QByteArray rawData);
    void parseFrameMarkersSerial(QByteArray rawData);
    void parseFrameMarkersWithChecks(QByteArray rawData);
    bool checkNextFrameMarker(QByteArray data, int currentIndex);
    bool checkNextFrameMarkerSerial(QByteArray data, int currentIndex);
    void sortADCData(QByteArray adcData);
    void setADCRecordEnabled(bool enableFlag);
    bool isADCRecordEnabled();
    int findfirstFrameMarkers(QByteArray rawData);
    int findlastFrameMarkers(QByteArray rawData);
    void setSmartDataProcessor(bool flag);
    bool isSmart();
    qint16 fullWord_rawData;
    QVector<quint8> ADC_Data;
    int firstFrameMarker;
    quint8 currentFrameMarker;
    int currentFrameMarkerIndex;
    int lastFrameMarker;
    QByteArray leftOverData;

private:
    QFile *File;
    QTextStream *out;
    QProcess *process;

    QString fileName;
    QString directory = QDir::homePath() + "/Desktop/";
    bool ADCEnabled = false;
    bool ADCRecordEnabled = false;
    bool smartDataProcessor = false;

    float samplingRate;
    float period;
    int syncPulse = 0;
};

}

#endif // DATAPROCESSOR_KA_H

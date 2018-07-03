#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include "../common/qtincludes.h"
#include "../common/data.h"
#include "../common/signalaudio.h"
#include "time.h"
#include "../Odin/socketodin.h"
#include "../Odin/odinwindow.h"

#define NUM_CHANNELS 10
#define NUM_BYTES_PER_CHANNEL 2
#define NUM_BYTES_COUNTER 2
#define NUM_BYTES_SYNC 1
#define NUM_BYTES_FRAME 2

namespace SylphX {

#define END_OF_LINE 2779058

class DataProcessor : public SignalAudio, public Data
{
    Q_OBJECT
public:
    DataProcessor(float samplingRate_, QProcess *process_);
    Data *data;
    SignalAudio *signalAudio;

    int parseFrameMarkers(QByteArray rawData);
    int parseFrameMarkersWithChecks(QByteArray rawData);
    bool checkNextFrameMarker(QByteArray data, int mark);
    void sortADCData(QByteArray adcData);
    void setADCRecordEnabled(bool enableFlag);
    bool isADCRecordEnabled();
    int findfirstFrameMarkers(QByteArray rawData);
    int findlastFrameMarkers(QByteArray rawData);
    void setSmartDataProcessor(bool flag);
    void setScale(int value);
    int getScale();
    bool isSmart();
    quint16 fullWord_rawData;
    QVector<quint8> ADC_Data;
    int firstFrameMarker;
    quint8 currentFrameMarker;
    int currentFrameMarkerIndex;
    int lastFrameMarker;
    QByteArray leftOverData;
    int getDebounce();

public slots:
    void setDebounce(int value);
    void setUpperThreshold(double value);
    void setLowerThreshold(double value);
    void setLastSentBytes(char *bytes);
    void setLastSentAmplitudes(double *amplitudes);

private:
    QFile *File;
    QTextStream *out;
    QProcess *process;

    QString fileName;
    QString directory = QDir::homePath() + "/Desktop/";
    bool ADCEnabled = false;
    bool ADCRecordEnabled = false;
    bool smartDataProcessor = true;

    qint64 packetSize = NUM_CHANNELS*NUM_BYTES_PER_CHANNEL + NUM_BYTES_COUNTER + NUM_BYTES_SYNC + NUM_BYTES_FRAME;
    float samplingRate;
    float period;
    int syncPulse = 0;
    double upperThreshold = 2.0;
    double lowerThreshold = 0.0;
    int debounce = 40;
    bool thresholdEnable = true;

    char lastSentByte[2] = {0, 0};
    double lastSentAmplitudes[4] = {0.0, 0.0, 0.0, 0.0};
signals:
    void upperThresholdCrossed();
    void lowerThresholdCrossed();
    int index = 0;
    int multiplier = 1;
};

}

#endif // DATAPROCESSOR_KA_H

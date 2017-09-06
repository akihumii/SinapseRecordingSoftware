#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include "../common/qtincludes.h"
#include "../common/data.h"
#include "../common/signalaudio.h"
#include "time.h"

#define END_OF_LINE 2779058

class DataProcessor : public SignalAudio, public Data
{
public:
    DataProcessor(float samplingRate_);
    Data *data;
    SignalAudio *signalAudio;

    void parseFrameMarkers(QByteArray rawData);
    bool checkNextFrameMarker(QByteArray data, int currentIndex);
    void sortADCData(QByteArray adcData);
    void setADCRecordEnabled(bool enableFlag);
    bool isADCRecordEnabled();
    qint16 fullWord_rawData;
    QVector<quint8> ADC_Data;

private:
    QFile *File;
    QTextStream *out;

    QString fileName;
    bool ADCEnabled = false;
    bool ADCRecordEnabled = false;

    float samplingRate;
    float period;

    float classifierK[2] = {0.214856, 299.9201};
    float classifierL[2] = {0.850121, 120.5125};
};

#endif // DATAPROCESSOR_KA_H

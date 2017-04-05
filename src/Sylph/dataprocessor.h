#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include "../common/qtincludes.h"
#include "../common/data.h"
#include "signalaudio.h"

#define END_OF_LINE 2779058

typedef enum FrameMarkers{
    FM_A = 0B00011010,
    FM_5 = 0B00010101,
    FM_0 = 0B00010000,
    FM_F = 0B00011111,
    FM_F0 = 0B11110000,
    FM_5A = 0B01011010
//Order of FrameMarker should be A5 0F for 10 bit (Since bytes are combined with MSB|LSB)
//Order of FrameMarker should be 5AF0 for 8 bit
} FrameMarkers;

class DataProcessor : public SignalAudio, public Data
{
public:
    DataProcessor();

    void setBitMode(bool BitMode);

    Data *data;
    SignalAudio *signalAudio;
    void parseFrameMarkers(QByteArray rawData);
    bool checkNextFrameMarker(QByteArray data, int currentIndex);
    int findfirstFrameMarkers(QByteArray rawData);
    int findlastFrameMarkers(QByteArray rawData);
    void sortADCData(QByteArray adcData);
    int firstFrameMarker;
    quint8 currentFrameMarker;
    int currentFrameMarkerIndex;
    qint16 fullWord_rawData;
    int lastFrameMarker;
    QByteArray leftOverData;
    QVector<quint8> ADC_Data;

private:
    QTextStream *out;
    bool is8BitMode;
    bool ADCEnabled = false;

    int prevleftOverByteCount = 0;

    double SamplingRate;
};

#endif // DATAPROCESSOR_KA_H

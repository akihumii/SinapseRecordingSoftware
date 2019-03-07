#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include "../common/qtincludes.h"
#include "../common/data.h"
#include "channel.h"
#include "../common/datastream.h"

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

typedef enum BITMODE{
    WORDLENGTH_8 = 0B00110100,
    WORDLENGTH_10 = 0B10110100
} BITMODE;

class DataProcessor : public Data
{
public:
    DataProcessor(Channel *NeutrinoChannel_, DataStream *dataStream_);
    DataStream *dataStream;

    void setBitMode(BITMODE BitMode);
    BITMODE getBitMode();

    QVector<quint16> ParseFrameMarkers10bits(QByteArray data_store);
    QVector<quint16> ParseFrameMarkers8bits(QByteArray data_store);
    QVector<double> getChannelData(int ChannelIndex);
    void MultiplexChannelData(QVector<quint16> Plot_Y_AllDataPoint);
    double signalReconstruction(char input);
    double signalReconstruction(char inputMSB, char inputLSB);
    void setInputReferred(bool flag);
    bool getInputReferred();
    void setGain(bool S1G0, bool S2GAIN1, bool S2GAIN0);

private:
    QTextStream *out;
    Data *data;
    Channel *NeutrinoChannel;

    int firstFrameMarker;
    quint8 currentFrameMarker;
    int currentFrameMarkerIndex;
    qint16 fullWord_rawData;
    int lastFrameMarker;
    int max = 0;
    double temp;
    uint32_t temp_int;
    QByteArray leftOverData;

    QVector<quint8> ADC_Data;

    BITMODE bitMode;
    bool isInputReferred = false;

    int first_10bitFrameMarker(QByteArray data);
    int last_10bitFrameMarker(QByteArray data);

    int first_8bitFrameMarker(QByteArray data);
    int last_8bitFrameMarker(QByteArray data);

    int prevleftOverByteCount = 0;

    double samplingRate;
    double gain;
};

#endif // DATAPROCESSOR_KA_H

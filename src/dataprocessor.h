#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include "qtincludes.h"
#include "data.h"
#include "channel.h"
#include "signalaudio.h"

#define FrameMarker_Begin10Bit 698
#define FrameMarker_End10Bit 1008

#define FrameMarker10Bit_A 0B00011010
#define FrameMarker10Bit_5 0B00010101
#define FrameMarker10Bit_0 0B00010000
#define FrameMarker10Bit_F 0B00011111

#define FrameMarker8Bit_F0 0B11110000
#define FrameMarker8Bit_5A 0B01011010

#define END_OF_LINE 2779058

//Order of FrameMarker should be A5 0F for 10 bit (Since bytes are combined with MSB|LSB)
//Order of FrameMarker should be 5AF0 for 8 bit

#define BitMode_8 0B00110100
#define BitMode_10 0B10110100


class DataProcessor : public SignalAudio, public Data{
public:
    DataProcessor(Channel *NeutrinoChannel_);
    DataProcessor();

    void setBitMode(bool BitMode);

    QVector<quint16> ParseFrameMarkers10bits(QByteArray data_store);
    QVector<quint16> ParseFrameMarkers8bits(QByteArray data_store);
    QVector<double> getChannelData(int ChannelIndex);
    void MultiplexChannelData(QVector<quint16> Plot_Y_AllDataPoint);

    Data *data;
    SignalAudio* signalAudio;
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
    QFile *File;
    QTextStream *out;
    Channel *NeutrinoChannel;
    bool is8BitMode;

    int first_10bitFrameMarker(QByteArray data);
    int last_10bitFrameMarker(QByteArray data);

    int first_8bitFrameMarker(QByteArray data);
    int last_8bitFrameMarker(QByteArray data);

    int prevleftOverByteCount = 0;

    double SamplingRate;
};

#endif // DATAPROCESSOR_KA_H

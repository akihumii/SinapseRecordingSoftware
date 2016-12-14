#ifndef DATAPROCESSOR_MA_H
#define DATAPROCESSOR_MA_H

#include "qtincludes.h"
#include "data.h"

#define END_OF_LINE 2779058

class DataProcessor_MA : public Data
{
public:
    DataProcessor_MA();
    Data *data;
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
};

#endif // DATAPROCESSOR_MA_H

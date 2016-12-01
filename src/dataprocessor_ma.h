#ifndef DATAPROCESSOR_MA_H
#define DATAPROCESSOR_MA_H

#include "qtincludes.h"
#include "data.h"

#define END_OF_LINE 2779058

class DataProcessor_MA : public Data
{
public:
    DataProcessor_MA();
    void parseFrameMarkers(QByteArray rawData);
//    QVector<double> filterData(QVector<double> unfilteredData);
private:
    int findfirstFrameMarkers(QByteArray rawData);
    int findlastFrameMarkers(QByteArray rawData);

    int firstFrameMarker;
    quint16 fullWord_rawData;
    int lastFrameMarker;
    QByteArray leftOverData;
};

#endif // DATAPROCESSOR_MA_H

#ifndef DATA_H
#define DATA_H

#include "qtincludes.h"

#define END_OF_LINE 2779058

enum TimeFrames{
    TimeFrames10ms = 10,
    TimeFrames20ms = 20,
    TimeFrames50ms = 50,
    TimeFrames100ms = 100,
    TimeFrames200ms = 200,
    TimeFrames500ms = 500,
    TimeFrames1000ms = 1000,
    TimeFrames2000ms = 2000,
    TimeFrames5000ms = 5000
};

class Data
{
public:
    Data();
    QVector<double> retrieveData(int ChannelIndex);
    QVector<double> retrieveXAxis();
    QVector<quint16> sortData(QByteArray data_store);
    void removeXAxis();
    bool isEmpty(int ChannelIndex);
    void clearChannelData(int ChannelIndex);
    void clearallChannelData();
    void setPlotEnabled(bool enableFlag);
    bool isPlotEnabled();
    void setRecordEnabled(bool enableFlag);
    bool isRecordEnabled();
    void setFileName(QString fileName);
    QString getFileName();
    void setNumDataPoints(int timeFrames);
    int getNumDataPoints();
    double getSamplingRate();

protected:
    QVector<double> X_axis;
    QVector<double> ChannelData[3];

    quint64 total_data_count = 0;
    bool RecordEnabled = false;
    void RecordData(int data);

private:
    QFile *File;
    QTextStream *out;

    bool PlotEnabled = false;
    int prevleftOverByteCount = 0;

    int numDataPoints = 2082;

    double SamplingRate;
};

#endif // DATA_H

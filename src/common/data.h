#ifndef DATA_H
#define DATA_H

#include "qtincludes.h"
#include "filter.h"

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

class Data : public Filter
{
public:
    Data();
    ~Data();
    Filter* filter;
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
    void setFileName(QString filename);
    QString getFileName();
    void setDirectory(QString dir);
    QString getDirectory();
    void setNumDataPoints(int timeFrames, double sampleFreq);
    void setHeader(QString header);
    int getNumDataPoints();
    double getSamplingRate();
protected:
    QVector<double> X_axis;
    QVector<double> ChannelData[12];

    quint8 headerSetting[14];
    QString headerSettingString;

    quint64 total_data_count = 0;
    bool RecordEnabled = false;
    void RecordData(int data);
    void RecordData(double data);
    void recordHeader();
    QString directory = QDir::homePath() + "/Desktop/";

private:
    QFile *File;
    QTextStream *out;

    QString fileName;


    bool PlotEnabled = true;
    int prevleftOverByteCount = 0;

    int numDataPoints = 2082;

    double SamplingRate;
};

#endif // DATA_H

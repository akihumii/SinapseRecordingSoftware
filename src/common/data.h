#ifndef DATA_H
#define DATA_H

#include "qtincludes.h"
#include "filter.h"

#define END_OF_LINE 2779058
#define NUM_CHANNEL 14

enum TimeFrames{
    TimeFrames10ms,
    TimeFrames20ms,
    TimeFrames50ms,
    TimeFrames100ms,
    TimeFrames200ms,
    TimeFrames500ms,
    TimeFrames1000ms,
    TimeFrames2000ms,
    TimeFrames5000ms
};

class Data : public QObject, public Filter
{
    Q_OBJECT
public:
    Data();
    ~Data();
    QVector<double> retrieveData(int ChannelIndex);
    QVector<double> retrieveXAxis();
    QVector<double> retrieveDyno_XAxis();
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
    int getDebounce();
    void setSmartDataProcessor(bool flag);
    void setScale(int value);
    int getScale();
    bool isSmart();
public slots:
    void setDebounce(int value);
    void setUpperThreshold(double value);
    void setLowerThreshold(double value);
    void setLastSentBytes(char *bytes);
    void setLastSentAmplitudes(double *amplitudes);
    void sendHighpassFilter(QVector<double> *value);
    void sendLowpassFilter(QVector<double> *value);
    void sendNotchFilter(QVector<double> *value);

protected:
    QVector<double> X_axis;
    QVector<double> Dyno_X_axis;
    QVector<double> ChannelData[NUM_CHANNEL];

    quint8 headerSetting[14];
    QString headerSettingString;

    quint64 total_data_count = 0;
    bool RecordEnabled = false;
    void RecordData(int data);
    void RecordData(uint32_t data);
    void RecordData(uint16_t data);
    void RecordData(uint8_t data);
    void RecordData(double data);
    void recordCommand();
    void recordHeader();
    QString directory = QDir::homePath() + "/Desktop/";

    char lastSentByte[2] = {0, 0};
    double lastSentAmplitudes[4] = {0.0, 0.0, 0.0, 0.0};
    double upperThreshold = 10.0;
    double lowerThreshold = 10.0;
    int debounce = 1000;
    int multiplier = 1;
    qint16 fullWord_rawData;
    int firstFrameMarker;
    int lastFrameMarker;
    QByteArray leftOverData;

private:
    QFile *File;
    QTextStream *out;
    QString fileName;

    bool PlotEnabled = true;
    int prevleftOverByteCount = 0;

    int numDataPoints = 2082;
    int dynoNumPoints = 300;
    double SamplingRate;
    bool smartDataProcessor = true;
};

#endif // DATA_H

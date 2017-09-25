#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include "../common/qtincludes.h"
#include "../common/data.h"
#include "../common/signalaudio.h"
#include "time.h"

#define END_OF_LINE 2779058

class DataProcessor : public SignalAudio, public Data
{
    Q_OBJECT
public:
    DataProcessor(float samplingRate_);
    Data *data;
    SignalAudio *signalAudio;

    void parseFrameMarkers(QByteArray rawData);
    bool checkNextFrameMarker(QByteArray data, int currentIndex);
    void sortADCData(QByteArray adcData);
    void setADCRecordEnabled(bool enableFlag);
    bool isADCRecordEnabled();
    void setClassifierK(float newValue);
    float getClassifierK();
    void setClassifierL(float newValue);
    float getClassifierL();
    void setClassifierWindowLength(float length);
    float getClassifierWindowLength();
    void setClassifierThreshold(float threshold);
    float getClassifierThreshold();
    void setClassifierChannel(int channel);
    int getClassifierChannel();
    void setClassifierEnabled(bool flag);
    bool getClassifierEnabled();
    void resizeArray();
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

    float classifierWindowLength = 0.3;            // in seconds
    float classifierThreshold = 0.0005;
    float classifierK = -0.7325;
    float classifierL = 1380.4;
    bool startSavingData = false;
    int numSavedData = 0;
    QVector<double> savedData;
    int classifierChannel = 1;
    bool classifierEnabled = false;
    int test = 0;

    float computeFeature(int channel);
    void classifyFeature(float x);

signals:
    void groupIsignal();
    void groupJsignal();
};

#endif // DATAPROCESSOR_KA_H

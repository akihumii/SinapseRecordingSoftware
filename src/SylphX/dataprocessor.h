#ifndef DATAPROCESSOR_H
#define DATAPROCESSOR_H

#include "../common/qtincludes.h"
#include "../common/data.h"
#include "../common/signalaudio.h"
#include "time.h"
#include "../Odin/socketodin.h"

namespace SylphX {

#define END_OF_LINE 2779058

class DataProcessor : public SignalAudio, public Data
{
    Q_OBJECT
public:
    DataProcessor(float samplingRate_, QProcess *process_);
    Data *data;
    SignalAudio *signalAudio;

    void parseFrameMarkers(QByteArray rawData);
    void parseFrameMarkersWithChecks(QByteArray rawData);
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
    int findfirstFrameMarkers(QByteArray rawData);
    int findlastFrameMarkers(QByteArray rawData);
    void setSmartDataProcessor(bool flag);
    bool isSmart();
    qint16 fullWord_rawData;
    QVector<quint8> ADC_Data;
    int firstFrameMarker;
    quint8 currentFrameMarker;
    int currentFrameMarkerIndex;
    int lastFrameMarker;
    QByteArray leftOverData;

private:
    QFile *File;
    QTextStream *out;
    QProcess *process;

    QString fileName;
    QString directory = QDir::homePath() + "/Desktop/";
    bool ADCEnabled = false;
    bool ADCRecordEnabled = false;
    bool smartDataProcessor = false;

    float samplingRate;
    float period;
    int syncPulse = 0;

    float classifierWindowLength = 0.3;            // in seconds
    float classifierThreshold = 0.0005;
    float classifierK = -0.7325;
    float classifierL = 1380.4;
    bool startSavingData = false;
    int numSavedData = 0;
    QVector<double> savedData;
    int classifierChannel = 1;
    bool classifierEnabled = false;

    float computeFeature(int channel);
    void classifyFeature(float x);
    void pauseClassifier(int msecDuration);

signals:
    void groupIsignal();
    void groupJsignal();
};

}

#endif // DATAPROCESSOR_KA_H

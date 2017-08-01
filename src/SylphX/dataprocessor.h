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
    DataProcessor(float samplingRate_);
    Data *data;
    SignalAudio *signalAudio;

    void parseFrameMarkers(QByteArray rawData);
    bool checkNextFrameMarker(QByteArray data, int currentIndex);
    void sortADCData(QByteArray adcData);
    void setADCRecordEnabled(bool enableFlag);
    bool isADCRecordEnabled();
    qint16 fullWord_rawData;
    QVector<quint8> ADC_Data;

public slots:
    void appendSync();

private:
    QFile *File;
    QTextStream *out;

    QString fileName;
    QString directory = QDir::homePath() + "/Desktop/";
    bool ADCEnabled = false;
    bool ADCRecordEnabled = false;

    float samplingRate;
    float period;
    int syncPulse = 0;
};

}

#endif // DATAPROCESSOR_KA_H

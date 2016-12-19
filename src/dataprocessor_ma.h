#ifndef DATAPROCESSOR_MA_H
#define DATAPROCESSOR_MA_H

#include "qtincludes.h"
#include "data.h"

#define END_OF_LINE 2779058

class DataProcessor_MA : public Data, public QObject{
//    Q_OBJECT
public:
    DataProcessor_MA(QObject *parent);
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
    QVector<double> ADC_Data;

private:
    QAudioOutput* audio; // class member
    QIODevice *audioDevice;
//    QDataStream *out3;
    QFile *file3;
    QByteArray audioBuffer1;
    QByteArray audioBuffer2;
private slots:
    void handleStateChanged(QAudio::State);
    void onNotify();
};

#endif // DATAPROCESSOR_MA_H

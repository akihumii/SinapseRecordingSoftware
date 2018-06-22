#ifndef SIGNALAUDIO_H
#define SIGNALAUDIO_H

#include "qtincludes.h"
#include "filter.h"

class SignalAudio : public QObject {
    Q_OBJECT
public:
    SignalAudio();
    ~SignalAudio();
    void setAudioChannel(int ChannelIndex);
    int getAudioChannel();
    void setSampleRate(int rate);
    int getSampleRate();
    void setAudioSampleSize(int size);
    int getAudioSampleSize();
protected:
    QAudioOutput* audio; // class member
    QIODevice *audioDevice;
    QByteArray audioBuffer[11];
    void appendAudioBuffer(int ChannelIndex, char LSB, char MSB);
    bool playAudio(int ChannelIndex);
private:
    Filter *filter;
    void clearAudioBuffer();
    int audioChannelSelected = 0;
    int sampleRate = 8000;
    int sampleSize = 16;
};

#endif // SIGNALAUDIO_H

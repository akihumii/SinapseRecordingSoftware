#ifndef SIGNALAUDIO_H
#define SIGNALAUDIO_H

#include "qtincludes.h"

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
    void clearAudioBuffer();
    int audioChannelSelected = 0;
    int sampleRate = 44100;
    int sampleSize = 16;
};

#endif // SIGNALAUDIO_H

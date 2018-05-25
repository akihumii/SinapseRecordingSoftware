#ifndef COMMANDODIN_H
#define COMMANDODIN_H

#include "serialodin.h"
#include "socketodin.h"
#include "../common/qtincludes.h"

namespace Odin {

typedef enum ADDRESS{
    AMPLITUDE_CHN1 = 0x10,
    AMPLITUDE_CHN2 = 0x11,
    AMPLITUDE_CHN3 = 0x12,
    AMPLITUDE_CHN4 = 0x13,
    PULSEDURATION_CHN1 = 0x30,
    PULSEDURATION_CHN2 = 0x31,
    PULSEDURATION_CHN3 = 0x32,
    PULSEDURATION_CHN4 = 0x33,
    FREQUENCY = 0x20,
    CHANNEL_ENABLE = 0x41,
    STEP_INCREASE = 0x51,
    STEP_DECREASE = 0x52,
    THRESHOLD_ENABLE = 0x61
} ADDRESS;

class CommandOdin : public QObject
{
    Q_OBJECT
public:
    CommandOdin(SerialOdin *serialOdin_, SocketOdin *socketOdin_);
    void sendCommand();
    void setAmplitude(int channel, double value);
    void setPulseDuration(int channel, int duration);
    void setFrequency(int value);
    void setChannelEnabled(int channel, bool flag);
    double getAmplitude(int channel);
    unsigned char getAmplitudeByte(int index);
    char getPulseDurationByte(int channel);
    int getPulseDuration(int channel);
    char getFrequencyByte();
    int getFrequency();
    void sendStart();
    void sendStop();
    void sendAmplitude(int channel);
    void sendPulseDuration(int channel);
    void sendFrequency();
    void sendChannelEnable();
    char getChannelEnabled();
    void initialiseCommand();
    int getNumChannelEnabled();
    QByteArray getlastSentCommand();
    void setStepSize(double step);
    char getStepSize();
    void sendStepSizeIncrease();
    void sendStepSizeDecrease();
    void sendThresholdEnable();
    void setThresholdEnable(char value);
    char getThresholdEnable();
private:
    SerialOdin *serialOdin;
    SocketOdin *socketOdin;

    double amplitude[4] = {0.0, 0.0, 0.0, 0.0};
    bool channelEnabled[4] = {true, true, true, true};
    int pulseDuration[4] = {200, 200, 200, 200};
    int frequency = 50;
    int numChannels = 0;
    char thresholdEnable = 15;

    float a = 0.0048;
    float b = 12.803;
    float c = 6.9185;

    char stepSize = 6;

    QByteArray outgoingCommand;
signals:
//    void commandSent(char *command);
};

}

#endif // COMMANDODIN_H

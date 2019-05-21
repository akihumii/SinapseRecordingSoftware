#ifndef COMMANDODIN_H
#define COMMANDODIN_H

#include "serialodin.h"
#include "socketodin.h"
#include "../common/qtincludes.h"

namespace Odin {

typedef enum ADDRESS{
    AMPLITUDE_CHN1 = 0xF1,
    AMPLITUDE_CHN2 = 0xF2,
    AMPLITUDE_CHN3 = 0xF3,
    AMPLITUDE_CHN4 = 0xF4,
    FREQUENCY = 0xF5,
    PULSEDURATION_CHN1 = 0xF6,
    PULSEDURATION_CHN2 = 0xF7,
    PULSEDURATION_CHN3 = 0xF9,
    PULSEDURATION_CHN4 = 0xFA,
    CHANNEL_ENABLE = 0xFB,
    STEP_INCREASE = 0xFC,
    STEP_DECREASE = 0xFD,
    THRESHOLD_ENABLE = 0xFE,
    STEP_SIZE = 0xC8,
    THRESHOLD_UPPER = 0xC9,
    THRESHOLD_LOWER = 0xCA,
    DEBOUNCE_DELAY = 0XCB
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
    QByteArray getlastRpiCommand();
    void setStepSize(double step);
    void setThresholdUpper(double threshold);
    void setThresholdLower(double threshold);
    void setDebounceDelay(double delay);
    char getStepSize();
    char getStepSizeByte();
    char getThresholdUpper();
    char getThresholdLower();
    int getDebounceDelay();
    char getDebounceDelayByte();
    void sendStepSizeIncrease();
    void sendStepSizeDecrease();
    void sendThresholdEnable();
    void sendThresholdUpper();
    void sendThresholdLower();
    void sendDebounceDelay();
    void sendStepSize();
    void setThresholdEnable(char value);
    char getThresholdEnable();
    unsigned char getCurrentAmplitude();
    void setCurrentAmplitude(unsigned char value);
private:
    SerialOdin *serialOdin;
    SocketOdin *socketOdin;

    double amplitude[4] = {0.0, 0.0, 0.0, 0.0};
    bool channelEnabled[4] = {false, false, false, false};
    int pulseDuration[4] = {200, 200, 200, 200};
    int frequency = 50;
    int numChannels = 0;
    char thresholdEnable = 15;

    float a = 0.0121;
    float b = 12.853;
    float c = 6.6892;

    char stepSize = 1;
    char thresholdUpper = 10;
    char thresholdLower = 10;
    int debounceDelay = 1000;
    unsigned char currentAmplitude = 0;

    QByteArray outgoingCommand;
    QByteArray rpiCommand;
signals:
//    void commandSent(char *command);
};

}

#endif // COMMANDODIN_H

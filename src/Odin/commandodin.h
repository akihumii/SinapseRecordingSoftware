#ifndef COMMANDODIN_H
#define COMMANDODIN_H

#include "serialodin.h"
#include "socketodin.h"
#include "../common/qtincludes.h"

namespace Odin {

typedef enum ADDRESS{
    START_STIMULATION =0xF8,
    STOP_STIMULATION = 0x8F,
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
    THRESHOLD_UPPER = 0xC9,
    THRESHOLD_LOWER = 0xCA,
    DEBOUNCE_DELAY = 0xCB,
    THRESHOLD_CHN1 = 0xCC,
    THRESHOLD_CHN2 = 0xCD,
    THRESHOLD_CHN3 = 0xCE,
    THRESHOLD_CHN4 = 0xCF,
    THRESHOLD_POWER_CHN1 = 0xD0,
    THRESHOLD_POWER_CHN2 = 0xD1,
    THRESHOLD_POWER_CHN3 = 0xD2,
    THRESHOLD_POWER_CHN4 = 0xD3
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
    void setThreshold(int channel, int value);
    void setThresholdPower(int channel, int value);
    void setChannelEnabled(int channel, bool flag);
    double getAmplitude(int channel);
    unsigned char getAmplitudeByte(int index);
    char getPulseDurationByte(int channel);
    int getPulseDuration(int channel);
    char getFrequencyByte();
    int getFrequency();
    int getThreshold(int channel);
    int getThresholdPower(int channel);
    void sendStart();
    void sendStartFlag();
    void sendStop();
    void sendAmplitude(int channel);
    void sendPulseDuration(int channel);
    void sendFrequency();
    void sendThreshold(int channel);
    void sendThresholdPower(int channel);
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
    char getThresholdUpper();
    char getThresholdLower();
    char getDebounceDelay();
    void sendStepSizeIncrease();
    void sendStepSizeDecrease();
    void sendThresholdEnable();
    void sendThresholdUpper();
    void sendThresholdLower();
    void sendDebounceDelay();
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
    int threshold[4] = {1, 1, 1, 1};
    int thresholdPower[4] = {10, 10, 10, 10};
    int numChannels = 0;
    char thresholdEnable = 15;

    float a = 0.0121;
    float b = 12.853;
    float c = 6.6892;

    char stepSize = 12;
    char thresholdUpper = 10;
    char thresholdLower = 10;
    char debounceDelay = 10;
    unsigned char currentAmplitude = 0;

    QByteArray outgoingCommand;
    QByteArray rpiCommand;
signals:
//    void commandSent(char *command);
};

}

#endif // COMMANDODIN_H

#ifndef COMMANDODIN_H
#define COMMANDODIN_H

#include "serialodin.h"
#include "socketodin.h"
#include "../common/qtincludes.h"

namespace Odin {

typedef enum ADDRESS{
    AMPLITUDE_CHN1 = 0x00,
    AMPLITUDE_CHN2 = 0x01,
    AMPLITUDE_CHN3 = 0x02,
    AMPLITUDE_CHN4 = 0x03,
    PULSEDURATION_CHN1 = 0x20,
    PULSEDURATION_CHN2 = 0x21,
    PULSEDURATION_CHN3 = 0x22,
    PULSEDURATION_CHN4 = 0x23,
    FREQUENCY = 0x10
} ADDRESS;

class CommandOdin : public QObject
{
    Q_OBJECT
public:
    CommandOdin(SerialOdin *serialOdin_, SocketOdin *socketOdin_);
    void sendCommand();
    void setAmplitude(int channel, double value);
    void setPulseDuration(int channel, int duration);
    void setFrequency(char value);
    double getAmplitude(int channel);
    unsigned char getAmplitudeByte(int index);
    char getPulseDuration(int channel);
    char getFrequency();
    void sendStart();
    void sendStop();
    void constructCommand();
    QByteArray getlastSentCommand();
private:
    SerialOdin *serialOdin;
    SocketOdin *socketOdin;

    double amplitude[4] = {0.0, 0.0, 0.0, 0.0};
    bool channelEnabled[4] = {false, false, false, false};
    int pulseDuration[4] = {0, 0, 0, 0};
    int frequency = 0;
    int numChannels = 0;

    float a = -0.0045;
    float b = 13.243;
    float c = 8.5461;

    QByteArray outgoingCommand;
};

}

#endif // COMMANDODIN_H

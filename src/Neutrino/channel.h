#ifndef CHANNEL_H
#define CHANNEL_H

#include "../common/qtincludes.h"

typedef enum ChannelState{
    CHANNELSTATE_ON = 0B10011001,
    CHANNELSTATE_OFF = 0B01100110,
} ChannelState;

class Channel
{
public:
    Channel();
    QByteArray getChannelState();
    bool *getChannelState_Bool();
    void setChannelState(int Index, bool state);
    int getNumChannels();
    void setNumChannels(int NumChannels);

private:
    bool SELCHNarray[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int numChannels = 10;
};

#endif // CHANNEL_H

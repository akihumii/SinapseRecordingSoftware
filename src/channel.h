#ifndef CHANNEL_H
#define CHANNEL_H

#include "qtincludes.h"

#define CHANNEL_ON 0B10011001
#define CHANNEL_OFF 0B01100110


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
    int numChannels;
};

#endif // CHANNEL_H

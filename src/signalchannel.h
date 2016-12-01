#ifndef SIGNALCHANNEL_H
#define SIGNALCHANNEL_H

#include "qtincludes.h"

enum SignalType{
    ImplantSignal,
    ADCSignal,
    FrameMarkerSignal
};

class SignalChannel
{
public:
    SignalChannel();
    SignalChannel(const QString &initCustomChannelName, SignalType initSignalType, int initChannelID);
    SignalType signalType;
    QString customChannelName;
    bool enabled;
    int channelID;

private:

};

#endif // SIGNALCHANNEL_H

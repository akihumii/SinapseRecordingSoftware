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
    bool isHighpassEnable;
    bool isLowpassEnable;
    bool isNotchEnable;
    QVector<double> channelData;

private:

};

#endif // SIGNALCHANNEL_H

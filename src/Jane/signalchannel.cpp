#include "signalchannel.h"

SignalChannel::SignalChannel(){
    enabled = true;
    channelID = 0;
}

SignalChannel::SignalChannel(const QString &initCustomChannelName, SignalType initSignalType, int initChannelID){
    customChannelName = initCustomChannelName;
    signalType = initSignalType;
    enabled = true;
    channelID = initChannelID;
}

#include "channel.h"

Channel::Channel()
{

}

QByteArray Channel::getChannelState(){
    QByteArray channelState;
    for(int i=0;i<10;i++){
        if(SELCHNarray[i]){
            channelState.append((const char) Channel_On);
        }
        else{
            channelState.append((const char) Channel_Off);
        }
    }
    return channelState;
}

void Channel::setChannelState(int Index, bool state){
    SELCHNarray[Index] = state;
}

void Channel::setNumChannels(int NumChannels){
    numChannels = NumChannels;
}

int Channel::getNumChannels(){
    return numChannels;
}

bool *Channel::getChannelState_Bool(){
    return SELCHNarray;
}

#include "channel.h"

Channel::Channel(){

}

QByteArray Channel::getChannelState(){
    QByteArray channelState;
    for(int i=0;i<8;i++){
        if(SELCHNarray[i]){
            channelState.append((const char) CHANNELSTATE_ON);
        }
        else{
            channelState.append((const char) CHANNELSTATE_OFF);
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

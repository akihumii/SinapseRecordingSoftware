#include "commandodin.h"

namespace Odin {

CommandOdin::CommandOdin(SerialOdin *serialOdin_, SocketOdin *socketOdin_) {
    serialOdin = serialOdin_;
    socketOdin = socketOdin_;
}

void CommandOdin::sendTestCommand(){
    QByteArray commandByte;
    for(int i = 0; i < 16; i++){
        commandByte.append(testCommand[i]);
    }
    if(serialOdin->isOdinSerialConnected()){
        serialOdin->writeCommand(commandByte);
    }
    else if(socketOdin->isConnected()){
        socketOdin->writeCommand(commandByte);
    }
}

void CommandOdin::constructCommand(){
    outgoingCommand.clear();
    outgoingCommand.append((const char) 0xAA);
    outgoingCommand.append(getMode());
    outgoingCommand.append(getChannelNumber());
    outgoingCommand.append(getPulseMagByte(0));
    outgoingCommand.append(getPulseDuration());
    outgoingCommand.append(getPulseNum());
    outgoingCommand.append(getInterPulseDuration());
    outgoingCommand.append(getMultiChannelSequenceByte());
    outgoingCommand.append((const char) 0x00);
    outgoingCommand.append(getTimeZoneByte1());
    outgoingCommand.append(getTimeZoneByte2());
    outgoingCommand.append(getZoneMask());
    outgoingCommand.append(getPulseMagByte(1));
    outgoingCommand.append(getPulseMagByte(2));
    outgoingCommand.append(getPulseMagByte(3));
    outgoingCommand.append(getPulseMagByte(4));
//    qDebug() << "Constructed command";
}

void CommandOdin::sendCommand(){
    constructCommand();
    if(serialOdin->isOdinSerialConnected()){
//        qDebug() << "Sending via serial";
        serialOdin->writeCommand(outgoingCommand);
    }
    else if(socketOdin->isConnected()){
//        qDebug() << "Sending via socket";
        socketOdin->writeCommand(outgoingCommand);
    }
}

void CommandOdin::sendStart(){
    outgoingCommand.clear();
    outgoingCommand.append((char) 0xF8);
    if(serialOdin->isOdinSerialConnected()){
//        qDebug() << "Sending via serial";
        serialOdin->writeCommand(outgoingCommand);
    }
    else if(socketOdin->isConnected()){
//        qDebug() << "Sending via socket";
        socketOdin->writeCommand(outgoingCommand);
    }
}

void CommandOdin::sendStop(){
    outgoingCommand.clear();
    outgoingCommand.append((const char) 0x8F);
    if(serialOdin->isOdinSerialConnected()){
//        qDebug() << "Sending via serial";
        serialOdin->writeCommand(outgoingCommand);
    }
    else if(socketOdin->isConnected()){
//        qDebug() << "Sending via socket";
        socketOdin->writeCommand(outgoingCommand);
    }
}

QByteArray CommandOdin::getlastSentCommand(){
    return outgoingCommand;
}

void CommandOdin::setMode(int mode){
    switch (mode){
        case 0:{
            OPMode = FLATSINGLE;
            break;
        }
        case 1:{
            OPMode = GRADEDAMPLITUDE;
            break;
        }
        case 2:{
            OPMode = GRADEDFFREQ_HIGHLOW;
            break;
        }
        case 3:{
            OPMode = GRADEDFREQ_LOWHIGH;
            break;
        }
        case 4:{
            OPMode = MULTICHANNEL_INTERLEAVED;
            break;
        }
        case 5:{
            OPMode = MULTICHANNEL_NONINTERLEAVED;
            break;
        }
        default:
            OPMode = FLATSINGLE;
        break;

    }
}

char CommandOdin::getMode(){
    return (char) OPMode;
}

void CommandOdin::setChannelNumber(CHANNELNUMBER num){
    ChannelNum = num;
}

char CommandOdin::getChannelNumber(){
    return (char) ((ChannelNum*16) + 12);
}

void CommandOdin::setPulseMag(int index, double mag){
    PulseMag[index] = mag;
}

unsigned char CommandOdin::getPulseMagByte(int index){
    if(PulseMag[index] == 0.0){
        return 0;
    }
    else{
        unsigned char temp = PulseMag[index]*PulseMag[index]*0.0012 + PulseMag[index]*25.556 - 6.601;
        return temp;
    }
}

void CommandOdin::setPulseDuration(int duration){
    PulseDuration = duration;
}

char CommandOdin::getPulseDuration(){
    return (char) (((2*PulseDuration - 40) / 10) + 1);
}

void CommandOdin::setPulseNum(char num){
    PulseNum = num;
}

char CommandOdin::getPulseNum(){
    return (char) PulseNum;
}

void CommandOdin::setInterPulseDuration(char duration){
    interPulseDuration = duration;
}

char CommandOdin::getInterPulseDuration(){
    return (char) ((((1000000/(quint8)interPulseDuration) - ((double)PulseDuration * 2.0 + 22.0))*0.0045) - 2.8147);
}

void CommandOdin::setMultiChannelSequence(CHANNELNUMBER first, CHANNELNUMBER second, CHANNELNUMBER third, CHANNELNUMBER fourth){
    multiChannelSeq[0] = first;
    multiChannelSeq[1] = second;
    multiChannelSeq[2] = third;
    multiChannelSeq[3] = fourth;
}

int *CommandOdin::getMultiChannelSequence(){
    return (int *) multiChannelSeq;
}

char CommandOdin::getMultiChannelSequenceByte(){
    return (char) (64*multiChannelSeq[0] + 16*multiChannelSeq[1] + 4*multiChannelSeq[2] + multiChannelSeq[3]);
}

void CommandOdin::setTimeZone(int index, TIMEZONE time){
    zone[index] = time;
}

char CommandOdin::getTimeZoneByte1(){
    return (char) (64*zone[0] + 16*zone[1] + 4*zone[2] + zone[3]);
}

char CommandOdin::getTimeZoneByte2(){
    return (char) (64*zone[4] + 16*zone[5] + 4*zone[6]);
}

void CommandOdin::setZoneMask(ZONEMASK mask){
    zoneMask = mask;
}

char CommandOdin::getZoneMask(){
    return (char) zoneMask;
}

}

#include "commandodin.h"

CommandOdin::CommandOdin(SerialOdin *serialOdin_, SocketOdin *socketOdin_){
    serialOdin = serialOdin_;
    socketOdin = socketOdin_;
}

void CommandOdin::sendTestCommand(){
    QByteArray commandByte;
    for(int i = 0; i < 16; i++){
        commandByte.append(testCommand[i]);
    }
    serialOdin->writeCommand(commandByte);
//    socketOdin->writeCommand(commandByte);
}

void CommandOdin::sendCommand(){
    QByteArray outgoingCommand;
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

    serialOdin->writeCommand(outgoingCommand);
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
//    qDebug() << OPMode;
}

char CommandOdin::getMode(){
    return (char) OPMode;
}

void CommandOdin::setChannelNumber(CHANNELNUMBER num){
    ChannelNum = num;
//    qDebug() << ChannelNum;
}

char CommandOdin::getChannelNumber(){
    return (char) ((ChannelNum*16) + 12);
}

void CommandOdin::setPulseMag(int index, double mag){
    PulseMag[index] = mag;
//    qDebug() << PulseMag[index];
}

char CommandOdin::getPulseMagByte(int index){
    return (char) ((quint8)PulseMag[index]*(quint8)PulseMag[index]*0.0012 + (quint8)PulseMag[index]*25.556 - 6.601);
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
//    qDebug() << (quint8) interPulseDuration;
}

char CommandOdin::getInterPulseDuration(){
    qDebug() << (quint8) interPulseDuration;
    qDebug() << (quint8) PulseDuration;
    return (char) (((1000000/(quint8)interPulseDuration) - ((quint8)PulseDuration * 2 + 100) - 1360) / 218);
}

void CommandOdin::setMultiChannelSequence(CHANNELNUMBER first, CHANNELNUMBER second, CHANNELNUMBER third, CHANNELNUMBER fourth){
    multiChannelSeq[0] = first;
    multiChannelSeq[1] = second;
    multiChannelSeq[2] = third;
    multiChannelSeq[3] = fourth;
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

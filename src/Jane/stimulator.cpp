#include "stimulator.h"

Stimulator::Stimulator()
{
    paramValue.resize(16);
    for(int i=0; i<16;i++){
        paramValue[i].resize(6);
        for(int j = 0; j < 6; j++){
            paramValue[i][j] = 0;
        }
    }
}

quint8 Stimulator::getMultiplier(quint8 arr[][2], int channel)
{
    quint8 multiplierBits=0;
    int number = arr[channel][2];
    switch (number){
        case 0:{
            multiplierBits = (quint8)X1;
            break;
        }
        case 1:{
            multiplierBits = (quint8)X2;
            break;
        }
        case 2:{
            multiplierBits = (quint8)X4;
            break;
        }
        case 3:{
            multiplierBits = (quint8)X8;
            break;
        }
        case 4:{
            multiplierBits = (quint8)X16;
            break;
        }
        case 5:{
            multiplierBits = (quint8)X32;
            break;
        }
        case 6:{
            multiplierBits = (quint8)X64;
            break;
        }
        case 7:{
            multiplierBits = (quint8)X128;
            break;
        }
        default:
            break;
    }
    return multiplierBits;
}

bool Stimulator::isStimulatorParamSetLower()
{
    for (int i = 0;i<8;i++){
            if (paramValue[i].isEmpty())
                return false;

    }
    return true;
}

bool Stimulator::isStimulatorParamSetUpper()
{
    for (int i = 8;i<16;i++){
            if (paramValue[i].isEmpty())
                return false;
    }
    return true;
}

QByteArray Stimulator::getSubSequence(int channelNumber)
{
    quint8 subSequence = 0B00000000;
    subSequence |= (subSeqParam[channelNumber] - 1);
    if(subSeqSelect[channelNumber]){
        subSequence = (subSequence << 4) | (int)SUBSEQ_CHOSEN;
    }
    else{
        subSequence = (subSequence << 4) | (int)SUBSEQ_UNCHOSEN;
    }

    QByteArray subSequenceArray;
    subSequenceArray.append((const char)subSequence);
    return subSequenceArray;
}

QByteArray Stimulator::getSubSequenceStart(int channelNumber)
{
    quint8 timeStart = 0B00000000;

    timeStart |= (subSeqStart[channelNumber][1] - 1);
    timeStart = Stimulator::getMultiplier(subSeqStart, channelNumber) | timeStart;

    QByteArray timeStartArray;
    timeStartArray.append((const char)timeStart);
    return timeStartArray;
}

QByteArray Stimulator::getSubSequenceStop(int channelNumber)
{
    quint8 timeStop = 0B00000000;

    timeStop |= (subSeqStop[channelNumber][1] - 1);
    timeStop = Stimulator::getMultiplier(subSeqStop, channelNumber) | timeStop;

    QByteArray timeStopArray;
    timeStopArray.append((const char)timeStop);
    return timeStopArray;
}

QByteArray Stimulator::getParameter()
{
    QByteArray parameter;
    return parameter;
}

QByteArray Stimulator::getTriggerCmd()
{
    QByteArray trigger;
    quint8 cmd = 0B00000000;
    for (int i = 0; i<8; i++){
        cmd += triggerCmd[i]*pow(2,7-i);
    }
    trigger.append(cmd);
    qDebug()<< cmd;
    return trigger;
}

void Stimulator::setSubSeqSelect(int index, bool state) { subSeqSelect[index] = state; }
void *Stimulator::getSubSeqSelectArray() { return subSeqSelect; }

void Stimulator::setSubSeqParam(int channel, int param) { subSeqParam[channel] = param; }
void *Stimulator::getSubSeqParamArray() { return subSeqParam; }

void Stimulator::setTriggerCmd(int index, bool state){ triggerCmd[index] = state; }


void Stimulator::setSubSeqTimeStart(int index, int value) { subSeqStart[index][1] = value; }
void Stimulator::setSubSeqMultipleStart(int index, int multiple) { subSeqStart[index][2] = multiple; }
void Stimulator::setSubSeqTimeStop(int index, int value) { subSeqStop[index][1] = value; }
void Stimulator::setSubSeqMultipleStop(int index, int multiple) { subSeqStop[index][2] = multiple; }
void *Stimulator::getSubSeqStartArray() { return subSeqStart;}
void *Stimulator::getSubSeqStopArray() { return subSeqStop;}

void Stimulator::setStimParam(int set, int paramOfSet, int value) { paramValue[set][paramOfSet] = value; }
QVector<QVector<quint8> > Stimulator::getStimParamArray() { return paramValue; }
quint8 Stimulator::getStimParam(int set, int paramOfSet) { return paramValue[set][paramOfSet];}

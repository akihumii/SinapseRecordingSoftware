#include "command.h"

Command::Command(){

}

QByteArray Command::constructCommand(){
    QByteArray outgoingCommand;
    outgoingCommand.append("STIM ");
    outgoingCommand.append(channel + " ");
    outgoingCommand.append(phaseType + " ");
    outgoingCommand.append(polarity + " ");
    outgoingCommand.append(anodicAmp + " ");
    outgoingCommand.append(anodicDuration + " ");
    outgoingCommand.append(cathodicAmp + " ");
    outgoingCommand.append(cathodicDuration + " ");
    outgoingCommand.append(numPulse + " ");
    outgoingCommand.append(adjDuration + "\r");
    qDebug() << outgoingCommand;
    return outgoingCommand;
}

void Command::setChannel(CHANNEL_NUMBER newChannel){
    channel = newChannel;
}

void Command::setPhaseType(PHASE_TYPE type){
    phaseType = type;
}

void Command::setPolarity(POLARITY newPolarity){
    polarity = newPolarity;
}

void Command::setAnodicAmp(QString amplitude){
    anodicAmp = amplitude;
}

void Command::setAnodicDuration(QString duration){
    anodicDuration = duration;
}

void Command::setCathodicAmp(QString amplitude){
    cathodicAmp = amplitude;
}

void Command::setCathodicDuration(QString duration){
    cathodicDuration = duration;
}

void Command::setNumPulse(QString number){
    numPulse = number;
}

void Command::setAdjDuration(QString duration){
    adjDuration = duration;
}

QString Command::getChannel(){ return channel; }
QString Command::getPhaseType(){ return phaseType; }
QString Command::getPolarity(){ return polarity; }
QString Command::getAnodicAmp(){ return anodicAmp; }
QString Command::getAnodicDuration(){ return anodicDuration; }
QString Command::getCathodicAmp(){ return cathodicAmp; }
QString Command::getCathodicDuration(){ return cathodicDuration; }
QString Command::getNumPulse(){ return numPulse; }
QString Command::getAdjDuration(){ return adjDuration; }

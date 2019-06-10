#include "commandcat.h"

namespace Cat {

CommandCat::CommandCat(){
}

void CommandCat::sendThreshold(int channel){
    rpiCommand.clear();
    switch (channel){
        case 0:{
            rpiCommand.append((const char) THRESHOLD_CHN1);
            break;
        }
        case 1:{
            rpiCommand.append((const char) THRESHOLD_CHN2);
            break;
        }
        case 2:{
            rpiCommand.append((const char) THRESHOLD_CHN3);
            break;
        }
        case 3:{
            rpiCommand.append((const char) THRESHOLD_CHN4);
            break;
        }
    }
    rpiCommand.append((const char) getThreshold(channel));
    qDebug() << "Sent thresholds: " << rpiCommand;
}

void CommandCat::sendThresholdPower(int channel){
    rpiCommand.clear();
    switch (channel){
        case 0:{
            rpiCommand.append((const char) THRESHOLD_POWER_CHN1);
            break;
        }
        case 1:{
            rpiCommand.append((const char) THRESHOLD_POWER_CHN2);
            break;
        }
        case 2:{
            rpiCommand.append((const char) THRESHOLD_POWER_CHN3);
            break;
        }
        case 3:{
            rpiCommand.append((const char) THRESHOLD_POWER_CHN4);
            break;
        }
    }
    rpiCommand.append((const char) getThresholdPower(channel));
    qDebug() << "Sent threshold power: " << rpiCommand ;
}

void CommandCat::sendDecodingWindowSize(){
    rpiCommand.clear();
    rpiCommand.append((const char) DECODING_WINDOW_SIZE);
    rpiCommand.append((const char) getDecodingWindowSize());
    qDebug() << "Send decoding window size: " << rpiCommand;
}

void CommandCat::sendOverlapWindowSize(){
    rpiCommand.clear();
    rpiCommand.append((const char) OVERLAP_WINDOW_SIZE);
    rpiCommand.append((const char) getOverlapWindowSize());
    qDebug() << "Send overlap window size: " << rpiCommand;
}

void CommandCat::sendSamplingFreq(){
    rpiCommand.clear();
    rpiCommand.append((const char) SAMPLING_FREQ);
    rpiCommand.append((const char) getSamplingFreq());
    qDebug() << "Send sampling frequency: " << rpiCommand;
}

void CommandCat::sendExtendStimulation(){
    rpiCommand.clear();
    rpiCommand.append((const char) EXTEND_STIMULATION);
    rpiCommand.append((const char) getExtendStimulation());
    qDebug() << "Send Extend Stimulation: " << rpiCommand;
}

void CommandCat::sendHighpassCutoffFreq(){
    rpiCommand.clear();
    rpiCommand.append((const char) HIGHPASS_CUTOFF_FREQ);
    rpiCommand.append((const char) getHighpassCutoffFreq());
    qDebug() << "Send highpass cutoff frequency: " << rpiCommand;
}

void CommandCat::sendLowpassCutoffFreq(){
    rpiCommand.clear();
    rpiCommand.append((const char) LOWPASS_CUTOFF_FREQ);
    rpiCommand.append((const char) getLowpassCutoffFreq());
    qDebug() << "Send lowpass cutoff frequency: " << rpiCommand;
}

void CommandCat::sendNotchCutoffFreq(){
    rpiCommand.clear();
    rpiCommand.append((const char) NOTCH_CUTOFF_FREQ);
    rpiCommand.append((const char) getNotchCutoffFreq());
    qDebug() << "Send notch cutoff frequency: " << rpiCommand;
}

void CommandCat::setThreshold(int channel, int value){
    threshold[channel] = value;
}

void CommandCat::setThresholdPower(int channel, int value){
    thresholdPower[channel] = value;
}

void CommandCat::setDecodingWindowSize(int value){
    decodingWindowSize = value;
}

void CommandCat::setOverlapWindowSize(int value){
    overlapWindowSize = value;
}

void CommandCat::setSamplingFreq(int value){
    samplingFreq = value;
}

void CommandCat::setExtendStimulation(int value){
    extendStimulation = value;
}

void CommandCat::setHighpassCutoffFreq(int value){
    highpassCutoffFreq = value;
}

void CommandCat::setLowpassCutoffFreq(int value){
    lowpassCutoffFreq = value;
}

void CommandCat::setNotchCutoffFreq(int value){
    notchCutoffFreq = value;
}

int CommandCat::getDecodingWindowSize(){
    return decodingWindowSize;
}

int CommandCat::getOverlapWindowSize(){
    return overlapWindowSize;
}

int CommandCat::getSamplingFreq(){
    return samplingFreq;
}

int CommandCat::getExtendStimulation(){
    return extendStimulation;
}

int CommandCat::getHighpassCutoffFreq(){
    return highpassCutoffFreq;
}

int CommandCat::getLowpassCutoffFreq(){
    return lowpassCutoffFreq;
}

int CommandCat::getNotchCutoffFreq(){
    return notchCutoffFreq;
}

int CommandCat::getThreshold(int channel){
    return threshold[channel];
}

int CommandCat::getThresholdPower(int channel){
    return thresholdPower[channel];
}

QByteArray CommandCat::getlastRpiCommand(){
    return rpiCommand;
}

}


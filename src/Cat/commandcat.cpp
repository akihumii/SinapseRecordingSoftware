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
    appendRpiCommand((short) getThreshold(channel));
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
    appendRpiCommand((short) getThresholdPower(channel));
    qDebug() << "Sent threshold power: " << rpiCommand ;
}

void CommandCat::sendDecodingWindowSize(){
    rpiCommand.clear();
    rpiCommand.append((const char) DECODING_WINDOW_SIZE);
    appendRpiCommand((short) getDecodingWindowSize());
    qDebug() << "Send decoding window size: " << rpiCommand;
}

void CommandCat::sendOverlapWindowSize(){
    rpiCommand.clear();
    rpiCommand.append((const char) OVERLAP_WINDOW_SIZE);
    appendRpiCommand((short) getOverlapWindowSize());
    qDebug() << "Send overlap window size: " << rpiCommand;
}

void CommandCat::sendSamplingFreq(){
    rpiCommand.clear();
    rpiCommand.append((const char) SAMPLING_FREQ);
    appendRpiCommand((short) getSamplingFreq());
    qDebug() << "Send sampling frequency: " << rpiCommand;
}

void CommandCat::sendExtendStimulation(){
    rpiCommand.clear();
    rpiCommand.append((const char) EXTEND_STIMULATION);
    appendRpiCommand((short) getExtendStimulation());
    qDebug() << "Send Extend Stimulation: " << rpiCommand;
}

void CommandCat::sendHighpassCutoffFreq(){
    rpiCommand.clear();
    rpiCommand.append((const char) HIGHPASS_CUTOFF_FREQ);
    appendRpiCommand((short) getHighpassCutoffFreq());
    qDebug() << "Send highpass cutoff frequency: " << rpiCommand;
}

void CommandCat::sendLowpassCutoffFreq(){
    rpiCommand.clear();
    rpiCommand.append((const char) LOWPASS_CUTOFF_FREQ);
    appendRpiCommand((short) getLowpassCutoffFreq());
    qDebug() << "Send lowpass cutoff frequency: " << rpiCommand;
}

void CommandCat::sendNotchCutoffFreq(){
    rpiCommand.clear();
    rpiCommand.append((const char) NOTCH_CUTOFF_FREQ);
    appendRpiCommand((short) getNotchCutoffFreq());
    qDebug() << "Send notch cutoff frequency: " << rpiCommand;
}

void CommandCat::sendSMChannel(){
    rpiCommand.clear();
    rpiCommand.append((const char) CLASSIFY_DIMENSION);
    appendRpiCommand((short) getSMChannel());
    qDebug() << "Send sm channel: " << rpiCommand;
}

void CommandCat::sendStartStimulation(){
    rpiCommand.clear();
    rpiCommand.append((const char) STIMULATION_FLAG);
    appendRpiCommand((short) getStartStimulation());
    qDebug() << "Send start stimulation: " << rpiCommand;
}

void CommandCat::appendRpiCommand(short data){
    if(data != 0 && data % 256 == 0){  // to solve the issue where the ocmmand will be terminated at 0 while being written into socket
        data += 1;
    }
    rpiCommand.append((const char)((data & (0xFF << (0*8))) >> (0*8)));
    if(sizeof(data) > 1){
        rpiCommand.append((const char)((data & (0xFF << (1*8))) >> (1*8)));
    }
    else{
        rpiCommand.append((const char) 0);  // can set to other value if it'll still terminate at zero
    }
}

void CommandCat::updateRpiCommand(char *data){
    rpiCommand.clear();
    rpiCommand.append((const char) data[0]);
    rpiCommand.append((const char) data[1]);
    rpiCommand.append((const char) 1);
}

void CommandCat::setSMChannel(int channel, int value){
    SMChannel[channel] = value;
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

void CommandCat::setStartStimulation(bool flag){
    startStimulationFlag = flag;
}

int CommandCat::getSMChannel(){
    int temp;
    temp = SMChannel[1] << 1 | SMChannel[0];
    temp += 520;
    return temp;
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

int CommandCat::getStartStimulation(){
    int temp;
    temp = 520 + (int) startStimulationFlag;
    return temp;
}

QByteArray CommandCat::getlastRpiCommand(){
    return rpiCommand;
}

}


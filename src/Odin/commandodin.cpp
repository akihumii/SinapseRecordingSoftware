#include "commandodin.h"

namespace Odin {

CommandOdin::CommandOdin(SerialOdin *serialOdin_, SocketOdin *socketOdin_) {
    serialOdin = serialOdin_;
    socketOdin = socketOdin_;
}

void CommandOdin::initialiseCommand(){
    sendStart();
    QTimer::singleShot((800), [=] {
        sendFrequency();
    });
    for(int i = 0; i < 4; i++){
        QTimer::singleShot((1000+i*200), [=] {
                sendPulseDuration(i);
        });
    }
    for(int i = 0; i < 4; i++){
        QTimer::singleShot((1800+i*200), [=] {
            sendAmplitude(i);
        });
    }
    QTimer::singleShot((2800), [=] {
        sendThresholdEnable();
    });
    QTimer::singleShot((3000), [=] {
        sendChannelEnable();
    });
    qDebug() << "Finished initialisation!";
}

void CommandOdin::sendCommand(){
    if(serialOdin->isOdinSerialConnected()){
        serialOdin->writeCommand(outgoingCommand);
    }
    socketOdin->writeCommand(outgoingCommand);
}

void CommandOdin::sendStart(){
    outgoingCommand.clear();
    outgoingCommand.append((const char) 0xF8);
    outgoingCommand.append((const char) 0xF8);
    if(serialOdin->isOdinSerialConnected()){
        serialOdin->writeCommand(outgoingCommand);
    }
    qDebug() << "Sending start via socket";
    socketOdin->writeCommand(outgoingCommand);
}

void CommandOdin::sendStop(){
    outgoingCommand.clear();
    outgoingCommand.append((const char) 0x8F);
    outgoingCommand.append((const char) 0x8F);
    if(serialOdin->isOdinSerialConnected()){
        serialOdin->writeCommand(outgoingCommand);
    }
    qDebug() << "Sending via socket";
    socketOdin->writeCommand(outgoingCommand);
}

void CommandOdin::sendAmplitude(int channel){
    outgoingCommand.clear();
    switch (channel){
        case 0:{
            outgoingCommand.append((const char) AMPLITUDE_CHN1);
            break;
        }
        case 1:{
            outgoingCommand.append((const char) AMPLITUDE_CHN2);
            break;
        }
        case 2:{
            outgoingCommand.append((const char) AMPLITUDE_CHN3);
            break;
        }
        case 3:{
            outgoingCommand.append((const char) AMPLITUDE_CHN4);
            break;
        }
    }
    if((quint8) getAmplitudeByte(channel) > 240){
        outgoingCommand.append((const char) 240);
    }
    else{
        outgoingCommand.append((const char) getAmplitudeByte(channel));
    }
    if(serialOdin->isOdinSerialConnected()){
        serialOdin->writeCommand(outgoingCommand);
    }
    socketOdin->writeCommand(outgoingCommand);
    qDebug() << "Sent Amplitude" << outgoingCommand;
}

void CommandOdin::sendPulseDuration(int channel){
    outgoingCommand.clear();
    switch (channel){
        case 0:{
            outgoingCommand.append((const char) PULSEDURATION_CHN1);
            break;
        }
        case 1:{
            outgoingCommand.append((const char) PULSEDURATION_CHN2);
            break;
        }
        case 2:{
            outgoingCommand.append((const char) PULSEDURATION_CHN3);
            break;
        }
        case 3:{
            outgoingCommand.append((const char) PULSEDURATION_CHN4);
            break;
        }
    }
    outgoingCommand.append((const char) getPulseDurationByte(channel));
    if(serialOdin->isOdinSerialConnected()){
        serialOdin->writeCommand(outgoingCommand);
    }
    socketOdin->writeCommand(outgoingCommand);
    qDebug() << "Sent Pulse Duration";
    for(int i = 0; i < outgoingCommand.size(); i++){
        qDebug() << (quint8) outgoingCommand.at(i);
    }
}

void CommandOdin::sendFrequency(){
    outgoingCommand.clear();
    outgoingCommand.append((const char) FREQUENCY);
    outgoingCommand.append((const char) getFrequency());
    if(serialOdin->isOdinSerialConnected()){
        serialOdin->writeCommand(outgoingCommand);
    }
    socketOdin->writeCommand(outgoingCommand);
    qDebug() << "Sent Frequency";
    for(int i = 0; i < outgoingCommand.size(); i++){
        qDebug() << (quint8) outgoingCommand.at(i);
    }
}

void CommandOdin::sendChannelEnable(){
    outgoingCommand.clear();
    outgoingCommand.append((const char) CHANNEL_ENABLE);
    outgoingCommand.append((const char) getChannelEnabled());
    if(serialOdin->isOdinSerialConnected()){
        serialOdin->writeCommand(outgoingCommand);
    }
    socketOdin->writeCommand(outgoingCommand);
    qDebug() << "Sent Channel Enabled";
    for(int i = 0; i < outgoingCommand.size(); i++){
        qDebug() << (quint8) outgoingCommand.at(i);
    }
}

QByteArray CommandOdin::getlastSentCommand(){
    return outgoingCommand;
}

void CommandOdin::setAmplitude(int channel, double value){
    amplitude[channel] = value;
}

double CommandOdin::getAmplitude(int channel){
    return amplitude[channel];
}

unsigned char CommandOdin::getAmplitudeByte(int index){
    if(amplitude[index] == 0.0){
        return 0;
    }
    else{
    //        unsigned char temp = PulseMag[index]*PulseMag[index]*0.0012 + PulseMag[index]*25.556 - 6.601;         // For 9.3mA
    // =================================================== HACK JOB =============================================================//
            QString formula =  QDir::currentPath() + QDir::separator() + "formula.txt";
            QFile formulaFile(formula);
            if(formulaFile.exists()){
                if(!formulaFile.open(QIODevice::ReadOnly | QIODevice::Text))
                    return 0.0;
                QTextStream in(&formulaFile);
                QString temp;
                temp = in.readLine();
                a = temp.toFloat();
    //            qDebug() << "a: " << temp;
                temp = in.readLine();
                b = temp.toFloat();
    //            qDebug() << "b: " << temp;
                temp = in.readLine();
                c = temp.toFloat();
    //            qDebug() << "c: " << temp;
                formulaFile.close();
            }
    // =================================================== HACK JOB =============================================================//
        unsigned char temp;
        if(amplitude[index] >= 150.0){
            temp = amplitude[index]*amplitude[index]*a + amplitude[index]*b - c;       // For 20.0mA
        }
        else{
            temp = 150.0*150.0*a + 150.0*b - c;       // For 20.0mA
        }
        qDebug() << "What is temp here " << temp;
        return temp;
    }
}

void CommandOdin::setPulseDuration(int channel, int duration){
    pulseDuration[channel] = duration;
}

char CommandOdin::getPulseDurationByte(int channel){
    return (char) ((pulseDuration[channel]/5) - 3);
}

int CommandOdin::getPulseDuration(int channel){
    return pulseDuration[channel];
}

void CommandOdin::setFrequency(int value){
    frequency = value;
}

char CommandOdin::getFrequencyByte(){
    char temp = (char) ((((1000000/(quint8)frequency) -
                            (2 * (int) channelEnabled[0] * pulseDuration[0] + 22) -
                            (2 * (int) channelEnabled[1] * pulseDuration[1] + 22) -
                            (2 * (int) channelEnabled[2] * pulseDuration[2] + 22) -
                            (2 * (int) channelEnabled[3] * pulseDuration[3] + 22)) -
                            (700 * numChannels)) / 161);
    return temp;
}

int CommandOdin::getFrequency(){
    return frequency;
}

void CommandOdin::setChannelEnabled(int channel, bool flag){
    channelEnabled[channel] = flag;
    numChannels = 0;
    for(int i = 0; i < 4; i++){
        if(channelEnabled[i]){
            numChannels++;
        }
    }
    qDebug() << "Number of Channels enabled: " << numChannels;
}

char CommandOdin::getChannelEnabled(){
    char temp;
    temp = (char) channelEnabled[3] << 3 | (char) channelEnabled[2] << 2 | (char) channelEnabled[1] << 1 | (char) channelEnabled[0];
    return temp;
}

int CommandOdin::getNumChannelEnabled(){
    return numChannels;
}

void CommandOdin::sendStepSizeIncrease(){
    outgoingCommand.clear();
    outgoingCommand.append((const char) STEP_INCREASE);
    outgoingCommand.append((const char) getStepSize());
    if(serialOdin->isOdinSerialConnected()){
        serialOdin->writeCommand(outgoingCommand);
    }
    socketOdin->writeCommand(outgoingCommand);
    currentAmplitude = ((currentAmplitude+getStepSize()) > (unsigned char) 240)? (unsigned char) 240 : currentAmplitude+getStepSize();
    qDebug() << "Sent Step Size Increase" << (quint8) currentAmplitude;
}

void CommandOdin::sendStepSizeDecrease(){
    outgoingCommand.clear();
    outgoingCommand.append((const char) STEP_DECREASE);
    outgoingCommand.append((const char) getStepSize());
    if(serialOdin->isOdinSerialConnected()){
        serialOdin->writeCommand(outgoingCommand);
    }
    socketOdin->writeCommand(outgoingCommand);
    currentAmplitude = ((currentAmplitude - getStepSize()) <(unsigned char) 0)? (unsigned char) 0 : currentAmplitude-getStepSize();
    qDebug() << "Sent Step Size Increase" << (quint8) currentAmplitude;
}

void CommandOdin::sendThresholdEnable(){
    outgoingCommand.clear();
    outgoingCommand.append((const char) THRESHOLD_ENABLE);
    outgoingCommand.append((const char) getThresholdEnable());
    if(serialOdin->isOdinSerialConnected()){
        serialOdin->writeCommand(outgoingCommand);
    }
    socketOdin->writeCommand(outgoingCommand);
    qDebug() << "Sent Threshold Enabled";
    for(int i = 0; i < outgoingCommand.size(); i++){
        qDebug() << (quint8) outgoingCommand.at(i);
    }
}

void CommandOdin::setThresholdEnable(char value){
    thresholdEnable = value;
}

char CommandOdin::getThresholdEnable(){
    return thresholdEnable;
}

void CommandOdin::setStepSize(double step){
    stepSize = step * 12;
    qDebug() << "Step size: " << (quint8) stepSize;
}

char CommandOdin::getStepSize(){
    return stepSize;
}

unsigned char CommandOdin::getCurrentAmplitude(){
    return currentAmplitude;
}

void CommandOdin::setCurrentAmplitude(unsigned char value){
    currentAmplitude = value;
}

}

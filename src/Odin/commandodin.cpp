#include "commandodin.h"

namespace Odin {

CommandOdin::CommandOdin(SerialOdin *serialOdin_, SocketOdin *socketOdin_) {
    serialOdin = serialOdin_;
    socketOdin = socketOdin_;
}

void CommandOdin::initialiseCommand(){
    sendStart();
    QThread::msleep(800);
    sendFrequency();
    QThread::msleep(100);
    for(int i = 0; i < 4; i++){
        sendPulseDuration(i);
    }
    QThread::msleep(100);
    for(int i = 0; i < 4; i++){
        sendAmplitude(i);
    }
    qDebug() << "Finished initialisation!";
}

void CommandOdin::sendCommand(){
    if(serialOdin->isOdinSerialConnected()){
//        qDebug() << "Sending via serial";
        serialOdin->writeCommand(outgoingCommand);
    }
//    else if(socketOdin->isConnected()){
//        qDebug() << "Sending via socket";
        socketOdin->writeCommand(outgoingCommand);
//    }
}

void CommandOdin::sendStart(){
    outgoingCommand.clear();
    outgoingCommand.append((const char) 0xF8);
    if(serialOdin->isOdinSerialConnected()){
//        qDebug() << "Sending via serial";
        serialOdin->writeCommand(outgoingCommand);
    }
//    else if(socketOdin->isConnected()){
        qDebug() << "Sending start via socket";
        socketOdin->writeCommand(outgoingCommand);
//    }
}

void CommandOdin::sendStop(){
    outgoingCommand.clear();
    outgoingCommand.append((const char) 0x8F);
    if(serialOdin->isOdinSerialConnected()){
//        qDebug() << "Sending via serial";
        serialOdin->writeCommand(outgoingCommand);
    }
//    else if(socketOdin->isConnected()){
        qDebug() << "Sending via socket";
        socketOdin->writeCommand(outgoingCommand);
//    }
}

void CommandOdin::sendAmplitude(int channel){
    QByteArray temp;
    switch (channel){
        case 0:{
            temp.append((const char) AMPLITUDE_CHN1);
            break;
        }
        case 1:{
            temp.append((const char) AMPLITUDE_CHN2);
            break;
        }
        case 2:{
            temp.append((const char) AMPLITUDE_CHN3);
            break;
        }
        case 3:{
            temp.append((const char) AMPLITUDE_CHN4);
            break;
        }
    }
    temp.append((const char) getAmplitudeByte(channel));
    if(serialOdin->isOdinSerialConnected()){
        serialOdin->writeCommand(temp);
    }
    socketOdin->writeCommand(temp);
    qDebug() << "Sent Amplitude" << temp;
}

void CommandOdin::sendPulseDuration(int channel){
    QByteArray temp;
    switch (channel){
        case 0:{
            temp.append((const char) PULSEDURATION_CHN1);
            break;
        }
        case 1:{
            temp.append((const char) PULSEDURATION_CHN2);
            break;
        }
        case 2:{
            temp.append((const char) PULSEDURATION_CHN3);
            break;
        }
        case 3:{
            temp.append((const char) PULSEDURATION_CHN4);
            break;
        }
    }
    temp.append((const char) getPulseDurationByte(channel));
    if(serialOdin->isOdinSerialConnected()){
        serialOdin->writeCommand(temp);
    }
    socketOdin->writeCommand(temp);
    qDebug() << "Sent Pulse Duration";
    for(int i = 0; i < temp.size(); i++){
        qDebug() << (quint8) temp.at(i);
    }
}

void CommandOdin::sendFrequency(){
    QByteArray temp;
    temp.append((const char) FREQUENCY);
    temp.append((const char) getFrequencyByte());
    if(serialOdin->isOdinSerialConnected()){
        serialOdin->writeCommand(temp);
    }
    socketOdin->writeCommand(temp);
    qDebug() << "Sent Frequency";
    for(int i = 0; i < temp.size(); i++){
        qDebug() << (quint8) temp.at(i);
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
            }
    // =================================================== HACK JOB =============================================================//
        unsigned char temp = amplitude[index]*amplitude[index]*a + amplitude[index]*b;       // For 20.0mA
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
    char temp = (char) ((((1000000/(quint8)frequency) - amplitude[0] - amplitude[1] - amplitude[2] - amplitude[3]) - (710*numChannels)) / 267);
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

}

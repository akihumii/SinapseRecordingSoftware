#include "commandodin.h"

namespace Odin {

CommandOdin::CommandOdin(SerialOdin *serialOdin_, SocketOdin *socketOdin_) {
    serialOdin = serialOdin_;
    socketOdin = socketOdin_;
}

void CommandOdin::constructCommand(){
}

void CommandOdin::sendCommand(){
    constructCommand();
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
    outgoingCommand.append((char) 0xF8);
    if(serialOdin->isOdinSerialConnected()){
//        qDebug() << "Sending via serial";
        serialOdin->writeCommand(outgoingCommand);
    }
//    else if(socketOdin->isConnected()){
//        qDebug() << "Sending via socket";
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
        constructCommand();
//    }
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
    unsigned char temp = amplitude[index]*amplitude[index]*a + amplitude[index]*b - c;       // For 20.0mA
    return temp;
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

}

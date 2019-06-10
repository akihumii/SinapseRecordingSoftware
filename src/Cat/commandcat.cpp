#include "commandcat.h"

namespace Cat {

CommandCat::CommandCat(){
}

int CommandCat::getThreshold(int channel){
    return threshold[channel];
}

void CommandCat::setThreshold(int channel, int value){
    threshold[channel] = value;
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
    qDebug() << "Sent thresholds" << rpiCommand;
}

int CommandCat::getThresholdPower(int channel){
    return thresholdPower[channel];
}

void CommandCat::setThresholdPower(int channel, int value){
    thresholdPower[channel] = value;
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
    qDebug() << "Sent threshold power" << rpiCommand ;
}

QByteArray CommandCat::getlastRpiCommand(){
    return rpiCommand;
}

}


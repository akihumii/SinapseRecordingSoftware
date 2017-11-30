#include "command.h"

Command::Command(Channel *NeutrinoChannel_)
{
    NeutrinoChannel = NeutrinoChannel_;
}

QByteArray Command::constructCommand()
{
    QByteArray outgoingCommand;

    for(int i=0;i<5;i++){       //Append 5x A5 in Hexadecimal
        outgoingCommand.append((const char) MARKER_A5);
    }

    outgoingCommand.append((const char) ChipID);    //Append Chip ID

    switch (OPModeSel){
        case 0:{                //Digital Command Loopback
            outgoingCommand.append((const char) DIGCOMLOOPBACK);
            outgoingCommand.append((const char) DCL_Mode);
            break;
        }
        case 1:{                //Data BER Assessment
            outgoingCommand.append(1, (const char) DATA_BER_MODE);
            for(int i=0;i<8;i++){
                outgoingCommand.append((const char) BERbytesHex[i]);
            }
            break;
        }
        case 2:{                //Selected Channel Out (8-bit)
            outgoingCommand.append((const char) BITMODE_8);
            outgoingCommand.append(NeutrinoChannel->getChannelState());
            break;
        }
        case 3:{                //Selected Channel Out (10-bit)
            outgoingCommand.append((const char) BITMODE_10);
            outgoingCommand.append(NeutrinoChannel->getChannelState());
            break;
        }
        case 4:{                //Oscillator Clock Tuning
            outgoingCommand.append((const char) OSC_CLK_MODE);
            break;
        }
        case 5:{                //Analog Measurement (8-bit)
            outgoingCommand.append((const char) ANALOG_MEASURE_8BIT);
            break;
        }
        case 6:{                //Analog Measurement (10-bit)
            outgoingCommand.append((const char) ANALOG_MEASURE_10Bit);
            break;
        }
        case 7:{                //Bioimpedance Measurement (8-bit)
            outgoingCommand.append((const char) BIOIMP_MEASURE_8BIT);
            outgoingCommand.append((const char) bioimp);
            break;
        }
        case 8:{                //Bioimpedance Measurement (10-bit)
            outgoingCommand.append((const char) BIOIMP_MEASURE_10BIT);
            outgoingCommand.append((const char) bioimp);
            break;
        }
        case 9:{                //Power Level Measurement
            outgoingCommand.append((const char) PWR_LVL_MEASURE);
            break;
        }
        case 10:{                //Bioimpedance Measurement (8-bit)
            outgoingCommand.append((const char) BIOIMP_MEASURE_8BIT);
            outgoingCommand.append((const char) bioimp);
            break;
        }
        case 11:{                //Bioimpedance Measurement (8-bit)
            outgoingCommand.append((const char) BIOIMP_MEASURE_8BIT);
            outgoingCommand.append((const char) bioimp);
            break;
        }
        default:
        break;
    }
    for(int i=0; i<14;i++){
        outgoingCommand.append((const char) JTAGarray[i]);
    }
    outgoingCommand.append((const char) MARKER_5A);
    return outgoingCommand;
}

QByteArray Command::resetCommand(){
    QByteArray outgoingCommand;

    for(int i=0;i<5;i++){
        outgoingCommand.append((const char) MARKER_A5);  //Append 5x A5 in Hexadecimal
    }
    outgoingCommand.append((const char) ChipID);        //Append Chip ID
    outgoingCommand.append((const char) RESETMODE);     //Hex value of 10 in decimal (OpMode of Reset)
    outgoingCommand.append((const char) MARKER_5A);      //Hex value of 5A in decimal

    return outgoingCommand;
}

void Command::setOPMode(int Mode){
    OPModeSel = (quint8) Mode;
}

quint8 Command::getOPMode(){
    return OPModeSel;
}

void Command::setChipID(int IDnum){
    ChipID = (quint8) IDnum;
}

quint8 Command::getChipID(){
    return ChipID;
}

void Command::setBioImpBit(int Index){
    bioimp |= 1<<Index;
}

quint8 Command::getBioImp(){
    return bioimp;
}

void Command::clearBioImpBit(int Index){
    bioimp &= ~(1<<Index);
}

void Command::setJTAGbit(int Index){
    JTAGarray[(Index/8)] |= 1<<(Index%8);
}

void Command::clearJTAGbit(int Index){
    JTAGarray[(Index/8)] &= ~(1<<(Index%8));
}

quint8 Command::getJTAG(int Index){
    return JTAGarray[Index];
}

void Command::setDCLMode(quint8 newDCL_Mode){
    DCL_Mode = newDCL_Mode;
}

quint8 Command::getDCLMode(){
    return DCL_Mode;
}

void Command::updateBER(int Index, QString newBER){
    BERbytesHex[Index] = newBER.toInt(&ok,16);
}

bool Command::havelastCommand(){
    return lastCommandexist;
}

void Command::setlastCommand(bool flag){
    lastCommandexist = flag;
}



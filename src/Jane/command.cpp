#include "command.h"
#include "qdebug.h"

Command::Command(Stimulator *thorParam_)
{
    thorParam = thorParam_;
}

QByteArray Command::constructCommand()
{
    outgoingCommand.clear();
    addSyncPulse();             //add 5 x 5A
    addChipID();
    addData();
    addEndPulse();
    cmd = outgoingCommand;
    return outgoingCommand;
}

QByteArray Command::resetCommand()
{
    outgoingCommand.clear();
    addSyncPulse();             //add 5 x 5A
    addChipID();
    outgoingCommand.append((const char) RESETMODE);
    addEndPulse();

    cmd = outgoingCommand;
    return outgoingCommand;
}

// Append 5 X 5A in Hexadecimal
void Command::addSyncPulse(){
    for(int i = 0; i< 5; i++){
        outgoingCommand.append((const char) FR_A5);
    }
}

void Command::addChipID()
{
    outgoingCommand.append((const char) chipID);    // Append chip ID
}



void Command::addData()
{
    switch(OPModeSelect){
        case 0:{                                    // Digital Command Loopback
            outgoingCommand.append((const char)DIGCOMLOOPBACK);
            outgoingCommand.append((const char)DCL_Mode);
            break;
        }
        case 1:{                                    // Data BER Assestment
            outgoingCommand.append((const char)DATA_BER_MODE);
            for(int i = 0; i < 8; i++)
                outgoingCommand.append((const char)BERbytesHex[i]);
            break;
        }
        case 2:{                                    // JTAG Programming Mode
            outgoingCommand.append((const char)JTAG_PROG);
            break;
        }
        case 3:{                                    // Stimulator parameter program
            outgoingCommand.append((const char)STIM_PARAMETER);

            if ((thorParam->isStimulatorParamSetLower()) && (thorParam->isStimulatorParamSetUpper())){
                qDebug()<<"Stimulator parameter program: Lower + Upper";
                //cont 1st cmd
                outgoingCommand.append((const char)STIM_LOWER);
                addStimulationParamSet(0,8);
                addEndPulse();
                //start 2nd cmd
                addSyncPulse();
                addChipID();
                outgoingCommand.append((const char)STIM_UPPER);
                addStimulationParamSet(8,16);
            }
            else if(thorParam->isStimulatorParamSetLower()){
                qDebug()<< "Stimulator parameter program: Lower";
                outgoingCommand.append((const char)STIM_LOWER);
                addStimulationParamSet(0,8);
            }
            else if (thorParam->isStimulatorParamSetUpper()){
                qDebug()<<"Stimulator parameter program: Upper";
                outgoingCommand.append((const char)STIM_UPPER);
                addStimulationParamSet(8,16);
            }
            break;
        }
        case 4:{                                    // Stimulator subsequence program
            qDebug()<<"Stimulator subsequence program: Lower + Upper";
            outgoingCommand.append((const char)STIM_SEQUENCE);
            outgoingCommand.append((const char) 0B00000000);
            for(int i = 0; i < 8; i++){
                outgoingCommand.append(thorParam -> getSubSequence(i));
                outgoingCommand.append(thorParam -> getSubSequenceStart(i));
                outgoingCommand.append(thorParam -> getSubSequenceStop(i));
            }
            break;
        }
        case 5:{                                    // Stimulator Trigger mode
            outgoingCommand.append((const char)STIM_TRIGGER);
            outgoingCommand.append(thorParam->getTriggerCmd());
        }
        case 6:{                                    // Oscilator clock tuning
            outgoingCommand.append((const char)OSC_CLK_MODE);
        }
        case 7:{                                    // 8-bit Analog Measurement
            outgoingCommand.append((const char)BITMODE_8);
        }
        case 8:{                                    // Bioimpedance Measurement
            outgoingCommand.append((const char)BIOIMP_MEASURE_8BIT);
        }
        default:
        break;
    }
}

void Command::addEndPulse()
{
    outgoingCommand.append((const char) FR_5A); // Append A5 as a last framemarker
}

void Command::setOPMode(int mode) { OPModeSelect = (quint8)mode;}
quint8 Command::getOPMode() { return OPModeSelect; }

void Command::setChipID(int IDNum) { chipID = (qint8)IDNum;}
quint8 Command::getChipID() { return chipID; }

void Command::setBioImpBit(int index) {bioimp |= 1 << index;}
void Command::clearBioImpBit(int index) {bioimp &= ~(1 << index);}
quint8 Command::getBioImp() { return bioimp; }

void Command::setJTAGBit(int index) { JTAGarray[(index/8)] |= 1 << (index %8); }
void Command::clearJTAGBit(int index) { JTAGarray[(index/8)] &= ~(1 << (index %8)); }
quint8 Command::getJTAG(int index) { return JTAGarray[index]; }

void Command::setDCLMode(quint8 newDCL_Mode) { DCL_Mode = newDCL_Mode; }
quint8 Command::getDCLMode() { return DCL_Mode; }

void Command::setLastCommand(bool flags) { lastCommandExits = flags; }


bool Command::haveLastCommand() { return lastCommandExits; }

void Command::updateBER(int index, QString newBER) { BERbytesHex[index] = newBER.toInt(&ok, 16); }

void Command::updateTriggerCmd(int index, QString state)
{
    thorParam->setTriggerCmd(index,(boolean) state.toInt());
}

void Command::addStimulationParamSet(int start, int end)
{
    for (int i=start;i<end;i++){
            outgoingCommand.append(thorParam->paramValue[i]);
    }
}

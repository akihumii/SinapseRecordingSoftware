#include "command.h"
#include "qdebug.h"

Command::Command(Stimulator *thorParam_)
{
    thorParam = thorParam_;
}

QByteArray Command::constructCommand()
{
    cmd.clear();
    addSyncPulse();             //add 5 x 5A
    addChipID();
    addData();
    addEndPulse();
    return cmd;
}

QByteArray Command::resetCommand()
{
    cmd.clear();
    addSyncPulse();             //add 5 x 5A
    addChipID();
    cmd.append((const char) RESETMODE);
    addEndPulse();
    return cmd;
}

// Append 5 X 5A in Hexadecimal
void Command::addSyncPulse(){
    for(int i = 0; i< 5; i++){
        cmd.append((const char) FR_A5);
    }
}

void Command::addChipID()
{
    cmd.append((const char) chipID);    // Append chip ID
}

void Command::addData()
{
    switch(OPModeSelect){
        case 0:{                                    // Digital Command Loopback
            cmd.append((const char)DIGCOMLOOPBACK);
            cmd.append((const char)DCL_Mode);
            break;
        }
        case 1:{                                    // Data BER Assestment
            cmd.append((const char)DATA_BER_MODE);
            for(int i = 0; i < 8; i++)
                cmd.append((const char)BERbytesHex[i]);
            break;
        }
        case 2:{                                    // JTAG Programming Mode
            cmd.append((const char)JTAG_PROG);
            for (int i=0;i<6;i++){
                cmd.append(JTAGarray[i]);
            }
            break;
        }
        case 3:{                                    // Stimulator parameter program
            cmd.append((const char)STIM_PARAMETER);

            if ((thorParam->isStimulatorParamSetLower()) && (thorParam->isStimulatorParamSetUpper())){
                qDebug()<<"Stimulator parameter program: Lower + Upper";
                //cont 1st cmd
                cmd.append((const char)STIM_LOWER);
                addStimulationParamSet(0,8);
                addEndPulse();
                //start 2nd cmd
                addSyncPulse();
                addChipID();
                cmd.append((const char)STIM_UPPER);
                addStimulationParamSet(8,16);
            }
            else if(thorParam->isStimulatorParamSetLower()){
                qDebug()<< "Stimulator parameter program: Lower";
                cmd.append((const char)STIM_LOWER);
                addStimulationParamSet(0,8);
            }
            else if (thorParam->isStimulatorParamSetUpper()){
                qDebug()<<"Stimulator parameter program: Upper";
                cmd.append((const char)STIM_UPPER);
                addStimulationParamSet(8,16);
            }
            break;
        }
        case 4:{                                    // Stimulator subsequence program
            qDebug()<<"Stimulator subsequence program:";
            cmd.append((const char)STIM_SEQUENCE);
            cmd.append((const char) 0B00000000);
            for(int i = 0; i < 8; i++){
                cmd.append(thorParam -> getSubSequence(i));
                cmd.append(thorParam -> getSubSequenceStart(i));
                cmd.append(thorParam -> getSubSequenceStop(i));
            }
            break;
        }
        case 5:{                                    // Stimulator Trigger mode
            cmd.append((const char)STIM_TRIGGER);
            cmd.append(thorParam->getTriggerCmd());
            break;
        }
        case 6:{                                    // Oscilator clock tuning
            cmd.append((const char)OSC_CLK_MODE);
            for (int i=0;i<6;i++){
                cmd.append(JTAGarray[i]);
            }
            break;
        }
        case 7:{                                    // 8-bit Analog Measurement
            cmd.append((const char)BITMODE_8);
            qDebug() << thorParam->getTriggerCmd();
            cmd.append(thorParam->getTriggerCmd());
            break;
        }
        case 8:{                                    // Bioimpedance Measurement
            cmd.append((const char)BIOIMP_MEASURE_8BIT);
            break;
        }
        default:
        break;
    }
}

void Command::addEndPulse()
{
    cmd.append((const char) FR_5A); // Append A5 as a last framemarker
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

QByteArray Command::getCmd() const
{
    return cmd;
}

void Command::setCmd(const QByteArray &value)
{
    cmd = value;
}

bool Command::haveLastCommand() { return lastCommandExits; }

void Command::updateBER(int index, QString newBER) { BERbytesHex[index] = newBER.toInt(&ok, 16); }

void Command::updateTriggerCmd(int index, QString state)
{
    qDebug() << index << ": " << state;
    thorParam->setTriggerCmd(index,(bool) state.toInt());
}

void Command::addStimulationParamSet(int start, int end)
{
    for (int i=start;i<end;i++){
            cmd.append(thorParam->paramValue[i]);
    }
}

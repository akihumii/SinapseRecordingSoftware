#include "command.h"
#include "qdebug.h"

command::command(Stimulator *thorParam_)
{
    thorParam = thorParam_;
}

QByteArray command::constructCommand()
{
    QByteArray outgoingCommand;
    for(int i = 0; i< 5; i++)                       // Append 5 X 5A in Hexadecimal
        outgoingCommand.append((const char) MARKER_A5);

    outgoingCommand.append((const char) chipID);    // Append chip ID
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
            break;
        }
        case 4:{                                    // Stimulator subsequence program
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
    outgoingCommand.append((const char) MARKER_5A); // Append A5 as a last framemarker
    return outgoingCommand;
}

QByteArray command::resetCommand()
{
    QByteArray outgoingCommand;
    for(int i = 0; i < 5; i++)
        outgoingCommand.append((const char) MARKER_A5);

    outgoingCommand.append((const char) chipID);
    outgoingCommand.append((const char) RESETMODE);
    outgoingCommand.append((const char) MARKER_5A);

    return outgoingCommand;
}

void command::setOPMode(int mode) { OPModeSelect = (quint8)mode;}
quint8 command::getOPMOde() { return OPModeSelect; }

void command::setChipID(int IDNum) { chipID = (qint8)IDNum;}
quint8 command::getChipID() { return chipID; }

void command::setBioImpBit(int index) {bioimp |= 1 << index;}
void command::clearBioImpBit(int index) {bioimp &= ~(1 << index);}
quint8 command::getBioImp() { return bioimp; }

void command::setJTAGBit(int index) { JTAGarray[(index/8)] |= 1 << (index %8); }
void command::clearJTAGBit(int index) { JTAGarray[(index/8)] &= ~(1 << (index %8)); }
quint8 command::getJTAG(int index) { return JTAGarray[index]; }

void command::setDCLMode(quint8 newDCL_Mode) { DCL_Mode = newDCL_Mode; }
quint8 command::getDCLMode() { return DCL_Mode; }

void command::setLastCommand(bool flags) { lastCommandExits = flags; }
bool command::haveLastCommand() { return lastCommandExits; }

void command::updateBER(int index, QString newBER) { BERbytesHex[index] = newBER.toInt(&ok, 16); }

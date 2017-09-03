#include "import/commandj.h"
#include "qdebug.h"

CommandJ::CommandJ(Stimulator *thorParam_)
{
    thorParam = thorParam_;
}

QByteArray CommandJ::constructCommand()
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

QByteArray CommandJ::resetCommand()
{
    QByteArray outgoingCommand;
    for(int i = 0; i < 5; i++)
        outgoingCommand.append((const char) MARKER_A5);

    outgoingCommand.append((const char) chipID);
    outgoingCommand.append((const char) RESETMODE);
    outgoingCommand.append((const char) MARKER_5A);

    return outgoingCommand;
}

void CommandJ::setOPMode(int mode) { OPModeSelect = (quint8)mode;}
quint8 CommandJ::getOPMOde() { return OPModeSelect; }

void CommandJ::setChipID(int IDNum) { chipID = (qint8)IDNum;}
quint8 CommandJ::getChipID() { return chipID; }

void CommandJ::setBioImpBit(int index) {bioimp |= 1 << index;}
void CommandJ::clearBioImpBit(int index) {bioimp &= ~(1 << index);}
quint8 CommandJ::getBioImp() { return bioimp; }

void CommandJ::setJTAGBit(int index) { JTAGarray[(index/8)] |= 1 << (index %8); }
void CommandJ::clearJTAGBit(int index) { JTAGarray[(index/8)] &= ~(1 << (index %8)); }
quint8 CommandJ::getJTAG(int index) { return JTAGarray[index]; }

void CommandJ::setDCLMode(quint8 newDCL_Mode) { DCL_Mode = newDCL_Mode; }
quint8 CommandJ::getDCLMode() { return DCL_Mode; }

void CommandJ::setLastCommand(bool flags) { lastCommandExits = flags; }
bool CommandJ::haveLastCommand() { return lastCommandExits; }

void CommandJ::updateBER(int index, QString newBER) { BERbytesHex[index] = newBER.toInt(&ok, 16); }

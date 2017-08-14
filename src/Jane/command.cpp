#include "command.h"

command::command()
{

}

QByteArray command::constructCommand()
{
    QByteArray outgoingCommand;
    for(int i = 0; i< 5; i++)
        outgoingCommand.append((const char) MARKER_A5);

    outgoingCommand.append((const char) chipID);

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

void command::setOPMode(int mode) { OPModeSelect = (quint8)mode; }
quint8 command::getOPMOde() { return OPModeSelect; }

void command::setChipID(int IDNum) { chipID = (qint8)IDNum; }
quint8 command::getChipID() { return chipID; }

void command::setJTAGBit(int index) { JTAGarray[(index/8)] |= 1 << (index %8); }
void command::clearJTAGBit(int index) { JTAGarray[(index/8)] &= ~(1 << (index %8)); }
quint8 command::getJTAG(int index) { return JTAGarray[index]; }

void command::setDCLMode(quint8 newDCL_Mode) { DCL_Mode = newDCL_Mode; }
quint8 command::getDCLMode() { return DCL_Mode; }

void command::setLastCommand(bool flags) { lastCommandExits = flags; }
bool command::getLastCommand() { return lastCommandExits; }

void command::updateBER(int index, QString newBER) { BERbytesHex[index] = newBER.toInt(&ok, 16); }
quint8 command::getBioImp() { return bioimp; }

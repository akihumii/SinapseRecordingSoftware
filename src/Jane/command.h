#ifndef COMMAND_H
#define COMMAND_H

#include <QByteArray>
#include <QString>
#include "stimulator.h"
#include "dataprocessor.h"

#define MARKER_A5 0B10100101
#define MARKER_5A 0B01011010

typedef enum Frame{
    FR_5A = 0B01011010,
    FR_A5 = 0B10100101
//Order of FrameMarker should be A5 0F for 10 bit (Since bytes are combined with MSB|LSB)
//Order of FrameMarker should be 5AF0 for 8 bit
} Frame;

typedef enum operationMode{
    RESETMODE           = 0B00010000,
    DIGCOMLOOPBACK      = 0B00010100,
    DCL_ENTER           = 0B00010101,
    DCL_EXIT            = 0B00010110,
    DATA_BER_MODE       = 0B00010110,
    JTAG_PROG           = 0B11100101,
    STIM_PARAMETER      = 0B00110100,
    STIM_SEQUENCE       = 0B10110100,
    STIM_TRIGGER        = 0B10011011,
    OSC_CLK_MODE        = 0B00100100,
    BITMODE_8           = 0B00110101,
    BIOIMP_MEASURE_8BIT = 0B01100101,

    STIM_LOWER          = 0B01001100,
    STIM_UPPER          = 0B00110011,

    CHANNEL_ON = 0B10011001

} operationMode;

class Command
{

public:
    QByteArray cmd;
    Command(Stimulator *thorParam_);
    QByteArray constructCommand();
    QByteArray resetCommand();
    void setOPMode(int mode);
    void setChipID(int IDNum);
    void setBioImpBit(int index);
    void clearBioImpBit(int index);
    quint8 getBioImp();
    quint8 getOPMode();
    quint8 getJTAG(int index);
    quint8 getDCLMode();
    quint8 getChipID();
    void setJTAGBit(int index);
    void clearJTAGBit(int index);
    void setDCLMode(quint8 newDCL_Mode);
    void updateBER(int index, QString newBER);
    void updateTriggerCmd(int index, QString state);
    bool haveLastCommand();
    void setLastCommand(bool flags);

private:
    QByteArray outgoingCommand;
    Stimulator *thorParam;
    quint8 chipID = 0;
    quint8 OPModeSelect = 2;
    quint8 bioimp = 0;
    quint8 JTAGarray[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    quint8 DCL_Mode = DCL_EXIT;
    bool ok;
    int BERbytesHex[8];
    bool lastCommandExits = false;
    void addSyncPulse();
    void addChipID();
    void addData();
    void addEndPulse();
    void addStimulationParamSet(int start, int end);
};

#endif // COMMAND_H

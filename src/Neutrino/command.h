#ifndef COMMAND_H
#define COMMAND_H

#include "../common/qtincludes.h"
#include "channel.h"

#define MARKER_A5 0B10100101
#define MARKER_5A 0B01011010

typedef enum OperationMode{
    DIGCOMLOOPBACK = 0B00010100,
    DCL_ENTER = 0B00010101,
    DCL_EXIT = 0B00010110,
    RESETMODE = 0B00010000,
    DATA_BER_MODE = 0B00010110,
    BITMODE_8 = 0B00110100,
    BITMODE_10 = 0B10110100,
    CHANNEL_ON = 0B10011001,
    CHANNEL_OFF = 0B01100110,
    OSC_CLK_MODE = 0B00100100,
    ANALOG_MEASURE_8BIT = 0B00110101,
    ANALOG_MEASURE_10Bit = 0B10110101,
    BIOIMP_MEASURE_8BIT = 0B01100101,
    BIOIMP_MEASURE_10BIT = 0B11100101,
    PWR_LVL_MEASURE = 0B00010101
} OperationMode;

class Command
{
public:
    Command(Channel *NeutrinoChannel_);
    QByteArray constructCommand();
    QByteArray resetCommand();
    void setOPMode(int Mode);
    void setChipID(int IDnum);
    void setBioImpBit(int Index);
    void clearBioImpBit(int index);
    quint8 getBioImp();
    quint8 getOPMode();
    quint8 getJTAG(int Index);
    quint8 getDCLMode();
    quint8 getChipID();
    void setJTAGbit(int Index);
    void clearJTAGbit(int Index);
    void setDCLMode(quint8 newDCL_Mode);
    void updateBER(int Index, QString newBER);
    bool havelastCommand();
    void setlastCommand(bool flag);
    int getNumChannels();

private:
    Channel *NeutrinoChannel;
    quint8 ChipID = 0;
    quint8 OPModeSel = 2;

    quint8 bioimp = 0;
    quint8 JTAGarray[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    quint8 DCL_Mode = DCL_EXIT;
    bool ok;
    int BERbytesHex[8];
    bool lastCommandexist = false;
    int numChannels;
    QByteArray lastSentCommand;
};

#endif // COMMAND_H

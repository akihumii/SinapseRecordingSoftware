#ifndef COMMAND_H
#define COMMAND_H

#include <QByteArray>
#include <QString>

#define MARKER_A5 0B10100101
#define MARKER_5A 0B01011010

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
    BIOIMP_MEASURE_8BIT = 0B01100101
} operationMode;

class command
{

public:
    command();
    QByteArray constructCommand();
    QByteArray resetCommand();
    void setOPMode(int mode);
    void setChipID(int IDNum);
    quint8 getBioImp();
    quint8 getOPMOde();
    quint8 getJTAG(int index);
    quint8 getDCLMode();
    quint8 getChipID();
    void setJTAGBit(int index);
    void clearJTAGBit(int index);
    void setDCLMode(quint8 newDCL_Mode);
    void updateBER(int index, QString newBER);
    bool getLastCommand();
    void setLastCommand(bool flags);

private:
    quint8 chipID = 0;
    quint8 OPModeSelect = 2;
    quint8 bioimp = 0;
    quint8 JTAGarray[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    quint8 DCL_Mode = DCL_EXIT;
    bool ok;
    int BERbytesHex[8];
    bool lastCommandExits = false;
};

#endif // COMMAND_H

#ifndef COMMAND_H
#define COMMAND_H

#include "qtincludes.h"
#include "channel.h"

#define MarkerA5 0B10100101
#define Marker5A 0B01011010

#define DigComLoopBack 0B00010100
#define DCL_Enter 0B00010101
#define DCL_Exit 0B00010110

#define ResetMode 0B00010000
#define DataBERMode 0B00010110

#define BitMode_8 0B00110100
#define BitMode_10 0B10110100

#define Channel_On 0B10011001
#define Channel_Off 0B01100110

#define OSCCLKMode 0B00100100

#define AnalogMeasure_8Bit 0B00110101
#define AnalogMeasure_10Bit 0B10110101

#define BioImpMeasure_8Bit 0B01100101
#define BioImpMeasure_10Bit 0B11100101

#define PwrLvlMeasure 0B00010101


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


private:
    Channel *NeutrinoChannel;
    quint8 ChipID = 0;
    quint8 OPModeSel = 0;

    quint8 bioimp = 0;
    quint8 JTAGarray[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    quint8 DCL_Mode = DCL_Exit;
    bool ok;
    int BERbytesHex[8];
    bool lastCommandexist = false;
};

#endif // COMMAND_H

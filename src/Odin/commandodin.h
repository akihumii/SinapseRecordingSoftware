#ifndef COMMANDODIN_H
#define COMMANDODIN_H

#include "serialodin.h"
#include "socketodin.h"

typedef enum MODE{
    FLATSINGLE = 0x55,
    GRADEDAMPLITUDE = 0x65,
    GRADEDFFREQ_HIGHLOW = 0x85,
    GRADEDFREQ_LOWHIGH = 0x95,
    MULTICHANNEL_INTERLEAVED = 0xBB,
    MULTICHANNEL_NONINTERLEAVED = 0xBE
} MODE;

typedef enum CHANNELNUMBER{
    CHANNELA = 0,
    CHANNELB = 1,
    CHANNELC = 2,
    CHANNELD = 3
} CHANNELNUMBER;

typedef enum TIMEZONE{
    TIME_A = 0,
    TIME_B = 1,
    TIME_C = 2,
    TIME_D = 3
} TIMEZONE;

typedef enum ZONEMASK{
    MASKALLZONE = 0xFF,
    MASKZONE1 = 0xFD,
    MASKZONE2 = 0xFB,
    MASKZONE3 = 0xF7,
    MASKZONE4 = 0xEF,
    MASKZONE5 = 0xDF,
    MASKZONE6 = 0xBF,
    MASKZONE7 = 0x7F,
    MASKNONE = 0x55
} ZONEMASK;

class CommandOdin
{
public:
    CommandOdin(SerialOdin *serialOdin_, SocketOdin *socketOdin_);
    void sendTestCommand();
    void sendCommand();
    void setMode(int mode);
    void setChannelNumber(CHANNELNUMBER num);
    void setPulseMag(int index, double mag);
    void setPulseDuration(int duration);
    void setPulseNum(char num);
    void setInterPulseDuration(char duration);
    void setMultiChannelSequence(CHANNELNUMBER first, CHANNELNUMBER second, CHANNELNUMBER third, CHANNELNUMBER fourth);
    void setTimeZone(int index, TIMEZONE time);
    void setZoneMask(ZONEMASK mask);
    char getMode();
    char getChannelNumber();
    char getPulseMagByte(int index);
    char getPulseDuration();
    char getPulseNum();
    char getInterPulseDuration();
    int *getMultiChannelSequence();
    char getMultiChannelSequenceByte();
    char getTimeZoneByte1();
    char getTimeZoneByte2();
    char getZoneMask();
    QByteArray getlastSentCommand();
private:
    SerialOdin *serialOdin;
    SocketOdin *socketOdin;

    MODE OPMode = FLATSINGLE;

    CHANNELNUMBER ChannelNum = CHANNELA;
    CHANNELNUMBER multiChannelSeq[4] = {CHANNELA, CHANNELB, CHANNELC, CHANNELD};

    TIMEZONE zone[7] = {TIME_A, TIME_A, TIME_A, TIME_A, TIME_A, TIME_A, TIME_A};
    ZONEMASK zoneMask = MASKNONE;

    double PulseMag[5] = {2.0, 2.0, 2.0, 2.0, 2.0};
    int PulseDuration = 200;
    char PulseNum = 2;
    char interPulseDuration = 75;

    QByteArray outgoingCommand;

    quint8 testCommand[16] = {
        0xAA,
        0x55,
        0x0C,
        0x2D,
        0x25,
        0x02,
        0x35,
        0xE4,
        0x00,
        0x00,
        0x00,
        0x55,
        0x00,
        0x00,
        0x00,
        0x00
    };
};

#endif // COMMANDODIN_H

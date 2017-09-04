#ifndef STIMULATOR_H
#define STIMULATOR_H

#include"../common/qtincludes.h"

typedef enum subSequenceChoosed{
    SUBSEQ_CHOSEN     = 0B0001,
    SUBSEQ_UNCHOSEN   = 0B0000
}subSequenceChoosed;

typedef enum multiplier{
    X1      = 0B00000000,
    X2      = 0B00000100,
    X4      = 0B00000010,
    X8      = 0B00000110,
    X16     = 0B00000001,
    X32     = 0B00000101,
    X64     = 0B00000011,
    X128    = 0B00000111
}multipler;

class Stimulator
{
public:
    Stimulator();
    quint8 getMultiplier(quint8 arr[][2], int channel);

    QByteArray getSubSequence(int channelNumber);
    QByteArray getSubSequenceStart(int channelNumber);
    QByteArray getSubSequenceStop(int channelNumber);

    QByteArray getParameter();
    QByteArray getTrigger();

    void setSubSeqSelect(int index, bool state);
    void *getSubSeqSelectArray();
    void setSubSeqParam(int channel, int param);
    void *getSubSeqParamArray();

    void setSubSeqTimeStart(int index, int value);
    void setSubSeqMultipleStart(int index, int multiple);
    void setSubSeqTimeStop(int index, int value);
    void setSubSeqMultipleStop(int index, int multiple);
    void *getSubSeqStartArray();
    void *getSubSeqStopArray();

    void setStimParam(int channel, int paramNumber, int value);
    void *getStimParamArray();

private:
    bool subSeqSelect[8] = {1, 1, 1, 1, 1, 1, 1, 1};
    quint8 subSeqParam[8] = {1, 1, 1, 1, 1, 1, 1, 1};
    quint8 subSeqStart[8][2] = {{32, 0}, {32, 0}, {32, 0}, {32, 0}, {32, 0}, {32, 0}, {32, 0}, {32, 0}};
    quint8 subSeqStop[8][2] = {{32, 0}, {32, 0}, {32, 0}, {32, 0}, {32, 0}, {32, 0}, {32, 0}, {32, 0}};

    int stimParam[16][5];
};

#endif // STIMULATOR_H

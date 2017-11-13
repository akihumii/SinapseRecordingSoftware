#ifndef STIMULATOR_H
#define STIMULATOR_H

#include"../common/qtincludes.h"

typedef enum subSequenceChoosed{
    SUBSEQ_CHOSEN     = 0B0001,
    SUBSEQ_UNCHOSEN   = 0B0000
}subSequenceChoosed;

typedef enum multiplier{
    X1      = 0B00000000,
    X2      = 0B10000000,
    X4      = 0B01000000,
    X8      = 0B11000000,
    X16     = 0B00100000,
    X32     = 0B10100000,
    X64     = 0B01100000,
    X128    = 0B11100000
}multipler;

class Stimulator
{
public:
    Stimulator();
    quint8 getMultiplier(quint8 arr[][2], int channel);

    bool isStimulatorParamSetLower();
    bool isStimulatorParamSetUpper();

    QByteArray getSubSequence(int channelNumber);
    QByteArray getSubSequenceStart(int channelNumber);
    QByteArray getSubSequenceStop(int channelNumber);

    QByteArray getParameter();
    QByteArray getTriggerCmd();

    void setSubSeqSelect(int index, bool state);
    void *getSubSeqSelectArray();
    void setSubSeqParam(int channel, int param);
    void *getSubSeqParamArray();

    void setTriggerCmd(int index, bool state);

    void setSubSeqTimeStart(int index, int value);
    void setSubSeqMultipleStart(int index, int multiple);
    void setSubSeqTimeStop(int index, int value);
    void setSubSeqMultipleStop(int index, int multiple);
    void *getSubSeqStartArray();
    void *getSubSeqStopArray();

    void setStimParam(int channel, int paramOfSet, int value);
    QVector<QVector<quint8>> getStimParamArray();
    quint8 getStimParam(int set, int paramOfSet);

private:
    bool subSeqSelect[8] = {1, 1, 1, 1, 1, 1, 1, 1};
    quint8 subSeqParam[8] = {1, 1, 1, 1, 1, 1, 1, 1};
    quint8 subSeqStart[8][2] = {{32, 0}, {32, 0}, {32, 0}, {32, 0}, {32, 0}, {32, 0}, {32, 0}, {32, 0}};
    quint8 subSeqStop[8][2] = {{32, 0}, {32, 0}, {32, 0}, {32, 0}, {32, 0}, {32, 0}, {32, 0}, {32, 0}};
    QVector<QVector<quint8>> paramValue;

    int stimParam[16][5];
    bool triggerCmd[8] = {0};  //DEFXYabc

};

#endif // STIMULATOR_H

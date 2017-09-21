#ifndef COMMAND_H
#define COMMAND_H

#include "../common/qtincludes.h"

typedef enum CHANNEL_NUMBER{
    CHANNEL_0 = '0',
    CHANNEL_1 = '1',
    CHANNEL_2 = '2',
    CHANNEL_3 = '3'
} CHANNEL_NUMBER;

typedef enum PHASE_TYPE{
    PHASE_TYPE_NONE = '0',
    MONOPHASIC = '1',
    BIPHASIC = '2'
} PHASE_TYPE;

typedef enum POLARITY{
    POLARITY_NONE = '0',
    ANODIC_PULSE_FIRST = '1',
    CATHODIC_PULSE_FIRST = '2'
} POLARITY;


class Command
{
public:
    Command();
    QByteArray constructCommand();
    void setChannel(CHANNEL_NUMBER channel);
    void setPhaseType(PHASE_TYPE type);
    void setPolarity(POLARITY polarity);
    void setAnodicAmp(QString amplitude);
    void setAnodicDuration(QString duration);
    void setCathodicAmp(QString amplitude);
    void setCathodicDuration(QString duration);
    void setNumPulse(QString number);
    void setAdjDuration(QString duration);
    QString getChannel();
    QString getPhaseType();
    QString getPolarity();
    QString getAnodicAmp();
    QString getAnodicDuration();
    QString getCathodicAmp();
    QString getCathodicDuration();
    QString getNumPulse();
    QString getAdjDuration();

private:
    QString channel = "0";
    QString phaseType = "0";
    QString polarity = "0";
    QString anodicAmp = "0";
    QString anodicDuration = "0";
    QString cathodicAmp = "0";
    QString cathodicDuration = "0";
    QString numPulse = "0";
    QString adjDuration = "0";
};

#endif // COMMAND_H

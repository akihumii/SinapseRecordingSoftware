#ifndef COMMANDCAT_H
#define COMMANDCAT_H

#include "../common/qtincludes.h"

namespace Cat {

typedef enum ADDRESS{
    THRESHOLD_CHN1 = 0xCC,
    THRESHOLD_CHN2 = 0xCD,
    THRESHOLD_CHN3 = 0xCE,
    THRESHOLD_CHN4 = 0xCF,
    THRESHOLD_POWER_CHN1 = 0xD0,
    THRESHOLD_POWER_CHN2 = 0xD1,
    THRESHOLD_POWER_CHN3 = 0xD2,
    THRESHOLD_POWER_CHN4 = 0xD3,
    DECODING_WINDOW_SIZE = 0xD4,
    OVERLAP_WINDOW_SIZE = 0xD5,
    SAMPLING_FREQ = 0xD6,
    HIGHPASS_CUTOFF_FREQ = 0xD7,
    LOWPASS_CUTOFF_FREQ = 0xD8,
    NOTCH_CUTOFF_FREQ = 0xD9,
    EXTEND_STIMULATION = 0xDA,
    CLASSIFY_DIMENSION = 0xDB,
    CLOSED_LOOP = 0xDC,
    RESET_FLAG = 0xDD,
    STIMULATION_FLAG = 0xDE,
    SAVING_FLAG = 0xDF

} ADDRESS;

class CommandCat : public QObject
{
    Q_OBJECT

public:
    CommandCat();
    void setThreshold(int channel, int value);
    void setThresholdPower(int channel, int value);
    void setDecodingWindowSize(int value);
    void setOverlapWindowSize(int value);
    void setSamplingFreq(int value);
    void setExtendStimulation(int value);
    void setHighpassCutoffFreq(int value);
    void setLowpassCutoffFreq(int value);
    void setNotchCutoffFreq(int value);
    void setSMChannel(int channel, int value);
    void setStartStimulation(bool flag);
    int getThreshold(int channel);
    int getThresholdPower(int channel);
    int getDecodingWindowSize();
    int getOverlapWindowSize();
    int getSamplingFreq();
    int getExtendStimulation();
    int getHighpassCutoffFreq();
    int getLowpassCutoffFreq();
    int getNotchCutoffFreq();
    int getSMChannel();
    int getStartStimulation();
    void sendThreshold(int channel);
    void sendThresholdPower(int channel);
    void sendDecodingWindowSize();
    void sendOverlapWindowSize();
    void sendSamplingFreq();
    void sendExtendStimulation();
    void sendHighpassCutoffFreq();
    void sendLowpassCutoffFreq();
    void sendNotchCutoffFreq();
    void sendSMChannel();
    void sendStartStimulation();

    QByteArray getlastRpiCommand();
    void updateRpiCommand(char *data);

public slots:

private:
    void appendRpiCommand(short data);
    int threshold[4] = {1, 1, 1, 1};
    int thresholdPower[4] = {10, 10, 10, 10};
    int decodingWindowSize = 200;
    int overlapWindowSize = 50;
    int samplingFreq = 1250;
    int extendStimulation = 0;
    int highpassCutoffFreq = 100;
    int lowpassCutoffFreq = 0;
    int notchCutoffFreq = 50;
    int SMChannel[2] = {1, 0};
    bool startStimulationFlag = false;

    QByteArray rpiCommand;

signals:
    void appendCommandSignal(short data);

};

}

#endif // COMMANDCAT_H

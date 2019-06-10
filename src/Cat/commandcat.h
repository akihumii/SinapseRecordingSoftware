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
    THRESHOLD_POWER_CHN4 = 0xD3
} ADDRESS;

class CommandCat : public QObject
{
    Q_OBJECT

public:
    CommandCat();
    void setThreshold(int channel, int value);
    void setThresholdPower(int channel, int value);
    int getThreshold(int channel);
    int getThresholdPower(int channel);
    void sendThreshold(int channel);
    void sendThresholdPower(int channel);

    QByteArray getlastRpiCommand();

private:
    int threshold[4] = {1, 1, 1, 1};
    int thresholdPower[4] = {10, 10, 10, 10};

    QByteArray rpiCommand;
};

}

#endif // COMMANDCAT_H

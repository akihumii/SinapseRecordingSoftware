#ifndef BIOIMPEDANCE_H
#define BIOIMPEDANCE_H

#include "../common/qtincludes.h"
#include "math.h"

#define PI 3.142

typedef enum CURRENT_TYPE{
    SMALL_CURRENT = 0,
    MEDIUM_CURRENT = 1,
    LARGE_CURRENT = 2
} CURRENT_TYPE;

typedef enum GAIN{
    MEDIUM_GAIN = 80,
    HIGH_GAIN = 160,
    SUPER_HIGH_GAIN = 320
} GAIN;

typedef enum INLINEQUAD{
    INLINE = 0,
    QUADRATURE = 1
} INLINEQUAD;

class BioImpedance
{
public:
    BioImpedance(QByteArray data_, double gain_);
    BioImpedance();
    void setHighCurrentInline(int channel, bool flag);
    bool getHighCurrentInline(int channel);
    void setLowCurrentInline(int channel, bool flag);
    bool getLowCurrentInline(int channel);
    void setHighCurrentHighGainInline(int channel, bool flag);
    bool getHighCurrentHighGainInline(int channel);
    void setHighCurrentQuad(int channel, bool flag);
    bool getHighCurrentQuad(int channel);
    void setLowCurrentQuad(int channel, bool flag);
    bool getLowCurrentQuad(int channel);
    void setHighCurrentHighGainQuad(int channel, bool flag);
    bool getHighCurrentHighGainQuad(int channel);
    void setTempInline(int channel, double value);
    void setTempQuad(int channel, double value);
    void setFinalInline(int channel, double value, GAIN gainType, CURRENT_TYPE currentType);
    double getFinalInline(int channel);
    void setFinalQuad(int channel, double value, GAIN gainType, CURRENT_TYPE currentType);
    double getFinalQuad(int channel);
    double getTempInline(int channel);
    double getTempQuad(int channel);
    void setResetVoltage(char value, GAIN gain);
    double getResetVoltage(GAIN gain);
    double calculateMagnitude(int channel);
    double calculatePhase(int channel);
private:
    QFile *File;
    QString directory = QDir::homePath() + "/Desktop/";
    QTextStream *out;
    QString fileName;
    double resetVoltageArray[2];
    double resetVoltage;
    double somethingElse;
    double gain = 80.0;
    double resolution = 5.0;
    double tempInline[10] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    double tempQuad[10] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    double finalInline[10] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    double finalQuad[10] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    bool highCurrentInline[10] = {false, false, false, false, false, false, false, false, false, false};
    bool highCurrentQuad[10] = {false, false, false, false, false, false, false, false, false, false};
    bool lowCurrentInline[10] = {false, false, false, false, false, false, false, false, false, false};
    bool lowCurrentQuad[10] = {false, false, false, false, false, false, false, false, false, false};
    bool highCurrentHighGainInline[10] = {false, false, false, false, false, false, false, false, false, false};
    bool highCurrentHighGainQuad[10] = {false, false, false, false, false, false, false, false, false, false};
    QVector<QVector<QVector<double>>> sortedData;
    QVector<QVector<QVector<double>>> RX_Data;
    QVector<QVector<QVector<double>>> impedance;
    double convertVoltage(quint8 temp);
    void allocate3Darray(QVector<QVector<QVector<double> > > &array3D);
    void allocate2Darray(QVector<QVector<double>> &array2D);
    void impedanceElimination();
    bool withinRange(double value, double min, double max);
    void sortBioImpedanceData(QByteArray data_temp);
    void calculateImpedance(QVector<QVector<QVector<double>>> &data);
};

#endif // BIOIMPEDANCE_H

#ifndef BIOIMPEDANCE_H
#define BIOIMPEDANCE_H

#include "../common/qtincludes.h"
#include "math.h"

#define PI 3.142

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
    void setFinalInline(int channel, double value);
    double getFinalInline(int channel);
    void setFinalQuad(int channel, double value);
    double getFinalQuad(int channel);
    double getTempInline(int channel);
    double getTempQuad(int channel);
    void setResetVoltage(char value);
    double getResetVoltage();
private:
    QFile *File;
    QString directory = QDir::homePath() + "/Desktop/";
    QTextStream *out;
    QString fileName;
    double resetVoltage;
    double somethingElse;
    double gain = 80.0;
    double resolution = 8.0;
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
    QByteArray data;
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
    void calculateResistance(int channel, double small, double medium, double large);
    void calculateCapacitance(int channel, double small, double medium, double large);
};

#endif // BIOIMPEDANCE_H

#ifndef BIOIMPEDANCE_H
#define BIOIMPEDANCE_H

#include "../common/qtincludes.h"
#include "math.h"

#define PI 3.142

class BioImpedance
{
public:
    BioImpedance(QByteArray data_, double gain_);
private:
    double resetVoltage;
    double somethingElse;
    double gain = 80.0;
    QByteArray data;
    QVector<QVector<QVector<double>>> sortedData;
    QVector<QVector<QVector<double>>> RX_Data;
    QVector<QVector<double>> impedance;
    double convertVoltage(quint8 temp);
    void allocate3Darray(QVector<QVector<QVector<double> > > &array3D);
    void allocate2Darray(QVector<QVector<double>> &array2D);
    void impedanceElimination();
    bool withinRange(double value, double min, double max);
    void sortBioImpedanceData(QByteArray data_temp);
    void calculateResistance(int channel, double small, double medium, double large);
    void calculateCapacitance(int channel, double small, double medium, double large);
};

#endif // BIOIMPEDANCE_H

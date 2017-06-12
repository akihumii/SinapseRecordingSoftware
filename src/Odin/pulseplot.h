#ifndef PULSEPLOT_H
#define PULSEPLOT_H

#include "../common/qtincludes.h"
#include <QtMath>

class PulsePlot
{
public:
    PulsePlot();
    void setPulseDuration(int pulseDuration_);
    int getPulseDuration();
    void setAmplitude(double amplitude_);
    double getAmplitude();
    void setFrequency(int freq);
    int getFrequency();
    QVector<double> retrieveXaxis();
    QVector<double> retrieveYaxis();
    void updateYvalues();

public slots:

private:
    QVector<double> xValues, yValues;
    int xRange = 2550;
    double yRange = 20.0;
    int pulseDuration = 200;
    double amplitude = 2.0;
    int frequency;

private slots:

};

#endif // PULSEPLOT_H

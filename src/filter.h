#ifndef FILTER_H
#define FILTER_H

#include "qtincludes.h"

class Filter
{
public:
    Filter();
    bool notchFilterEnabled = false;
    bool lowpassFilterEnabled = false;
    bool highpassFilterEnabled = false;
    double highpassFreq = 100;
    double lowpassFreq = 0;
    double notchFreq = 0;
    void setNotchFilter(double cutoffFreq, double bandwidth, double sampleFreq);
    void setNotchFilterEnabled(bool enableFlag);
    bool isNotchFilterEnabled();
    double currentNotchFreq();
    void setLowpassFilter(double cutoffFreq, double sampleFreq);
    void setLowpassFilterEnabled(bool enableFlag);
    bool isLowpassFilterEnabled();
    double currentLowpassFreq();
    void setHighpassFilter(double cutoffFreq, double sampleFreq);
    void setHighpassFilterEnabled(bool enableFlag);
    bool isHighpassFilterEnabled();
    double currentHighpassFreq();
    bool isFilterEnabled();
    QVector<double> filterData(QVector<double> rawData, int ChannelIndex);

private:
    double a0_hp, a1_hp, a2_hp, b1_hp, b2_hp;        // coefficients for 2nd order low pass butterworth filter
    double a0_lp, a1_lp, a2_lp, b1_lp, b2_lp;        // coefficients for 2nd order high pass butterworth filter
    double a0_n, a1_n, a2_n, b1_n, b2_n;             // coefficients for notch filter
    double TWO_PI = 6.28318530718;
    QVector<double> filteredData[10];
    QVector<double> filteredData_hp[10];
    QVector<double> filteredData_n[10];
    QVector<double> prevRawData[10];
    QVector<double> hipassFilter(QVector<double> rawData, int ChannelIndex);
    QVector<double> lopassFilter(QVector<double> rawData, int ChannelIndex);
    QVector<double> notchFilter(QVector<double> rawData, int ChannelIndex);
};

#endif // FILTER_H

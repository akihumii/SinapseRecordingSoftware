#include "filter.h"

Filter::Filter(){

}

// Set cutoff frequency and sample frequency for calculation of butterworth low pass filter's coefficient
void Filter::setLowpassFilter(double cutoffFreq, double sampleFreq){
    qDebug() << "Setting Low pass filter";
    lowpassFreq = cutoffFreq;
    double fr = sampleFreq/cutoffFreq;
    double omega = qTan(M_PI/fr);
    double c = 1 + qCos(M_PI/4) * omega + (omega * omega);
    a0_lp = (omega * omega) / c;
    a2_lp = a0_lp;
    a1_lp = a0_lp * 2;
    b1_lp = 2 * (omega * omega - 1) / c;
    b2_lp = (1 - qCos(M_PI/4) * omega + (omega * omega)) / c;
}

// Set notch filter parameters.  All filter parameters are given in Hz (or
// in Samples/s).  A bandwidth of 10 Hz is recommended for 50 or 60 Hz notch
// filters.  Narrower bandwidths will produce extended ringing in the time
// domain in response to large transients.
void Filter::setNotchFilter(double cutoffFreq, double bandwidth, double sampleFreq){
    qDebug() << "Setting Notch filter";
    notchFreq = cutoffFreq;
    double d = qExp(-M_PI * bandwidth / sampleFreq);

    // Calculate biquad IIR filter coefficients.
    b1_n = -(1.0 + d * d) * qCos(2.0 * M_PI * cutoffFreq / sampleFreq);
    b2_n = d * d;
    a0_n = (1 + d * d) / 2.0;
    a1_n = b1_n;
    a2_n = a0_n;
}

// Set cutoff frequency and sample frequency for calculation of butterworth high pass filter's coefficient
void Filter::setHighpassFilter(double cutoffFreq, double sampleFreq){
    qDebug() << "Setting High pass filter";
    highpassFreq = cutoffFreq;
    cutoffFreq = 0.5*sampleFreq - cutoffFreq;
    double fr = sampleFreq/cutoffFreq;
    double omega = qTan(M_PI/fr);
    double c = 1 + qCos(M_PI/4) * omega + (omega * omega);
    a0_hp = (omega * omega) / c;
    a2_hp = a0_hp;
    a1_hp = a0_hp * -2;
    b1_hp = -2 * (omega * omega - 1) / c;
    b2_hp = (1 - qCos(M_PI/4) * omega + (omega * omega)) / c;
}

void Filter::setLowpassFilterEnabled(bool enableFlag){
    lowpassFilterEnabled = enableFlag;
    if(lowpassFilterEnabled){
        qDebug() << "Low pass filter disabled";
    }
    else{
        qDebug() << "Low pass filter disabled";
    }
}

void Filter::setNotchFilterEnabled(bool enableFlag){
    notchFilterEnabled = enableFlag;
    if(notchFilterEnabled){
        qDebug() << "Notch filter enabled";
    }
    else{
        qDebug() << "Notch filter disabled";
    }
}

void Filter::setHighpassFilterEnabled(bool enableFlag){
    highpassFilterEnabled = enableFlag;
    if(highpassFilterEnabled){
        qDebug() << "High pass filter enabled";
    }
    else{
        qDebug() << "High pass filter disabled";
    }
}

bool Filter::isHighpassFilterEnabled(){
    return highpassFilterEnabled;
}

bool Filter::isLowpassFilterEnabled(){
    return lowpassFilterEnabled;
}

bool Filter::isNotchFilterEnabled(){
    return notchFilterEnabled;
}

bool Filter::isFilterEnabled(){
    return highpassFilterEnabled || lowpassFilterEnabled || notchFilterEnabled;
}

double Filter::currentLowpassFreq(){
    return lowpassFreq;
}

double Filter::currentHighpassFreq(){
    return highpassFreq;
}

double Filter::currentNotchFreq(){
    return notchFreq;
}

QVector<double> Filter::filterData(QVector<double> rawData){
    if(prevRawData.size() >= 2){
        rawData.prepend(prevRawData.at(0));
        rawData.prepend(prevRawData.at(1));
        prevRawData.clear();
    }
    prevRawData.append(rawData.at(rawData.size()-1));
    prevRawData.append(rawData.at(rawData.size()-2));
    if(notchFilterEnabled){
        rawData = notchFilter(rawData);
    }
    if(highpassFilterEnabled){
        rawData = hipassFilter(rawData);
    }
    if(lowpassFilterEnabled){
        rawData = lopassFilter(rawData);
    }
    return rawData;
}

QVector<double> Filter::hipassFilter(QVector<double> rawData){
    double temp;
    if(filteredData_hp.size() > 1){
        filteredData_hp.remove(0, filteredData_hp.size()-2);
    }
    else{
        filteredData_hp.append(rawData.at(0));
        filteredData_hp.append(rawData.at(1));
    }
    for (int t = 2; t < rawData.size(); t++) {
        temp = a0_hp * rawData.at(t) + a1_hp * rawData.at(t-1) + a2_hp * rawData.at(t-2) - b1_hp * filteredData_hp.at(t-1) - b2_hp * filteredData_hp.at(t-2);
        filteredData_hp.append(temp);
    }
    return filteredData_hp;
}

QVector<double> Filter::lopassFilter(QVector<double> rawData){
    double temp;
    if(filteredData.size() > 1){
        filteredData.remove(0, filteredData.size()-2);
    }
    else{
        filteredData.append(rawData.at(0));
        filteredData.append(rawData.at(1));
    }
    for (int t = 2; t < rawData.size(); t++) {
        temp = a0_lp * rawData.at(t) + a1_lp * rawData.at(t-1) + a2_lp * rawData.at(t-2) - b1_lp * filteredData.at(t-1) - b2_lp * filteredData.at(t-2);
        filteredData.append(temp);
    }
    return filteredData;
}

QVector<double> Filter::notchFilter(QVector<double> rawData){
    double temp;
    if(filteredData_n.size() > 1){
        filteredData_n.remove(0, filteredData_n.size()-2);
    }
    else{
        filteredData_n.append(rawData.at(0));
        filteredData_n.append(rawData.at(1));
    }
    for (int t = 2; t < rawData.size(); t++) {
        temp = a0_n * rawData.at(t) + a1_n * rawData.at(t-1) + a2_n * rawData.at(t-2) - b1_n * filteredData_n.at(t-1) - b2_n * filteredData_n.at(t-2);
        filteredData_n.append(temp);
    }
    return filteredData_n;
}

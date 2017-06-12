#include "pulseplot.h"

PulsePlot::PulsePlot()
{
    xValues.reserve(2550);
    yValues.reserve(2550);

    for(int i = 0; i < 2550; i ++){
        xValues.append(i*0.000001);
//        qDebug() << xValues.at(i);
    }
}

void PulsePlot::setPulseDuration(int pulseDuration_){
    pulseDuration = pulseDuration_;
}

int PulsePlot::getPulseDuration(){
    return pulseDuration;
}

void PulsePlot::setAmplitude(double amplitude_){
    amplitude = amplitude_;
}

double PulsePlot::getAmplitude(){
    return amplitude;
}

void PulsePlot::setFrequency(int freq){
    frequency = freq;
}

int PulsePlot::getFrequency(){
    return frequency;
}

QVector<double> PulsePlot::retrieveXaxis(){
    return xValues;
}

QVector<double> PulsePlot::retrieveYaxis(){
    return yValues;
}

void PulsePlot::updateYvalues(){
    for(int i = 0; i < 2550; i++){
        yValues.append(0);
    }

    for(int i = 1264; i < 1286; i++){
        yValues.replace(i, (double) 0.0);
    }

    for(int i = 1263; i > (1263-pulseDuration); i--){
        yValues.replace(i, (double) amplitude);
    }

    for(int i = 1286; i < (1286+pulseDuration); i++){
        yValues.replace(i, (double) -amplitude);
    }

    for(int i = (1263-pulseDuration); i > 0; i--){
        yValues.replace(i, (double) 0.0);
    }

    for(int i = (1286+pulseDuration); i < 2550; i++){
        yValues.replace(i, (double) 0.0);
    }

    for(int i = 0; i < 2550; i ++){
//        qDebug() << yValues.at(i);
    }
}

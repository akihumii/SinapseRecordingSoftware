#include "bioimpedance.h"

BioImpedance::BioImpedance(QByteArray data_){
    allocate3Darray(sortedData);
    allocate3Darray(RX_Data);
    if(data_.size() == 62){
        sortBioImpedanceData(data_);
    }
    else{
        qDebug() << "Measurement incomplete!";
    }
}

void BioImpedance::allocate3Darray(QVector<QVector<QVector<double>>> &array3D){
    array3D.resize(10);
    for(int j = 0; j < 10; j++){
        array3D[j].resize(2);
        for(int k = 0; k < 2; k++){
            array3D[j][k].resize(3);
        }
    }
}

void BioImpedance::sortBioImpedanceData(QByteArray data_temp){
    resetVoltage = convertVoltage((quint8) data_temp.at(0));
    qDebug() << "Reset Voltage is " << resetVoltage;

    somethingElse = convertVoltage((quint8) data_temp.at(1));
    qDebug() << "Something Else is " << somethingElse;

    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 2; j++){
            for(int k = 0; k < 3; k++){
                sortedData[i][j][k] = convertVoltage((quint8) data_temp.at(i * 6 + j * 3 + k + 2));
                qDebug() << "Current in iteration : " << i * 6 + j * 3 + k + 2 << "And it has a value of " << sortedData[i][j][k];
            }
            if(j%2 == 0){
                calculateResistance(i, sortedData.at(i).at(j).at(0), sortedData.at(i).at(j).at(1), sortedData.at(i).at(j).at(2));
            }
            else{
                calculateCapacitance(i, sortedData.at(i).at(j).at(0), sortedData.at(i).at(j).at(1), sortedData.at(i).at(j).at(2));
            }
        }
    }
}

double BioImpedance::convertVoltage(quint8 temp){
    return ((temp / 255.0) * 1.2);
}

void BioImpedance::calculateResistance(int channel, double small, double medium, double large){
    double temp;
    temp = ((small - resetVoltage) * PI * PI) / (3.232 * 8 * 0.00000000109 * gain);
//    temp = ((small - resetVoltage) * PI * PI) / (3.232 * 0.0000000011 * gain);
    RX_Data[channel][0][0] = temp;
    qDebug() << "Channel " << channel << "small (R) : " << temp;
    temp = ((medium - resetVoltage) * PI * PI) / (3.232 * 8 * 0.0000000127 * gain);
//    temp = ((medium - resetVoltage) * PI * PI) / (3.232 * 0.0000000111 * gain);
    RX_Data[channel][0][1] = temp;
    qDebug() << "Channel " << channel << "medium (R) : " << temp;
    temp = ((large - resetVoltage) * PI * PI) / (3.232 * 8 * 0.000000136 * gain);
//    temp = ((large - resetVoltage) * PI * PI) / (3.232 * 0.0000001111 * gain);
    RX_Data[channel][0][2] = temp;
    qDebug() << "Channel " << channel << "large (R) : " << temp;
}

void BioImpedance::calculateCapacitance(int channel, double small, double medium, double large){
    double temp;
    temp = 3.232 * ((8 * 0.0000000011 * gain) / (PI * PI * (small - resetVoltage)));
    RX_Data[channel][1][0] = temp;
    qDebug() << "Channel " << channel << "small (C) : " << temp;
    temp = 3.232 * ((8 * 0.0000000111 * gain) / (PI * PI * (medium - resetVoltage)));
    RX_Data[channel][1][1] = temp;
    qDebug() << "Channel " << channel << "medium (C) : " << temp;
    temp = 3.232 * ((8 * 0.0000001111 * gain) / (PI * PI * (large - resetVoltage)));
    qDebug() << "Channel " << channel << "large (C) : " << temp;
    RX_Data[channel][1][2] = temp;
}

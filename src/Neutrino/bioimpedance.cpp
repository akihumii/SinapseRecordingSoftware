#include "bioimpedance.h"

BioImpedance::BioImpedance(QByteArray data_, double gain_){
    allocate3Darray(sortedData);
    allocate3Darray(RX_Data);
    allocate3Darray(impedance);
    gain = gain_;
    File = new QFile;
    fileName = directory + QDateTime::currentDateTime().toString("'impedance_'yyyyMMdd_HHmmss'.txt'");
    File->setFileName(fileName);
    if(File->open(QIODevice::WriteOnly|QIODevice::Text)){
        qDebug()<< "File opened";
    }
    else{
        qDebug() << "File failed to open";
    }
    out = new QTextStream(File);
    if(data_.size() == 82){
        sortBioImpedanceData(data_);
        impedanceElimination();
        if(File->isOpen()){
            File->close();
        }
    }
    else{
        qDebug() << "Measurement incomplete!";
    }
}

void BioImpedance::allocate2Darray(QVector<QVector<double>> &array2D){
    array2D.resize(10);
    for(int i = 0; i < 10; i++){
        array2D[i].resize(2);
    }
}

void BioImpedance::allocate3Darray(QVector<QVector<QVector<double>>> &array3D){
    array3D.resize(10);
    for(int j = 0; j < 10; j++){
        array3D[j].resize(4);
        for(int k = 0; k < 4; k++){
            array3D[j][k].resize(2);
        }
    }
}

void BioImpedance::sortBioImpedanceData(QByteArray data_temp){
    resetVoltage = convertVoltage((quint8) data_temp.at(0));
    qDebug() << "Reset Voltage is " << resetVoltage;
    *out << "Reset Voltage is " << resetVoltage << "\n";
    *out << "Gain is set at : " << gain << "\n";

    somethingElse = convertVoltage((quint8) data_temp.at(1));
    qDebug() << "Something Else is " << somethingElse;

    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 4; j++){
            for(int k = 0; k < 2; k++){
                sortedData[i][j][k] = convertVoltage((quint8) data_temp.at(i * 8 + j * 2 + k + 2));
                qDebug() << "Sorted Data at X: " << i << "Y: " << j << "Z: "<< k << sortedData[i][j][k];
            }
//            if(j%2 == 0){
//                calculateResistance(i, sortedData.at(i).at(j).at(0), sortedData.at(i).at(j).at(1), sortedData.at(i).at(j).at(2));
//            }
//            else{
//                calculateCapacitance(i, sortedData.at(i).at(j).at(0), sortedData.at(i).at(j).at(1), sortedData.at(i).at(j).at(2));
//            }
        }
    }
    calculateImpedance(sortedData);
}

void BioImpedance::calculateImpedance(QVector<QVector<QVector<double>>> &data){
    for(int i = 0; i < 10; i++){
        if( (double) ((data[i][0][0] - resetVoltage) * PI * PI) / (3.232 * 8 * 0.00000000109 * gain) > 0){
            RX_Data[i][0][0] = (double) ((data[i][0][0] - resetVoltage) * PI * PI) / (3.232 * 8 * 0.00000000109 * gain);
        }
        else{
            RX_Data[i][0][0] = 0.0;
        }
        if( (double) ((data[i][1][0] - resetVoltage) * PI * PI) / (3.232 * 8 * 0.00000000109 * gain) > 0){
            RX_Data[i][1][0] = (double) ((data[i][1][0] - resetVoltage) * PI * PI) / (3.232 * 8 * 0.0000000127 * gain);
        }
        else{
            RX_Data[i][1][0] = 0.0;
        }
        if( (double) ((data[i][2][0] - resetVoltage) * PI * PI) / (3.232 * 8 * 0.00000000109 * gain) > 0){
            RX_Data[i][2][0] = (double) ((data[i][2][0] - resetVoltage) * PI * PI) / (3.232 * 8 * 0.000000136 * gain);
        }
        else{
            RX_Data[i][2][0] = 0.0;
        }

        RX_Data[i][0][1] = (double) 1.0 / (3.232 * ((8 * 0.0000000011 * gain) / (PI * PI * (data[i][0][1] - resetVoltage))));
        RX_Data[i][1][1] = (double) 1.0 / (3.232 * ((8 * 0.0000000111 * gain) / (PI * PI * (data[i][1][1] - resetVoltage))));
        RX_Data[i][2][1] = (double) 1.0 / (3.232 * ((8 * 0.0000001111 * gain) / (PI * PI * (data[i][2][1] - resetVoltage))));

        qDebug() << "Channel " << i+1 << "'s small current (INLINE) : " << RX_Data[i][0][0] << " | Voltage: " << data[i][0][0];
        qDebug() << "Channel " << i+1 << "'s small current (QUADRATURE) : " << RX_Data[i][0][1] << " | Voltage: " << data[i][0][1];

        qDebug() << "Channel " << i+1 << "'s medium current (INLINE) : " << RX_Data[i][1][0] << " | Voltage: " << data[i][1][0];
        qDebug() << "Channel " << i+1 << "'s medium current (QUADRATURE) : " << RX_Data[i][1][1] << " | Voltage: " << data[i][1][1];

        qDebug() << "Channel " << i+1 << "'s large current (INLINE) : " << RX_Data[i][2][0] << " | Voltage: " << data[i][2][0];
        qDebug() << "Channel " << i+1 << "'s large current (QUADRATURE) : " << RX_Data[i][2][1] << " | Voltage: " << data[i][2][1];

        *out << "Channel " << i+1 << "'s small current (INLINE) : " << RX_Data[i][0][0] << " | Voltage: " << data[i][0][0] << "\n";
        *out << "Channel " << i+1 << "'s small current (QUADRATURE) : " << RX_Data[i][0][1] << " | Voltage: " << data[i][0][1] << "\n";

        *out << "Channel " << i+1 << "'s medium current (INLINE) : " << RX_Data[i][1][0] << " | Voltage: " << data[i][1][0] << "\n";
        *out << "Channel " << i+1 << "'s medium current (QUADRATURE) : " << RX_Data[i][1][1] << " | Voltage: " << data[i][1][1] << "\n";

        *out << "Channel " << i+1 << "'s large current (INLINE) : " << RX_Data[i][2][0] << " | Voltage: " << data[i][2][0] << "\n";
        *out << "Channel " << i+1 << "'s large current (QUADRATURE) : " << RX_Data[i][2][1] << " | Voltage: " << data[i][2][1] << "\n \n";
    }
}

void BioImpedance::impedanceElimination(){
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 3; j++){
            for(int k = 0; k < 2; k++){
//                if(sortedData[i][j][k] == 1.2 || sortedData[i][j][k] == 0.0){
//                    impedance[i][j][k] = 0.0;
//                    if(k == 0){
//                        qDebug() << "Channel " << i+1 << "Magnitude: " << impedance[i][j][k];
//                    }
//                    else{
//                        qDebug() << "Channel " << i+1 << "Phase angle: " << impedance[i][j][k];
//                    }
//                }
//                else{
                    if(k == 0){
                        impedance[i][j][k] = sqrt(pow(RX_Data[i][j][0], 2.0) + pow(RX_Data[i][j][1], 2.0));
                        qDebug() << "Channel " << i+1 << "Magnitude: " << impedance[i][j][k];
                        *out << "Channel " << i+1 << " Magnitude: " << impedance[i][j][k] << "\n";
                    }
                    else{
                        impedance[i][j][k] = atan(RX_Data[i][j][1] / RX_Data[i][j][0]) * 180.0 / PI;
                        qDebug() << "Channel " << i+1 << "Phase angle: " << impedance[i][j][k];
                        *out << "Channel " << i+1 << " Phase angle: " << impedance[i][j][k] << "\n";
                    }
//                }
            }
        }
        *out << "\n";
    }
}

bool BioImpedance::withinRange(double value, double min, double max){
    return ((value > min) && (value < max));
}

double BioImpedance::convertVoltage(quint8 temp){
    return ((temp / 255.0) * 1.2);
}

void BioImpedance::calculateResistance(int channel, double small, double medium, double large){
    if( (double) ((small - resetVoltage) * PI * PI) / (3.232 * 8 * 0.00000000109 * gain) > 0){
        RX_Data[channel][0][0] = (double) ((small - resetVoltage) * PI * PI) / (3.232 * 8 * 0.00000000109 * gain);
    }
    else{
        RX_Data[channel][0][0] = 0.0;
    }
    if( (double) ((medium - resetVoltage) * PI * PI) / (3.232 * 8 * 0.00000000109 * gain) > 0){
        RX_Data[channel][0][1] = (double) ((medium - resetVoltage) * PI * PI) / (3.232 * 8 * 0.0000000127 * gain);
    }
    else{
        RX_Data[channel][0][1] = 0.0;
    }
    if( (double) ((large - resetVoltage) * PI * PI) / (3.232 * 8 * 0.00000000109 * gain) > 0){
        RX_Data[channel][0][2] = (double) ((large - resetVoltage) * PI * PI) / (3.232 * 8 * 0.000000136 * gain);
    }
    else{
        RX_Data[channel][0][2] = 0.0;
    }
    qDebug() << "Channel " << channel+1 << "'s small current (RESISTANCE) : " << RX_Data[channel][0][0] << " | Voltage: " << small;
    qDebug() << "Channel " << channel+1 << "'s medium current (RESISTANCE) : " << RX_Data[channel][0][1] << " | Voltage: " << medium;
    qDebug() << "Channel " << channel+1 << "'s large current (RESISTANCE) : " << RX_Data[channel][0][2] << " | Voltage: " << large;
}

void BioImpedance::calculateCapacitance(int channel, double small, double medium, double large){
    RX_Data[channel][1][0] = (double) 1.0 / (3.232 * ((8 * 0.0000000011 * gain) / (PI * PI * (small - resetVoltage))));
    RX_Data[channel][1][1] = (double) 1.0 / (3.232 * ((8 * 0.0000000111 * gain) / (PI * PI * (medium - resetVoltage))));
    RX_Data[channel][1][2] = (double) 1.0 / (3.232 * ((8 * 0.0000001111 * gain) / (PI * PI * (large - resetVoltage))));
    qDebug() << "Channel " << channel+1 << "'s small current (CAPACITANCE) : " << RX_Data[channel][1][0] << " | Voltage: " << small;
    qDebug() << "Channel " << channel+1 << "'s medium current (CAPACITANCE) : " << RX_Data[channel][1][1] << " | Voltage: " << medium;
    qDebug() << "Channel " << channel+1 << "'s large current (CAPACITANCE) : " << RX_Data[channel][1][2] << " | Voltage: " << large;
}

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

BioImpedance::BioImpedance(){
    allocate3Darray(sortedData);
    allocate3Darray(RX_Data);
    allocate3Darray(impedance);
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

void BioImpedance::setResetVoltage(char value, GAIN gain){
    if(gain == MEDIUM_GAIN)
        resetVoltageArray[0] = convertVoltage((quint8) value);
    else if(gain == SUPER_HIGH_GAIN)
        resetVoltageArray[1] = convertVoltage((quint8) value);
}

double BioImpedance::getResetVoltage(GAIN gain){
    if(gain == MEDIUM_GAIN)
        return resetVoltageArray[0];
    else if(gain == SUPER_HIGH_GAIN)
        return resetVoltageArray[1];
}

void BioImpedance::setFinalInline(int channel, double value, GAIN gainType, CURRENT_TYPE currentType){
    double current;
    switch (currentType){
        case SMALL_CURRENT:{
            current = 0.00000000109;
            break;
        }
        case MEDIUM_CURRENT:{
            current = 0.0000000127;
            break;
        }
        case LARGE_CURRENT:{
            current = 0.000000136;
            break;
        }
    }

    double tempReset;
    switch (gainType){
        case MEDIUM_GAIN:{
            tempReset = resetVoltageArray[0];
            break;
        }
        case SUPER_HIGH_GAIN:{
            tempReset = resetVoltageArray[1];
            break;
        }
    }

    finalInline[channel] = (double) ((value - tempReset) * PI * PI) / (3.232 * 8 * current * (double) gainType);
    qDebug() << "Final Inline Value for Channel " << channel + 1 << " is: " << finalInline[channel] << " | Voltage :" << value;
}

double BioImpedance::calculateMagnitude(int channel){
    return sqrt(pow(finalInline[channel], 2.0) + pow(finalQuad[channel], 2.0));
}

double BioImpedance::calculatePhase(int channel){
    return (atan(finalQuad[channel] / finalInline[channel]) * 180.0 / PI);
}

double BioImpedance::getFinalInline(int channel){
    return finalInline[channel];
}

void BioImpedance::setFinalQuad(int channel, double value, GAIN gainType, CURRENT_TYPE currentType){
    double current;
    switch (currentType){
        case SMALL_CURRENT:{
            current = 0.0000000011;
            break;
        }
        case MEDIUM_CURRENT:{
            current = 0.0000000111;
            break;
        }
        case LARGE_CURRENT:{
            current = 0.0000001111;
            break;
        }
    }

    double tempReset;
    switch (gainType){
        case MEDIUM_GAIN:{
            tempReset = resetVoltageArray[0];
            break;
        }
        case SUPER_HIGH_GAIN:{
            tempReset = resetVoltageArray[1];
            break;
        }
        default:{
            tempReset = resetVoltageArray[0];
            break;
        }
    }

    finalQuad[channel] = (double) 1.0 / (3.232 * ((8 * current * (double) gainType) / (PI * PI * (value - tempReset))));
    qDebug() << "Final Quad Value for Channel " << channel + 1 << " is: " << finalQuad[channel] << " | Voltage :" << value;
}

double BioImpedance::getFinalQuad(int channel){
    return finalQuad[channel];
}

void BioImpedance::setHighCurrentInline(int channel, bool flag){
    highCurrentInline[channel] = flag;
}

bool BioImpedance::getHighCurrentInline(int channel){
    return highCurrentInline[channel];
}

void BioImpedance::setLowCurrentInline(int channel, bool flag){
    lowCurrentInline[channel] = flag;
}

bool BioImpedance::getLowCurrentInline(int channel){
    return lowCurrentInline[channel];
}

void BioImpedance::setHighCurrentHighGainInline(int channel, bool flag){
    highCurrentHighGainInline[channel] = flag;
}

bool BioImpedance::getHighCurrentHighGainInline(int channel){
    return highCurrentHighGainInline[channel];
}

void BioImpedance::setHighCurrentQuad(int channel, bool flag){
    highCurrentQuad[channel] = flag;
}

bool BioImpedance::getHighCurrentQuad(int channel){
    return highCurrentQuad[channel];
}

void BioImpedance::setLowCurrentQuad(int channel, bool flag){
    lowCurrentQuad[channel] = flag;
}

bool BioImpedance::getLowCurrentQuad(int channel){
    return lowCurrentQuad[channel];
}

void BioImpedance::setHighCurrentHighGainQuad(int channel, bool flag){
    highCurrentHighGainQuad[channel] = flag;
}

bool BioImpedance::getHighCurrentHighGainQuad(int channel){
    return highCurrentHighGainQuad[channel];
}

void BioImpedance::setTempInline(int channel, double value){
    tempInline[channel] = value;
}

void BioImpedance::setTempQuad(int channel, double value){
    tempQuad[channel] = value;
}

double BioImpedance::getTempInline(int channel){
    return tempInline[channel];
}

double BioImpedance::getTempQuad(int channel){
    return tempQuad[channel];
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
        if( (double) ((data[i][1][0] - resetVoltage) * PI * PI) / (3.232 * 8 * 0.0000000127 * gain) > 0){
            RX_Data[i][1][0] = (double) ((data[i][1][0] - resetVoltage) * PI * PI) / (3.232 * 8 * 0.0000000127 * gain);
        }
        else{
            RX_Data[i][1][0] = 0.0;
        }
        if( (double) ((data[i][2][0] - resetVoltage) * PI * PI) / (3.232 * 8 * 0.000000136 * gain) > 0){
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
            }
        }
        *out << "\n";
    }
}

double BioImpedance::convertVoltage(quint8 temp){
    return ((temp / 255.0) * 1.2);
}

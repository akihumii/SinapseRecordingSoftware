#include "dataprocessor.h"

DataProcessor::DataProcessor(){
}

void DataProcessor::setBitMode(bool BitMode){
    is8BitMode = BitMode;
}

void DataProcessor::parseFrameMarkers(QByteArray rawData){
    qDebug() << rawData.size();
    for(int i = 0; i < rawData.size(); i = i + 21){
        for(int j = 2; j < 10; j++){
            fullWord_rawData = ((quint8) rawData.at(i+((2*j))) << 8 | (quint8) rawData.at(i+((2*j)+1)))-32768;
            if(RecordEnabled){
                RecordData(fullWord_rawData);
            }
            appendAudioBuffer(j-2, rawData.at(i+(2*j)+1), rawData.at(i+(2*j)));
            ChannelData[j-2].append(fullWord_rawData*(0.000000195));
        }
        for(int j = 0; j < 2; j++){
            fullWord_rawData = ((quint8) rawData.at(i+((2*j))) << 8 | (quint8) rawData.at(i+((2*j)+1)))-32768;
            appendAudioBuffer(j+8, rawData.at(i+(2*j)+1), rawData.at(i+(2*j)));
            if(RecordEnabled){
                RecordData(fullWord_rawData);
            }
            ChannelData[j+8].append(fullWord_rawData*(0.000000195));
        }
        total_data_count++;
        X_axis.append(total_data_count*0.000202);
        ChannelData[11].append((quint8) rawData.at(i+20));
        if(RecordEnabled){
            RecordData((quint8)rawData.at(i+20));
            RecordData(END_OF_LINE);
        }
    }
    playAudio(getAudioChannel());
}

bool DataProcessor::checkNextFrameMarker(QByteArray data, int currentIndex){
    if(((quint8) data.at(currentIndex + 21) == (quint8) data.at(currentIndex) + 1
        && (quint8) data.at(currentIndex + 42) == (quint8) data.at(currentIndex + 21) + 1
        && (quint8) data.at(currentIndex + 63) == (quint8) data.at(currentIndex + 42) + 1
        && (quint8) data.at(currentIndex + 84) == (quint8) data.at(currentIndex + 63) + 1
        && (quint8) data.at(currentIndex + 105) == (quint8) data.at(currentIndex + 84) + 1)){
        return true;
    }
    else{
        return false;
    }
}

int DataProcessor::findfirstFrameMarkers(QByteArray rawData){
    for(int i = 0; i < rawData.size()-42; i++){
        if((quint8)rawData.at(i+42) == (quint8)rawData.at(i+21)+1
                && (quint8)rawData.at(i+21) == (quint8)rawData.at(i)+1){
            return i;
        }
        else if((quint8)rawData.at(i+42) == 1 && (quint8)rawData.at(i+21) == 0 && (quint8)rawData.at(i) == 250){
            return i;
        }
        else if((quint8)rawData.at(i+42) == 0 && (quint8)rawData.at(i+21) == 250 && (quint8)rawData.at(i) == 249){
            return i;
        }
    }
    return 0;
}

int DataProcessor::findlastFrameMarkers(QByteArray rawData){
    if(rawData.size()>40){
        for(int i = rawData.size()-1; i > 41; i--){
            if((quint8)rawData.at(i-21)+1 == (quint8)rawData.at(i) && (quint8)rawData.at(i-42)+1 == (quint8)rawData.at(i-21)){
                return i;
            }
            else if((quint8)rawData.at(i-42) == 249 && (quint8)rawData.at(i-21) == 250 && (quint8)rawData.at(i) == 0){
                return i;
            }
            else if((quint8)rawData.at(i-42) == 250 && (quint8)rawData.at(i-21) == 0 && (quint8)rawData.at(i) == 1){
                return i;
            }
        }
    }
    return 0;
}

void DataProcessor::sortADCData(QByteArray adcData){
    for(int i = 0; i < adcData.size(); i++){
        audioBuffer[10].append(adcData.at(i));
        ADC_Data.append(adcData.at(i));
    }
}

void DataProcessor::setADCRecordEnabled(bool enableFlag){
    ADCRecordEnabled = enableFlag;
    if(enableFlag){
        fileName = directory + QDateTime::currentDateTime().toString("'data_'yyyyMMdd_HHmmss'ADC.csv'");
        File = new QFile;
        File->setFileName(fileName);
        if(File->open(QIODevice::WriteOnly|QIODevice::Text)){
            qDebug()<< "File ADC opened";
        }
        else{
            qDebug() << "File ADC failed to open";
        }
        out = new QTextStream(File);
        qDebug() << "setting Record Enabled";
    }
    else{
        File->close();
        qDebug() << "setting Record disabled";
    }
}

bool DataProcessor::isADCRecordEnabled(){
    return ADCRecordEnabled;
}

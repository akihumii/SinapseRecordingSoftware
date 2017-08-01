#include "dataprocessor.h"

namespace SylphX {

DataProcessor::DataProcessor(float samplingRate_){
    samplingRate = samplingRate_;
    period = 1/samplingRate_;
}

void DataProcessor::parseFrameMarkers(QByteArray rawData){
    qDebug() << rawData.size();
    for(int i = 0; i < rawData.size(); i = i + 22){
        for(int j = 2; j < 10; j++){
            fullWord_rawData = ((quint8) rawData.at(i+1+((2*j))) << 8 | (quint8) rawData.at(i+1+((2*j)+1)))-32768;
            if(RecordEnabled){
                RecordData(fullWord_rawData);
            }
            appendAudioBuffer(j-2, rawData.at(i+1+(2*j)+1), rawData.at(i+1+(2*j)));
            ChannelData[j-2].append(fullWord_rawData*(0.000000195));
        }
        for(int j = 0; j < 2; j++){
            fullWord_rawData = ((quint8) rawData.at(i+1+((2*j))) << 8 | (quint8) rawData.at(i+1+((2*j)+1)))-32768;
            appendAudioBuffer(j+8, rawData.at(i+1+(2*j)+1), rawData.at(i+1+(2*j)));
            if(RecordEnabled){
                RecordData(fullWord_rawData);
            }
            ChannelData[j+8].append(fullWord_rawData*(0.000000195));
        }
//        if(ADC_Data.size()>5){
//            ChannelData[10].append(ADC_Data.at(0)/ 256.0 * 2.5);
//            if(RecordEnabled){
//                for(int i = 0; i < 5; i++){
//                    *out << ADC_Data.at(i) << " , \n";
//                }
//            }
//            ADC_Data.remove(0, 6);
//        }
        ChannelData[10].append((quint8) rawData.at(i+22));
        total_data_count++;
        X_axis.append(total_data_count*period);
        ChannelData[11].append((quint8) rawData.at(i+21));
        if(RecordEnabled){
            RecordData((quint8)rawData.at(i+20));
            RecordData(END_OF_LINE);
        }
    }
//    playAudio(getAudioChannel());
}

bool DataProcessor::checkNextFrameMarker(QByteArray data, int currentIndex){
    if(((quint8) data.at(currentIndex + 22) == (quint8) data.at(currentIndex) + 1
        && (quint8) data.at(currentIndex + 44) == (quint8) data.at(currentIndex + 22) + 1
        && (quint8) data.at(currentIndex + 66) == (quint8) data.at(currentIndex + 44) + 1
        && (quint8) data.at(currentIndex + 88) == (quint8) data.at(currentIndex + 66) + 1
        /*&& (quint8) data.at(currentIndex + 110) == (quint8) data.at(currentIndex + 88) + 1*/)){
        return true;
    }
    else{
        return false;
    }
}

void DataProcessor::sortADCData(QByteArray adcData){
    for(int i = 0; i < adcData.size(); i++){
        audioBuffer[10].append(adcData.at(i));
        ADC_Data.append(adcData.at(i));
    }
}

void DataProcessor::appendSync(){
    qDebug() << "Sync pulse detected!";
//    ChannelData[10].removeLast();
//    ChannelData[10].append(1);
//    if(RecordEnabled){
//        RecordData(1);
//    }
    syncPulse = 1;
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

}

#include "dataprocessor.h"

DataProcessor::DataProcessor(float samplingRate_){
    samplingRate = samplingRate_;
    period = 1/samplingRate_;
}

void DataProcessor::parseFrameMarkers(QByteArray rawData){
//    qDebug() << rawData.size();
    for(int i = 0; i < rawData.size(); i = i + 7){
        fullWord_rawData = ((quint8) rawData.at(i) << 8 | (quint8) rawData.at(i+1))-32768;
//        appendAudioBuffer(0, rawData.at(i+2), rawData.at(i+1));
        if(RecordEnabled){
            RecordData(fullWord_rawData);
        }
        ChannelData[0].append(fullWord_rawData*(0.000000195));

        fullWord_rawData = ((quint8) rawData.at(i+2) << 8 | (quint8) rawData.at(i+3))-32768;
//        appendAudioBuffer(1, rawData.at(i+4), rawData.at(i+3));
        if(RecordEnabled){
            RecordData(fullWord_rawData);
        }
        for(int j = 0; j < 8; j++){
            if(RecordEnabled){
                RecordData(fullWord_rawData);
            }
        }
        ChannelData[1].append(fullWord_rawData*(0.000000195));
        if(RecordEnabled){
            RecordData((quint8) rawData.at(i+4));
        }
//        if(ADC_Data.size()>0){
//            ChannelData[2].append(ADC_Data.at(0)/ 256.0 * 2.5);
        ChannelData[2].append((quint8) rawData.at(i+4));
//            ADC_Data.remove(0, 1);
//        }
        ChannelData[3].append((quint8) rawData.at(i+5) << 8 | (quint8) rawData.at(i+6));
        if(RecordEnabled){
            RecordData((quint8) rawData.at(i+5) << 8 | (quint8) rawData.at(i+6));
            RecordData(END_OF_LINE);
        }
        total_data_count++;
        X_axis.append(total_data_count*period);
    }
//    playAudio(getAudioChannel());
}

bool DataProcessor::checkNextFrameMarker(QByteArray data, int currentIndex){
    if(((quint8) data.at(currentIndex + 7) == (quint8) data.at(currentIndex) + 1
        && (quint8) data.at(currentIndex + 14) == (quint8) data.at(currentIndex + 7) + 1
        && (quint8) data.at(currentIndex + 21) == (quint8) data.at(currentIndex + 14) + 1
        && (quint8) data.at(currentIndex + 28) == (quint8) data.at(currentIndex + 21) + 1
        && (quint8) data.at(currentIndex + 35) == (quint8) data.at(currentIndex + 28) + 1)){
        return true;
    }
    else{
        return false;
    }
}

void DataProcessor::sortADCData(QByteArray adcData){
    for(int i = 0; i < adcData.size(); i++){
        audioBuffer[2].append(adcData.at(i));
        ADC_Data.append(adcData.at(i));
    }
}

void DataProcessor::setADCRecordEnabled(bool enableFlag){
    ADCRecordEnabled = enableFlag;
//    if(enableFlag){
//        fileName = directory + QDateTime::currentDateTime().toString("'data_'yyyyMMdd_HHmmss'ADC.csv'");
//        File = new QFile;
//        File->setFileName(fileName);
//        if(File->open(QIODevice::WriteOnly|QIODevice::Text)){
//            qDebug()<< "File ADC opened";
//        }
//        else{
//            qDebug() << "File ADC failed to open";
//        }
//        out = new QTextStream(File);
//        qDebug() << "setting Record Enabled";
//    }
//    else{
//        File->close();
//        qDebug() << "setting Record disabled";
//    }
}

bool DataProcessor::isADCRecordEnabled(){
    return ADCRecordEnabled;
}


#include "dataprocessor.h"

DataProcessor::DataProcessor(float samplingRate_){
    samplingRate = samplingRate_;
    period = 1/samplingRate_;
}

void DataProcessor::parseFrameMarkers(QByteArray rawData){
//    qDebug() << rawData.size();
    for(int i = 0; i < rawData.size(); i = i + 5){
        fullWord_rawData = ((quint8) rawData.at(i) << 8 | (quint8) rawData.at(i+1))-32768;
        appendAudioBuffer(0, rawData.at(i+2), rawData.at(i+1));
        if(RecordEnabled){
            RecordData(fullWord_rawData);
        }
        ChannelData[0].append(fullWord_rawData*(0.000000195));

        fullWord_rawData = ((quint8) rawData.at(i+2) << 8 | (quint8) rawData.at(i+3))-32768;
        appendAudioBuffer(1, rawData.at(i+4), rawData.at(i+3));
        if(RecordEnabled){
            RecordData(fullWord_rawData);
        }
        ChannelData[1].append(fullWord_rawData*(0.000000195));
        if(RecordEnabled){
            if(ADC_Data.size()>0){
                RecordData(ADC_Data.at(0));
            }
            else{
                RecordData(0);
            }
            RecordData((quint8) rawData.at(i+4));
            RecordData(END_OF_LINE);
        }
//        if(ADC_Data.size()>0){
//            ChannelData[2].append(ADC_Data.at(0)/ 256.0 * 2.5);
//            ADC_Data.remove(0, 1);
//        }

        ChannelData[2].append(0);
        if(RecordEnabled){
            RecordData(0);
        }

        ChannelData[3].append((quint8) rawData.at(i+4));
        total_data_count = total_data_count+1;
        X_axis.append(total_data_count*period);
    }
//    playAudio(getAudioChannel());
}

bool DataProcessor::checkNextFrameMarker(QByteArray data, int currentIndex){
    if(((quint8) data.at(currentIndex + 5) == (quint8) data.at(currentIndex) + 1
        && (quint8) data.at(currentIndex + 10) == (quint8) data.at(currentIndex + 5) + 1
        && (quint8) data.at(currentIndex + 15) == (quint8) data.at(currentIndex + 10) + 1
        && (quint8) data.at(currentIndex + 20) == (quint8) data.at(currentIndex + 15) + 1
        && (quint8) data.at(currentIndex + 25) == (quint8) data.at(currentIndex + 20) + 1)){
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

void DataProcessor::appendSync(){
    qDebug() << "Sync pulse detected!";
//    ChannelData[10].removeLast();
    ChannelData[2].append(1);
    if(RecordEnabled){
        RecordData(1);
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


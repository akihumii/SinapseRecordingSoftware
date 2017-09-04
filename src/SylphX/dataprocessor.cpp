#include "dataprocessor.h"

namespace SylphX {

DataProcessor::DataProcessor(float samplingRate_){
    samplingRate = samplingRate_;
    period = 1/samplingRate_;
}

void DataProcessor::parseFrameMarkers(QByteArray rawData){
//    qDebug() << rawData.size();
    for(int i = 0; i < rawData.size(); i = i + 22){
        for(int j = 2; j < 10; j++){
            fullWord_rawData = ((quint8) rawData.at(i+1+((2*j))) << 8 | (quint8) rawData.at(i+1+((2*j)+1)))-32768;
            std::cout << fullWord_rawData;
            if(RecordEnabled){
                RecordData(fullWord_rawData);
            }
            appendAudioBuffer(j-2, rawData.at(i+1+(2*j)+1), rawData.at(i+1+(2*j)));
            ChannelData[j-2].append(fullWord_rawData*(0.000000195));
        }
        for(int j = 0; j < 2; j++){
            fullWord_rawData = ((quint8) rawData.at(i+1+((2*j))) << 8 | (quint8) rawData.at(i+1+((2*j)+1)))-32768;
            std::cout << fullWord_rawData;
            appendAudioBuffer(j+8, rawData.at(i+1+(2*j)+1), rawData.at(i+1+(2*j)));
            if(RecordEnabled){
                RecordData(fullWord_rawData);
            }
            ChannelData[j+8].append(fullWord_rawData*(0.000000195));
        }
        ChannelData[10].append((quint8) rawData.at(i));
        if(RecordEnabled){
            RecordData((quint8) rawData.at(i));
        }
        total_data_count++;
        X_axis.append(total_data_count*period);
        ChannelData[11].append((quint8) rawData.at(i+21));
        if(RecordEnabled){
            RecordData((quint8)rawData.at(i+21));
            RecordData(END_OF_LINE);
        }
    }
//    playAudio(getAudioChannel());
}

void DataProcessor::parseFrameMarkersWithChecks(QByteArray rawData){
//    qDebug() << rawData.size();
    if(leftOverData.size() > 0){
        for(int i=leftOverData.size()-1;i>=0;i--){
            rawData.prepend(leftOverData.at(i));
        }
        leftOverData.clear();
    }
    firstFrameMarker = findfirstFrameMarkers(rawData);
    lastFrameMarker = findlastFrameMarkers(rawData);
//    qDebug() << lastFrameMarker;
    if(lastFrameMarker > 0){
        for(int i = 0; i < lastFrameMarker - 20; i = i + 1){
            if (i%22 == firstFrameMarker && checkNextFrameMarker(rawData, i)){
                for(int j = 2; j < 10; j++){
                    fullWord_rawData = ((quint8) rawData.at(i+1+((2*j)+1)) << 8 | (quint8) rawData.at(i+1+((2*j)+2)))-32768;
                    appendAudioBuffer(j-2, rawData.at(i+(2*j)+2), rawData.at(i+(2*j)+1));
                    if(RecordEnabled){
                        RecordData(fullWord_rawData);
                    }
                    ChannelData[j-2].append(fullWord_rawData*(0.000000195));
                }
                for(int j = 0; j < 2; j++){
                    fullWord_rawData = ((quint8) rawData.at(i+1+((2*j)+1)) << 8 | (quint8) rawData.at(i+1+((2*j)+2)))-32768;
                    appendAudioBuffer(j+8, rawData.at(i+(2*j)+2), rawData.at(i+(2*j)+1));
                    if(RecordEnabled){
                        RecordData(fullWord_rawData);
                    }
                    ChannelData[j+8].append(fullWord_rawData*(0.000000195));
                }
                ChannelData[10].append((quint8) rawData.at(i+1));
                ChannelData[11].append((quint8) rawData.at(i));
                total_data_count = total_data_count+1;
                X_axis.append(total_data_count*period);
                if(RecordEnabled){
                    RecordData((quint8) rawData.at(i+1));
                    RecordData((quint8) rawData.at(i));
                    RecordData(END_OF_LINE);
                }
            }
        }
        for(int i = lastFrameMarker; i < rawData.size(); i++){
            leftOverData.append(rawData.at(i));
        }
    }
//    playAudio(getAudioChannel());
}

bool DataProcessor::checkNextFrameMarker(QByteArray data, int currentIndex){
        if(((quint8) data.at(currentIndex + 22) == (quint8) data.at(currentIndex) + 1)){
        return true;
    }
    else if((quint8)data.at(currentIndex) == 249
                && (quint8)data.at(currentIndex+22) == 250){
        return true;
    }
    else if((quint8)data.at(currentIndex) == 250
                && (quint8)data.at(currentIndex+22) == 0){
        return true;
    }
    else{
        return false;
    }
}

int DataProcessor::findfirstFrameMarkers(QByteArray rawData){
    for(int i = 0; i < rawData.size()-44; i++){
        if((quint8)rawData.at(i+44) == (quint8)rawData.at(i+22)+1
                && (quint8)rawData.at(i+22) == (quint8)rawData.at(i)+1){
            return i;
        }
        else if((quint8)rawData.at(i+44) == 1 && (quint8)rawData.at(i+22) == 0 && (quint8)rawData.at(i) == 250){
            return i;
        }
        else if((quint8)rawData.at(i+44) == 0 && (quint8)rawData.at(i+22) == 250 && (quint8)rawData.at(i) == 249){
            return i;
        }
    }
    return 0;
}

int DataProcessor::findlastFrameMarkers(QByteArray rawData){
    if(rawData.size()>42){
        for(int i = rawData.size()-1; i > 43; i--){
            if((quint8)rawData.at(i-22)+1 == (quint8)rawData.at(i) && (quint8)rawData.at(i-44)+1 == (quint8)rawData.at(i-22)){
                return i;
            }
            else if((quint8)rawData.at(i-44) == 249 && (quint8)rawData.at(i-22) == 250 && (quint8)rawData.at(i) == 0){
                return i;
            }
            else if((quint8)rawData.at(i-44) == 250 && (quint8)rawData.at(i-22) == 0 && (quint8)rawData.at(i) == 1){
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

void DataProcessor::setSmartDataProcessor(bool flag){
    smartDataProcessor = flag;
}

bool DataProcessor::isSmart(){
    return smartDataProcessor;
}

}

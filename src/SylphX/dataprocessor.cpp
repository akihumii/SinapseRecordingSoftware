#include "dataprocessor.h"

namespace SylphX {

DataProcessor::DataProcessor(float samplingRate_, QProcess *process_){
    samplingRate = samplingRate_;
    period = 1/samplingRate_;
    process = process_;
}

void DataProcessor::parseFrameMarkers(QByteArray rawData){
    for(int i = 0; i < rawData.size(); i = i + packetSize){
        for(int j = 2; j < NUM_CHANNELS; j++){
            fullWord_rawData = ((quint8) rawData.at(i+((2*j))) << 8 | (quint8) rawData.at(i+((2*j)+1)))-32768;
            if(RecordEnabled){
                RecordData(fullWord_rawData);
            }
            ChannelData[j-2].append(fullWord_rawData*(0.000000195));
        }
        for(int j = 0; j < 2; j++){
            fullWord_rawData = ((quint8) rawData.at(i+((2*j))) << 8 | (quint8) rawData.at(i+((2*j)+1)))-32768;
            if(this->isRecordEnabled()){
                RecordData(fullWord_rawData);
            }
            ChannelData[j+(NUM_CHANNELS-2)].append(fullWord_rawData*(0.000000195));
        }
//        for(int j = NUM_CHANNELS; j < 10; j++){
//            if(RecordEnabled){
//                RecordData(0);
//            }
//            ChannelData[j].append(0*(0.000000195));
//        }
        ChannelData[10].append((quint8) rawData.at(i+packetSize-5));
        if(RecordEnabled){
            RecordData((quint8) rawData.at(i+packetSize-5));
        }
        total_data_count++;
        X_axis.append(total_data_count*period);
        ChannelData[11].append((quint8) rawData.at(i+(packetSize-4)) << 8 | (quint8) rawData.at(i+packetSize-3));
        if(RecordEnabled){
            RecordData((quint8) rawData.at(i+(packetSize-4)) << 8 | (quint8) rawData.at(i+(packetSize-3)));
            RecordData(END_OF_LINE);
        }
    }
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
//    qDebug() << firstFrameMarker;
    if(lastFrameMarker > 0){
        for(int i = 0; i < lastFrameMarker - (packetSize-1); i = i + 1){
            if (i%packetSize == firstFrameMarker && checkNextFrameMarker(rawData, i)){
                for(int j = 2; j < 10; j++){
                    fullWord_rawData = ((quint8) rawData.at(i+1+((2*j))) << 8 | (quint8) rawData.at(i+1+((2*j)+1)))-32768;
//                    appendAudioBuffer(j-2, rawData.at(i+(2*j)+2), rawData.at(i+(2*j)+1));
                    if(RecordEnabled){
                        RecordData(fullWord_rawData);
                    }
                    ChannelData[j-2].append(fullWord_rawData*(0.000000195));
                }
                for(int j = 0; j < 2; j++){
                    fullWord_rawData = ((quint8) rawData.at(i+1+((2*j))) << 8 | (quint8) rawData.at(i+1+((2*j)+1)))-32768;
//                    appendAudioBuffer(j+8, rawData.at(i+(2*j)+2), rawData.at(i+(2*j)+1));
                    if(RecordEnabled){
                        RecordData(fullWord_rawData);
                    }
                    ChannelData[j+8].append(fullWord_rawData*(0.000000195));
                }
                ChannelData[10].append((quint8) rawData.at(i+packetSize-4));
                ChannelData[11].append((quint8) rawData.at(i+(packetSize-3)) << 8 | (quint8) rawData.at(i+(packetSize-2)));
//                qDebug() << (quint8) rawData.at(i+(packetSize));
                total_data_count = total_data_count+1;
                X_axis.append(total_data_count*period);
                if(RecordEnabled){
                    RecordData((quint8) rawData.at(i-4));
                    RecordData((quint8) rawData.at((quint8) rawData.at(i-3) << 8 | (quint8) rawData.at(i-2)));
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

bool DataProcessor::checkNextFrameMarker(QByteArray data, int mark){
    if((quint8) data.at(mark) == 165 && (quint8) data.at(mark+1+(NUM_CHANNELS*2)) == 0 && data.at(mark+4+(NUM_CHANNELS*2)) == 90)
        return true;
    else
        return false;
}

int DataProcessor::findfirstFrameMarkers(QByteArray rawData){
//    qDebug() << "Finding first frame marker";
    for(int i = 0; i < rawData.size()-(packetSize*2); i++){
        if((quint8)rawData.at(i+(packetSize*2)) == (quint8)rawData.at(i+packetSize)
                && (quint8)rawData.at(i+packetSize) == (quint8)rawData.at(i)){
            return i;
        }
    }
    return 0;
}

int DataProcessor::findlastFrameMarkers(QByteArray rawData){
//    qDebug() << "Finding last frame marker";
    if(rawData.size()>(packetSize*2-2)){
        for(int i = rawData.size()-1; i > (packetSize*2-1); i--){
            if((quint8)rawData.at(i-packetSize) == (quint8)rawData.at(i) && (quint8)rawData.at(i-(packetSize*2)) == (quint8)rawData.at(i-packetSize)){
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

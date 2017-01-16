#include "dataprocessor.h"

DataProcessor::DataProcessor(Channel *NeutrinoChannel_){
    qDebug()<< "Compiling for NEUTRINO_II.. ";
    NeutrinoChannel = NeutrinoChannel_;
    File = new QFile("Edison.txt");
}

DataProcessor::DataProcessor(){

}

QVector<quint16> DataProcessor::ParseFrameMarkers10bits(QByteArray data_store){
    QVector<quint16> Plot_Y_AllDataPoint;
    Plot_Y_AllDataPoint.clear();
    uint16_t combine_10bit;
    int numChannels = NeutrinoChannel->getNumChannels();
    if(leftOverData.size() > 0){
        for(int i=leftOverData.size()-1;i>=0;i--){
            data_store.prepend(leftOverData.at(i));
        }
        leftOverData.clear();
    }
    int firstFrameMarker = first_10bitFrameMarker(data_store);
//    if (firstFrameMarker < 0){
//        data_store.clear();
//        qDebug() << "Can't find first frame marker..";
//        return Plot_Y_AllDataPoint;
//    }
    int lastFrameMarker = last_10bitFrameMarker(data_store);
    qDebug() << lastFrameMarker;
        for(int j = firstFrameMarker ; j < lastFrameMarker; j = j + 1){
            if((uint8_t)data_store.at(j) == FM_A
                && (uint8_t)data_store.at(j+1) == FM_5
                && (uint8_t)data_store.at(j+2) == FM_0
                && (uint8_t)data_store.at(j+3) == FM_F){
                if((uint8_t)data_store.at(j+(numChannels*2 + 4)) == FM_A
                    && (uint8_t)data_store.at(j+(numChannels*2 + 5)) == FM_5
                    && (uint8_t)data_store.at(j+(numChannels*2 + 6)) == FM_0
                    && (uint8_t)data_store.at(j+(numChannels*2 + 7)) == FM_F){
                    for(int i = 0; i < numChannels; i++){
                        combine_10bit = ((uint8_t)data_store.at(j+5+i*2) << 5) | (uint8_t)data_store.at(j+4+i*2);
                        Plot_Y_AllDataPoint.append(combine_10bit);
                    }
                }
            }
        }
        for(int i = lastFrameMarker; i < data_store.size(); i++){
            leftOverData.append(data_store.at(i));
        }
    return Plot_Y_AllDataPoint;
}

int DataProcessor::first_10bitFrameMarker(QByteArray data){
    for(int i=0;i<data.size()-4;i++){
        if((uint8_t)data.at(i) == FM_A
            && (uint8_t)data.at(i+1) == FM_5
            && (uint8_t)data.at(i+2) == FM_0
            && (uint8_t)data.at(i+3) == FM_F){
            return i;
        }
    }
    return 0;
}

int DataProcessor::last_10bitFrameMarker(QByteArray data){
    if(data.size()>3){
        for(int i = data.size()-1; i > 3; i--){
            if((uint8_t)data.at(i-3) == FM_A
                && (uint8_t)data.at(i-2) == FM_5
                && (uint8_t)data.at(i-1) == FM_0
                && (uint8_t)data.at(i) == FM_F){
                return i-3;
            }
        }
    }
    return 0;
}

QVector<quint16> DataProcessor::ParseFrameMarkers8bits(QByteArray data_store){
    QVector<quint16> Plot_Y_AllDataPoint;
    Plot_Y_AllDataPoint.clear();
    uint8_t current_8bit;
    int numChannels = NeutrinoChannel->getNumChannels();
    if(leftOverData.size() > 0){
        for(int i=0;i<leftOverData.size();i++){
            data_store.prepend(leftOverData.at(i));
        }
        leftOverData.clear();
    }
    int firstFrameMarker = first_8bitFrameMarker(data_store);
    int lastFrameMarker = last_8bitFrameMarker(data_store);
    for(int j = firstFrameMarker ; j < lastFrameMarker; j=j+1){
        if((uint8_t)data_store.at(j) == FM_F0){
                if((uint8_t)data_store.at(j+(numChannels + 1)) == FM_5A){
                    for(int i = 0; i < numChannels; i++){
                        current_8bit = (uint8_t)data_store.at(j+1+i);
                        Plot_Y_AllDataPoint.append(current_8bit);
                    }
                }
        }
    }
    for(int i = lastFrameMarker; i < data_store.size(); i++){
        leftOverData.append(data_store.at(i));
    }
    return Plot_Y_AllDataPoint;
}

int DataProcessor::last_8bitFrameMarker(QByteArray data){
    for(int i= data.size(); i>2; i--){
        if((uint8_t) data.at(i-1) == FM_F0
            && (uint8_t)data.at(i-2) == FM_5A){
            return i-2;
        }
    }
    return -1;
}

int DataProcessor::first_8bitFrameMarker(QByteArray data){
    for(int i=0; i<data.size()-1;i++){
        if((uint8_t) data.at(i) == FM_5A
            && (uint8_t)data.at(i+1) == FM_F0){
            return i;
        }
    }
    return -1;
}

void DataProcessor::MultiplexChannelData(QVector<quint16> Plot_Y_AllDataPoint){
    bool *channels = NeutrinoChannel->getChannelState_Bool();
    int numChannels = NeutrinoChannel->getNumChannels();
    if(!Plot_Y_AllDataPoint.isEmpty()){
        for(int i = 0; i < (Plot_Y_AllDataPoint.size()); i = i + numChannels){
            int k = 0;
            for(int ChannelIndex=0;ChannelIndex<10;ChannelIndex++){
                if(channels[ChannelIndex]){
                    if(is8BitMode){
                        ChannelData[ChannelIndex].append(Plot_Y_AllDataPoint.at(i+k)*1.2/256);
                    }
                    else if(!is8BitMode){
                        ChannelData[ChannelIndex].append(Plot_Y_AllDataPoint.at(i+k)*1.2/1024);
                    }
                    if(isRecordEnabled()){
                        RecordData(Plot_Y_AllDataPoint.at(i+k));
                    }
                    k++;
                }
                else{
                    if(isRecordEnabled()){
                        RecordData(0);
                    }
                    ChannelData[ChannelIndex].append(-0.1);
                }
            }
            if(isRecordEnabled()){
                RecordData(END_OF_LINE);
            }
            total_data_count = total_data_count+1;
            X_axis.append(total_data_count*0.000056);
        }
    }
    Plot_Y_AllDataPoint.clear();
}

void DataProcessor::setBitMode(bool BitMode){
    is8BitMode = BitMode;
}

void DataProcessor::parseFrameMarkers(QByteArray rawData){
    if(leftOverData.size() > 0){
        for(int i=leftOverData.size()-1;i>=0;i--){
            rawData.prepend(leftOverData.at(i));
        }
        leftOverData.clear();
    }
    firstFrameMarker = findfirstFrameMarkers(rawData);
    lastFrameMarker = findlastFrameMarkers(rawData);
    if(lastFrameMarker > 0){
        for(int i=0;i<lastFrameMarker-3;i=i+1){
            if (i%5 == firstFrameMarker && checkNextFrameMarker(rawData, i)){
                fullWord_rawData = ((quint8) rawData.at(i+1) << 8 | (quint8) rawData.at(i+2))-32768;
                appendAudioBuffer(0, rawData.at(i+2), rawData.at(i+1));
                if(RecordEnabled){
                    RecordData(fullWord_rawData);
                }
                ChannelData[0].append(fullWord_rawData*(0.000000195));

                fullWord_rawData = ((quint8) rawData.at(i+3) << 8 | (quint8) rawData.at(i+4))-32768;
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
                    RecordData((quint8) rawData.at(i));
                    RecordData(END_OF_LINE);
                }
                if(ADC_Data.size()>0){
                    ChannelData[2].append(ADC_Data.at(0)/ 256.0 * 2.5);
                    ADC_Data.remove(0, 1);
                }
                ChannelData[3].append((quint8) rawData.at(i));
                total_data_count = total_data_count+1;
                X_axis.append(total_data_count*0.000048);
            }
        }
        for(int i = lastFrameMarker; i < rawData.size(); i++){
            leftOverData.append(rawData.at(i));
        }
    }
    playAudio(getAudioChannel());
}

bool DataProcessor::checkNextFrameMarker(QByteArray data, int currentIndex){
    if((quint8) data.at(currentIndex + 5) == (quint8) data.at(currentIndex) + 1
            || ((quint8) data.at(currentIndex) == 250 && (quint8) data.at(currentIndex + 5) == 0)){
        return true;
    }
    else{
        return false;
    }
}

int DataProcessor::findfirstFrameMarkers(QByteArray rawData){
    for(int i=0; i<rawData.size()-10;i++){
        if((quint8)rawData.at(i+5) == (quint8)rawData.at(i)+1
                && (quint8)rawData.at(i+10) == (quint8)rawData.at(i+5)+1){
            return i;
        }
        else if((quint8)rawData.at(i+10) == 1 && (quint8)rawData.at(i+5) == 0 && (quint8)rawData.at(i) == 250){
            return i;
        }
        else if((quint8)rawData.at(i+10) == 0 && (quint8)rawData.at(i+5) == 250 && (quint8)rawData.at(i) == 249){
            return i;
        }
    }
    return -1;
}

int DataProcessor::findlastFrameMarkers(QByteArray rawData){
    if(rawData.size()>8){
        for(int i=rawData.size()-1;i>9;i--){
            if((quint8)rawData.at(i-5)+1 == (quint8)rawData.at(i) && (quint8)rawData.at(i-10)+1 == (quint8)rawData.at(i-5)){
                return i;
            }
            else if((quint8)rawData.at(i-10) == 249 && (quint8)rawData.at(i-5) == 250 && (quint8)rawData.at(i) == 0){
                return i;
            }
            else if((quint8)rawData.at(i-10) == 250 && (quint8)rawData.at(i-5) == 0 && (quint8)rawData.at(i) == 1){
                return i;
            }
        }
    }
    return -1;
}

void DataProcessor::sortADCData(QByteArray adcData){
    for(int i=0;i<adcData.size();i++){
        audioBuffer[2].append(adcData.at(i));
        ADC_Data.append(adcData.at(i));
    }
}

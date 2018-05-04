#include "dataprocessor.h"

DataProcessor::DataProcessor(Channel *NeutrinoChannel_){
    NeutrinoChannel = NeutrinoChannel_;
}

QVector<quint16> DataProcessor::ParseFrameMarkers10bits(QByteArray data_store){
    qDebug() << data_store.size() << " | " << leftOverData.size();
    QVector<quint16> Plot_Y_AllDataPoint;
    Plot_Y_AllDataPoint.clear();
    uint16_t combine_10bit;
    int numChannels = NeutrinoChannel->getNumChannels();
    if(leftOverData.size() > 0){
        for(int i=0;i<leftOverData.size();i++){
            data_store.prepend(leftOverData.at(i));
        }
        leftOverData.clear();
        leftOverData.resize(0);
    }
    firstFrameMarker = first_10bitFrameMarker(data_store);
    lastFrameMarker = last_10bitFrameMarker(data_store);
    for(int j = firstFrameMarker ; j < lastFrameMarker - (numChannels*2); j = j + 1){
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
    if(leftOverData.size() > 204800)
    {
        leftOverData.clear();
        leftOverData.resize(0);
    }
    return Plot_Y_AllDataPoint;
}

double DataProcessor::signalReconstruction(char input){
    return ((quint8) input * 1.2/256.0);
}

double DataProcessor::signalReconstruction(char inputMSB, char inputLSB){
    quint16 temp = ((((quint8) inputMSB << 5) | ((quint8) inputLSB)) & 0B0000001111111111);
    return (temp * 1.2/1024.0);
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
    qDebug() << data_store.size() << " | " << leftOverData.size();
    QVector<quint16> Plot_Y_AllDataPoint;
    Plot_Y_AllDataPoint.clear();
    uint8_t current_8bit;
    int numChannels = NeutrinoChannel->getNumChannels();
    if(leftOverData.size() > 0){
        for(int i=0;i<leftOverData.size();i++){
            data_store.prepend(leftOverData.at(i));
        }
        leftOverData.clear();
        leftOverData.resize(0);
    }
    firstFrameMarker = first_8bitFrameMarker(data_store);
    lastFrameMarker = last_8bitFrameMarker(data_store);
    for(int j = firstFrameMarker ; j < lastFrameMarker - numChannels; j=j+1){
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
    if(leftOverData.size() > 204800)
    {
        leftOverData.clear();
        leftOverData.resize(0);
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
    return 0;
}

int DataProcessor::first_8bitFrameMarker(QByteArray data){
    for(int i=0; i<data.size()-1;i++){
        if((uint8_t) data.at(i) == FM_5A
            && (uint8_t)data.at(i+1) == FM_F0){
            return i;
        }
    }
    return 0;
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
                        if(isInputReferred){
                            ChannelData[ChannelIndex].append(((Plot_Y_AllDataPoint.at(i+k)*1.2/256) - 0.5) / gain);
                        }
                        else{
                            ChannelData[ChannelIndex].append(Plot_Y_AllDataPoint.at(i+k)*1.2/256);
                        }
                    }
                    else if(!is8BitMode){
                        if(isInputReferred){
                            ChannelData[ChannelIndex].append(((Plot_Y_AllDataPoint.at(i+k)*1.2/1024) - 0.5) / gain);
                        }
                        else{
                            ChannelData[ChannelIndex].append(Plot_Y_AllDataPoint.at(i+k)*1.2/1024);
                        }
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
            X_axis.append(total_data_count*((14.0*(numChannels+2.0)/3000000.0)));
        }
    }
    Plot_Y_AllDataPoint.clear();
}

void DataProcessor::setBitMode(bool BitMode){
    is8BitMode = BitMode;
}

void DataProcessor::setInputReferred(bool flag){
    isInputReferred = flag;
}

bool DataProcessor::getInputReferred(){
    return isInputReferred;
}

void DataProcessor::setGain(bool S1G0, bool S2GAIN1, bool S2GAIN0){
    double D1 = 1.0, D2 = 0, R = 1047.13;
    char temp = (char) S2GAIN1 << 1 | (char) S2GAIN0;
    if(S1G0){
        D1 = 4.0;
    }

    switch(temp){
        case 0:
            D2 = 1.0;
            break;
        case 1:
            D2 = 1.95;
            break;
        case 2:
            D2 = 2.85;
            break;
        case 3:
            D2 = 1.0/5.4325;
            break;
        default:
            D2 = 1.0;
            break;
    }

    gain = R / (D1*D2);
    qDebug() << "D1 is set as: " << D1 << " | D2 is set as: " << D2 << " | Resulting gain: " << gain;
}

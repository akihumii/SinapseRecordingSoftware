#include "dataprocessor.h"

DataProcessor::DataProcessor(Channel *NeutrinoChannel_, DataStream *dataStream_){
    NeutrinoChannel = NeutrinoChannel_;
    dataStream = dataStream_;
    setGain(false, false, false);
}

QVector<quint16> DataProcessor::ParseFrameMarkers10bits(QByteArray data_store){
//    qDebug() << data_store.size();
    QVector<quint16> Plot_Y_AllDataPoint;
    bool *channels = NeutrinoChannel->getChannelState_Bool();
    if(leftOverData.size() > 0){
        data_store.prepend(leftOverData);
        leftOverData.clear();
        leftOverData.resize(0);
    }
    firstFrameMarker = first_10bitFrameMarker(data_store);
    lastFrameMarker = last_10bitFrameMarker(data_store);
    for(int j = firstFrameMarker ; j < lastFrameMarker - (NeutrinoChannel->getNumChannels()*2); j = j + 1){
        if((uint8_t)data_store.at(j) == FM_A
            && (uint8_t)data_store.at(j+1) == FM_5
            && (uint8_t)data_store.at(j+2) == FM_0
            && (uint8_t)data_store.at(j+3) == FM_F){
            if((uint8_t)data_store.at(j+(NeutrinoChannel->getNumChannels()*2 + 4 + 8)) == FM_A
                && (uint8_t)data_store.at(j+(NeutrinoChannel->getNumChannels()*2 + 5 + 8)) == FM_5
                && (uint8_t)data_store.at(j+(NeutrinoChannel->getNumChannels()*2 + 6 + 8)) == FM_0
                && (uint8_t)data_store.at(j+(NeutrinoChannel->getNumChannels()*2 + 7 + 8)) == FM_F){
                int k = 0;
                for(int ChannelIndex=0;ChannelIndex<10;ChannelIndex++){
                    if(channels[ChannelIndex]){
                        if(bitMode == WORDLENGTH_10){
                            if(isInputReferred){
                                temp = (((((uint8_t)data_store.at(j+5+8+k*2) << 5) | (uint8_t)data_store.at(j+4+8+k*2))*1.2/1024) - 0.5) / gain;
                                ChannelData[ChannelIndex].append(temp);
                                dataStream->appendData(ChannelIndex, temp);
                            }
                            else{
                                temp = (((uint8_t)data_store.at(j+5+8+k*2) << 5) | (uint8_t)data_store.at(j+4+8+k*2))*1.2/1024;
                                ChannelData[ChannelIndex].append(temp);
                                dataStream->appendData(ChannelIndex, temp);
                            }
                        }
                        if(isRecordEnabled()){
//                            RecordData((quint16)((uint8_t)data_store.at(j+5+k*2) << 5) | (uint8_t)data_store.at(j+4+k*2)); // For development
                            RecordData((quint16)((uint8_t)data_store.at(j+5+8+k*2) << 5) | (uint8_t)data_store.at(j+4+8+k*2)); // Actual code
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
                ChannelData[10].append((quint16) (quint8) data_store.at(j+5+6) << 5 | (quint8) data_store.at(j+4+6));
                temp_int = (quint32)   (((((uint32_t)data_store.at(j + 4 + 1) << 5 | (uint32_t)data_store.at(j + 4)) >> 1 & 0b111111111)|
                                     (((((uint32_t)data_store.at(j + 4 + 3) << 5 | (uint32_t)data_store.at(j + 4 + 2)) >> 1 & 0b111111111)) << 9) |
                                     (((((uint32_t)data_store.at(j + 4 + 5) << 5 | (uint32_t)data_store.at(j + 4 + 4)) >> 1 & 0b111111111)) << 18))
                                      & ~(0b11111 << 27));
//                if(temp_int > max){
//                    max = temp_int;
//                    qDebug() << max;
//                }
                ChannelData[11].append(temp_int / 1000);
                if(isRecordEnabled()){
                    RecordData((quint16) (quint8) data_store.at(j+5+6) << 5 | (quint8) data_store.at(j+4+6));
                    RecordData((quint32)   (((((uint32_t)data_store.at(j + 4 + 1) << 5 | (uint32_t)data_store.at(j + 4)) >> 1 & 0b111111111)|
                                           (((((uint32_t)data_store.at(j + 4 + 3) << 5 | (uint32_t)data_store.at(j + 4 + 2)) >> 1 & 0b111111111)) << 9) |
                                           (((((uint32_t)data_store.at(j + 4 + 5) << 5 | (uint32_t)data_store.at(j + 4 + 4)) >> 1 & 0b111111111)) << 18))
                                            & ~(0b11111 << 27))); // Record of counter
                    RecordData(END_OF_LINE);
                }
                total_data_count = total_data_count+1;
                X_axis.append(total_data_count*((14.0*(NeutrinoChannel->getNumChannels()+2.0)/3000000.0)));
            }
        }
    }
    for(int i = lastFrameMarker; i < data_store.size(); i++){
        leftOverData.append(data_store.at(i));
    }
    if(leftOverData.size() > 20480){
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
    QVector<quint16> Plot_Y_AllDataPoint;
    bool *channels = NeutrinoChannel->getChannelState_Bool();
    if(leftOverData.size() > 0){
        data_store.prepend(leftOverData);
        leftOverData.clear();
        leftOverData.resize(0);
    }
    firstFrameMarker = first_8bitFrameMarker(data_store);
    lastFrameMarker = last_8bitFrameMarker(data_store);
    for(int j = firstFrameMarker ; j < lastFrameMarker - NeutrinoChannel->getNumChannels(); j=j+1){
        if((uint8_t)data_store.at(j) == FM_F0){
            if((uint8_t)data_store.at(j+(NeutrinoChannel->getNumChannels() + 5)) == FM_5A){
                int k = 0;
                for(int ChannelIndex=0;ChannelIndex<10;ChannelIndex++){
                    if(channels[ChannelIndex]){
                        if(bitMode == WORDLENGTH_8){
                            if(isInputReferred){
                                temp = (((uint8_t)data_store.at(j+k+5)*1.2/256) - 0.5) / gain;
                                ChannelData[ChannelIndex].append(temp);
                                dataStream->appendData(ChannelIndex, temp);
                            }
                            else{
                                temp = (uint8_t)data_store.at(j+k+5)*1.2/256;
                                ChannelData[ChannelIndex].append(temp);
                                dataStream->appendData(ChannelIndex, temp);
                            }
                        }
                        if(isRecordEnabled()){
                            RecordData((uint8_t)data_store.at(j+k+1)); // For development
//                            RecordData((uint8_t)data_store.at(j+k+5)); // Actual
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
                ChannelData[10].append((quint8) data_store.at(j+4));
                temp_int = (quint32) (((((quint32)data_store.at(j+3)>>1) & 0b1111111) << 14) |
                                                  ((((quint32)data_store.at(j+2)>>1) & 0b1111111) << 7) |
                                                  (((quint32)data_store.at(j+1)>>1) & 0b1111111)) &
                                                  ~(0b11111111111 << 21);
//                if(temp_int > max){
//                    max = temp_int;
//                    qDebug() << max;
//                }
                ChannelData[11].append(temp_int / 1000);

                if(isRecordEnabled()){
                    RecordData((quint8) data_store.at(j+4));
                    RecordData((quint32) (((((quint32)data_store.at(j+3)>>1) & 0b1111111) << 14) |
                                            ((((quint32)data_store.at(j+2)>>1) & 0b1111111) << 7) |
                                            (((quint32)data_store.at(j+1)>>1) & 0b1111111)) &
                                            ~(0b11111111111 << 21));
                    RecordData(END_OF_LINE);
                }
                total_data_count = total_data_count+1;
                X_axis.append(total_data_count*((14.0*(NeutrinoChannel->getNumChannels()+2.0)/3000000.0)));
            }
        }
    }
    for(int i = lastFrameMarker; i < data_store.size(); i++){
        leftOverData.append(data_store.at(i));
    }
    if(leftOverData.size() > 20480){
        leftOverData.clear();
        leftOverData.resize(0);
    }
    return Plot_Y_AllDataPoint;
}

int DataProcessor::last_8bitFrameMarker(QByteArray data){
    for(int i = data.size(); i > 2; i--){
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
    if(!Plot_Y_AllDataPoint.isEmpty()){
        for(int i = 0; i < (Plot_Y_AllDataPoint.size()); i = i + NeutrinoChannel->getNumChannels()){
            int k = 0;
            for(int ChannelIndex=0;ChannelIndex<10;ChannelIndex++){
                if(channels[ChannelIndex]){
                    if(bitMode == WORDLENGTH_8){
                        if(isInputReferred){
                            ChannelData[ChannelIndex].append(((Plot_Y_AllDataPoint.at(i+k)*1.2/256) - 0.5) / gain);
                        }
                        else{
                            ChannelData[ChannelIndex].append(Plot_Y_AllDataPoint.at(i+k)*1.2/256);
                        }
                    }
                    else if(bitMode == WORDLENGTH_10){
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
            X_axis.append(total_data_count*((14.0*(NeutrinoChannel->getNumChannels()+2.0)/3000000.0)));
        }
    }
    Plot_Y_AllDataPoint.clear();
}

void DataProcessor::setBitMode(BITMODE BitMode){
    bitMode = BitMode;
}

BITMODE DataProcessor::getBitMode(){
    return bitMode;
}

void DataProcessor::setInputReferred(bool flag){
    isInputReferred = flag;
}

bool DataProcessor::getInputReferred(){
    return isInputReferred;
}

void DataProcessor::setGain(bool S1G0, bool S2GAIN1, bool S2GAIN0){
    double D1 = 1.0, D2 = 0, R = 1047.13;
    D1 = S1G0? 4.0 : 1.0;
    switch((char) S2GAIN1 << 1 | (char) S2GAIN0){
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

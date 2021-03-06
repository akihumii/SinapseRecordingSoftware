#include "dataprocessor.h"

DataProcessor::DataProcessor(DataStream *dataStream_){
    dataStream = dataStream_;
}

int DataProcessor::parseFrameMarkers(QByteArray rawData){
    forceSensorFlag? biasFullWord = 0 : biasFullWord = 32768;
    forceSensorFlag? biasMultiplier = 10000: biasMultiplier = multiplier;
        for(int i = 0; i < rawData.size(); i = i + packetSize){  // loop through every byte with stepsize of packetSize
            if(index > getNumDataPoints()){
                index = 0;
            }
            for(int j = 2; j < NUM_CHANNELS; j++){  // channel 1 to channel 8 data
                fullWord_rawData = ((quint8) rawData.at(i+((2*j))) << 8 | (quint8) rawData.at(i+((2*j)+1)))-biasFullWord;
                if(RecordEnabled){
                    RecordData(fullWord_rawData);
                }
    //            if(thresholdEnable){
    //                if(fullWord_rawData*(0.000195) > upperThreshold && j == 5+2){
    //                    thresholdEnable = false;
    //                    emit channelACrossed();
    //                    QTimer::singleShot(debounce, [=] {
    //                            thresholdEnable = true;
    //                    });
    //                }
    //                if(fullWord_rawData*(0.000195) > lowerThreshold && j == 6+2){
    //                    thresholdEnable = false;
    //                    emit channelBCrossed();
    //                    QTimer::singleShot(debounce, [=] {
    //                            thresholdEnable = true;
    //                    });
    //                }
    //            }
                ChannelData[j-2].replace(index, fullWord_rawData*(0.000000195)*biasMultiplier);
    //            if(dataStream->getStreamConnected(j-2)){
    //                dataStream->appendData(j-2, fullWord_rawData*(0.000000195));
    //            }
    //            appendAudioBuffer(j-2, rawData.at(i+((2*j))), rawData.at(i+((2*j)+1)));
            }
            for(int j = 0; j < 2; j++){  // channel 9 to channel 10 data
                fullWord_rawData = ((quint8) rawData.at(i+((2*j))) << 8 | (quint8) rawData.at(i+((2*j)+1)))-biasFullWord;
                if(RecordEnabled){
                    RecordData(fullWord_rawData);
                }
                ChannelData[j+(NUM_CHANNELS-2)].replace(index, fullWord_rawData*(0.000000195)*biasMultiplier);
                if(j == 0 && forceSensorFlag){
                    forceData.append(fullWord_rawData*(0.000000195)*biasMultiplier);
                }
    //            if(dataStream->getStreamConnected(j+(NUM_CHANNELS-2))){
    //                dataStream->appendData(j+(NUM_CHANNELS-2), fullWord_rawData*(0.000000195));
    //            }
    //            appendAudioBuffer(j+(NUM_CHANNELS-2), rawData.at(i+((2*j))), rawData.at(i+((2*j)+1)));
            }

    //        for(int j = 0; j < 10; j++){
    //            if(dataStream->getChannelSize(i) > 20480){
    //                dataStream->clearChannelData(i);
    //            }
    //        }

            ChannelData[10].replace(index, (quint8) rawData.at(i+packetSize-5));
            ChannelData[11].replace(index, ((quint8) rawData.at(i+(packetSize-4)) << 8 | (quint8) rawData.at(i+packetSize-3))/1000.0);
            if(RecordEnabled){
                RecordData((quint8) rawData.at(i+packetSize-5));  // sync pulse
                RecordData((quint8) rawData.at(i+(packetSize-4)) << 8 | (quint8) rawData.at(i+(packetSize-3)));  // counter
                recordCommand();
                RecordData(END_OF_LINE);
            }
            index++;
        }
//    playAudio(getAudioChannel());
    return rawData.size();
}

void DataProcessor::clearTransientData(){
    forceData.clear();
}

QVector<double> DataProcessor::retrieveTransientData(){
    return forceData;
}

int DataProcessor::parseFrameMarkersWithChecks(QByteArray rawData){
    if(leftOverData.size() > 0){
//        rawData.prepend(leftOverData);
//        leftOverData.clear();
//        leftOverData.resize(0);
        for(int i=leftOverData.size()-1;i>=0;i--){
            rawData.prepend(leftOverData.at(i));
        }
        leftOverData.clear();
    }
    firstFrameMarker = findfirstFrameMarkers(rawData);
    lastFrameMarker = findlastFrameMarkers(rawData);
    if(lastFrameMarker > 0){
        for(int i = 0; i < lastFrameMarker - (packetSize-1); i = i + 1){
            if (i%packetSize == firstFrameMarker){
                if(checkNextFrameMarker(rawData, i)){
                    resync_count = 0;
                    if(index > getNumDataPoints()){
                        index = 0;
                    }
                    for(int j = 2; j < NUM_CHANNELS; j++){
                        fullWord_rawData = ((quint8) rawData.at(i+1+((2*j))) << 8 | (quint8) rawData.at(i+1+((2*j)+1)))-32768;
//                        for(int k=0; k < 7; k++){
//                            appendAudioBuffer(j-2, rawData.at(i+1+((2*j)+1)), rawData.at(i+1+((2*j))));
//                        }
                        if(RecordEnabled){
                            RecordData(fullWord_rawData);
                        }
    //                    ChannelData[j-2].replace(index, fullWord_rawData*(0.000000195)*multiplier);
                        ChannelData[j-2].replace(index, fullWord_rawData*(0.000000195)*multiplier);
    //                    if(thresholdEnable){
    //                        if(fullWord_rawData*(0.000195) > upperThreshold && j == 5+2){
    //                            thresholdEnable = false;
    //                            emit channelACrossed();
    //                            QTimer::singleShot(debounce, [=] {
    //                                    thresholdEnable = true;
    //                            });
    //                        }
    //                        if(fullWord_rawData*(0.000195) > lowerThreshold && j == 6+2){
    //                            thresholdEnable = false;
    //                            emit channelBCrossed();
    //                            QTimer::singleShot(debounce, [=] {
    //                                    thresholdEnable = true;
    //                            });
    //                        }
    //                    }
    //                    if(dataStream->getStreamConnected(j-2)){
                            dataStream->appendData(j-2, fullWord_rawData*(0.000000195));
    //                    }
                    }
                    for(int j = 0; j < 2; j++){
                        fullWord_rawData = ((quint8) rawData.at(i+1+((2*j))) << 8 | (quint8) rawData.at(i+1+((2*j)+1)))-32768;
//                        for(int k=0; k < 7; k++){
//                            appendAudioBuffer(j+8, rawData.at(i+1+((2*j)+1)), rawData.at(i+1+((2*j))));
//                        }
                        if(RecordEnabled){
                            RecordData(fullWord_rawData);
                        }
                        ChannelData[j+(NUM_CHANNELS-2)].replace(index, fullWord_rawData*(0.000000195)*multiplier);
    //                    if(dataStream->getStreamConnected(j+(NUM_CHANNELS-2))){
                            dataStream->appendData(j+(NUM_CHANNELS-2), fullWord_rawData*(0.000000195));
    //                    }
    //                    ChannelData[j+(NUM_CHANNELS-2)].replace(index, 0);
                    }

    //                for(int j = 0; j < 10; j++){
    //                    if(dataStream->getChannelSize(i) > 20480){
    //                        dataStream->clearChannelData(i);
    //                    }
    //                }

                    ChannelData[10].replace(index, (quint8) rawData.at(i+packetSize-4));
//                    if((quint8) rawData.at(i+packetSize-4) > 0){
                        dyno_index =  ((index * retrieveDyno_XAxis().size()-1) / getNumDataPoints());
                        ChannelData[12].replace(dyno_index, dyno_data);
//                        qDebug() << dyno_index;
//                        if(dyno_store.size() > 0){
//                            for(int c = 0; c < dyno_store.size(); c++){
//                                ChannelData[12].replace(dyno_index+c, dyno_store.at(c));
//                            }
//                            dyno_store.clear();
//                        }
//                        dyno_start? ChannelData[12].replace(dyno_index+1, 20) : ChannelData[12].replace(dyno_index-1, 20);
//                        dyno_start = !dyno_start;
//                    }
                    ChannelData[11].replace(index, ((quint8) rawData.at(i+(packetSize-3)) << 8 | (quint8) rawData.at(i+packetSize-2))/1000.0);
                    ChannelData[12].replace(dyno_index, dyno_data);
                    if(RecordEnabled){
                        RecordData((quint8) rawData.at(i+(packetSize-4)));
                        RecordData((quint8) rawData.at(i+(packetSize-3)) << 8 | (quint8) rawData.at(i+(packetSize-2)));
                        recordCommand();
                        RecordData(END_OF_LINE);
                    }
                    index++;
                }
                else{
                    resync_count++;
                    if(resync_count > 50){
                        resync_count = 0;
                        qDebug() << "Resynced!";
                        emit dataLost();
                    }
                }
            }
        }
        for(int i = lastFrameMarker; i < rawData.size(); i++){
            leftOverData.append(rawData.at(i));
        }
    }
//    playAudio(getAudioChannel());
//    playAudio(5);
    return rawData.size();
}

bool DataProcessor::checkNextFrameMarker(QByteArray data, int mark){
        if((quint8) data.at(mark) == 165 &&
                ((quint8) data.at(mark+1+(NUM_CHANNELS*2)) == 0 || (quint8) data.at(mark+1+(NUM_CHANNELS*2)) == 255) &&
                data.at(mark+4+(NUM_CHANNELS*2)) == 90)
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

int DataProcessor::getResyncCounter(){
    return resync_count;
}

void DataProcessor::appendDynoData(double data){
//    if(dyno_start){
//        ChannelData[12].replace(dyno_index, data);
//        if(dyno_index > retrieveDyno_XAxis().size()-1){
//            dyno_index = 0;
//        }
//        else{
//            dyno_index++;
//        }
//    }
//    else{
//        dyno_store.append(data);
//    }
    dyno_data = data;
//    qDebug() << data;
}

void DataProcessor::sortADCData(QByteArray adcData){
    for(int i = 0; i < adcData.size(); i++){
        audioBuffer[10].append(adcData.at(i));
        ADC_Data.append(adcData.at(i));
    }
}

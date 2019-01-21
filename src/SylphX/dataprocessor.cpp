#include "dataprocessor.h"

DataProcessor::DataProcessor(DataStream *dataStream_){
    dataStream = dataStream_;
}

int DataProcessor::parseFrameMarkers(QByteArray rawData){
    for(int i = 0; i < rawData.size(); i = i + packetSize){
        if(index > getNumDataPoints()){
            index = 0;
        }
        for(int j = 2; j < NUM_CHANNELS; j++){
            fullWord_rawData = ((quint8) rawData.at(i+((2*j))) << 8 | (quint8) rawData.at(i+((2*j)+1)))-32768;
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
            ChannelData[j-2].replace(index, fullWord_rawData*(0.000000195)*multiplier);
//            if(dataStream->getStreamConnected(j-2)){
//                dataStream->appendData(j-2, fullWord_rawData*(0.000000195));
//            }
//            appendAudioBuffer(j-2, rawData.at(i+((2*j))), rawData.at(i+((2*j)+1)));
        }
        for(int j = 0; j < 2; j++){
            fullWord_rawData = ((quint8) rawData.at(i+((2*j))) << 8 | (quint8) rawData.at(i+((2*j)+1)))-32768;
            if(RecordEnabled){
                RecordData(fullWord_rawData);
            }
            ChannelData[j+(NUM_CHANNELS-2)].replace(index, fullWord_rawData*(0.000000195)*multiplier);
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
        ChannelData[11].replace(index, (quint8) rawData.at(i+(packetSize-4)) << 8 | (quint8) rawData.at(i+packetSize-3));
        if(RecordEnabled){
            RecordData((quint8) rawData.at(i+packetSize-5));
            RecordData((quint8) rawData.at(i+(packetSize-4)) << 8 | (quint8) rawData.at(i+(packetSize-3)));
            recordCommand();
            RecordData(END_OF_LINE);
        }
        index++;
    }
//    playAudio(getAudioChannel());
    return rawData.size();
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
            if (i%packetSize == firstFrameMarker && checkNextFrameMarker(rawData, i)){
                if(index > getNumDataPoints()){
                    index = 0;
                }
                for(int j = 2; j < NUM_CHANNELS; j++){
                    fullWord_rawData = ((quint8) rawData.at(i+1+((2*j))) << 8 | (quint8) rawData.at(i+1+((2*j)+1)))-32768;
//                    appendAudioBuffer(j-2, rawData.at(i+1+((2*j))), rawData.at(i+1+((2*j)+1)));
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
//                    appendAudioBuffer(j+8, rawData.at(i+1+((2*j))), rawData.at(i+1+((2*j)+1)));
                    if(RecordEnabled){
                        RecordData(fullWord_rawData);
                    }
                    ChannelData[j+(NUM_CHANNELS-2)].replace(index, fullWord_rawData*(0.000000195)*multiplier);
//                    if(dataStream->getStreamConnected(j+(NUM_CHANNELS-2))){
//                        dataStream->appendData(j+(NUM_CHANNELS-2), fullWord_rawData*(0.000000195));
//                    }
//                    ChannelData[j+(NUM_CHANNELS-2)].replace(index, 0);
                }

//                for(int j = 0; j < 10; j++){
//                    if(dataStream->getChannelSize(i) > 20480){
//                        dataStream->clearChannelData(i);
//                    }
//                }

                ChannelData[10].replace(index, (quint8) rawData.at(i+packetSize-4));
                ChannelData[11].replace(index, (quint8) rawData.at(i+(packetSize-3)) << 8 | (quint8) rawData.at(i+packetSize-2));
                if(RecordEnabled){
                    RecordData((quint8) rawData.at(i+(packetSize-4)));
                    RecordData((quint8) rawData.at(i+(packetSize-3)) << 8 | (quint8) rawData.at(i+(packetSize-2)));
                    recordCommand();
                    RecordData(END_OF_LINE);
                }
                index++;
            }
        }
        for(int i = lastFrameMarker; i < rawData.size(); i++){
            leftOverData.append(rawData.at(i));
        }
    }
//    playAudio(getAudioChannel());
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

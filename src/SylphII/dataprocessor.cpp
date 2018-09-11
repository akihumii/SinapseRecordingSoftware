#include "dataprocessor.h"

DataProcessor::DataProcessor(float samplingRate_){
    samplingRate = samplingRate_;
    period = 1/samplingRate_;
}

int DataProcessor::parseFrameMarkers(QByteArray rawData){
    for(int i = 0; i < rawData.size(); i = i + packetSize){
        if(index > getNumDataPoints()){
            index = 0;
        }
        for(int j = 0; j < NUM_CHANNELS; j++){
            fullWord_rawData = ((quint8) rawData.at(i+((2*j)+1)) << 8 | (quint8) rawData.at(i+((2*j)+2)))-32768;
            if(RecordEnabled){
                RecordData(fullWord_rawData);
            }
            ChannelData[j].replace(index, fullWord_rawData*(0.000000195)*multiplier);
//            appendAudioBuffer(j, rawData.at(i+((2*j))), rawData.at(i+((2*j)+1)));
        }

//        ChannelData[10].replace(index, (quint8) rawData.at(i+packetSize-5));
        ChannelData[2].replace(index, (quint8) rawData.at(i+(packetSize-2)) << 8 | (quint8) rawData.at(i+packetSize-1));
        if(RecordEnabled){
            RecordData((quint8) rawData.at(i+(packetSize-2)) << 8 | (quint8) rawData.at(i+(packetSize-1)));
            RecordData(END_OF_LINE);
        }
        index++;
    }
//    playAudio(getAudioChannel());
    return rawData.size();

}

int DataProcessor::parseFrameMarkersWithChecks(QByteArray rawData){
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
                if(index > getNumDataPoints()){
                    index = 0;
                }
                for(int j = 0; j < NUM_CHANNELS; j++){
                    fullWord_rawData = ((quint8) rawData.at(i+1+((2*j))) << 8 | (quint8) rawData.at(i+1+((2*j)+1)))-32768;
//                    appendAudioBuffer(j+8, rawData.at(i+1+((2*j))), rawData.at(i+1+((2*j)+1)));
                    if(RecordEnabled){
                        RecordData(fullWord_rawData);
                    }
                    ChannelData[j].replace(index, fullWord_rawData*(0.000000195)*multiplier);
//                    dataStream->appendData(j, fullWord_rawData*(0.000000195));
                }

                for(int j = 0; j < 10; j++){
                    for(int i = 0; i < 8; i++){
                        appendAudioBuffer(j, 0, 0);
                    }
                }

                ChannelData[3].replace(index, (quint8) rawData.at(i+(packetSize-1)) << 8 | (quint8) rawData.at(i+packetSize-0));
                if(RecordEnabled){
                    RecordData((quint8) rawData.at(i+(packetSize-1)) << 8 | (quint8) rawData.at(i+(packetSize)));
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

//bool DataProcessor::checkNextFrameMarker(QByteArray data, int currentIndex){
//    if(((quint8) data.at(currentIndex + 5) == (quint8) data.at(currentIndex) + 1
//        && (quint8) data.at(currentIndex + 10) == (quint8) data.at(currentIndex + 5) + 1
//        && (quint8) data.at(currentIndex + 15) == (quint8) data.at(currentIndex + 10) + 1
//        && (quint8) data.at(currentIndex + 20) == (quint8) data.at(currentIndex + 15) + 1
//        && (quint8) data.at(currentIndex + 25) == (quint8) data.at(currentIndex + 20) + 1)){
//        return true;
//    }
//    else{
//        return false;
//    }
//}

bool DataProcessor::checkNextFrameMarker(QByteArray data, int mark){
    if((quint8) data.at(mark) + 1 == (quint8) data.at(mark+packetSize))
        return true;
    else
        return false;
}

int DataProcessor::findfirstFrameMarkers(QByteArray rawData){
//    qDebug() << "Finding first frame marker";
    for(int i = 0; i < rawData.size()-(packetSize*2); i++){
        if((quint8)rawData.at(i+(packetSize*2)) == (quint8)rawData.at(i+packetSize) + 1
                && (quint8)rawData.at(i+packetSize) == (quint8)rawData.at(i) + 1){
            return i;
        }
    }
    return 0;
}

int DataProcessor::findlastFrameMarkers(QByteArray rawData){
//    qDebug() << "Finding last frame marker";
    if(rawData.size()>(packetSize*2-2)){
        for(int i = rawData.size()-1; i > (packetSize*2-1); i--){
            if((quint8)rawData.at(i-packetSize) + 1 == (quint8)rawData.at(i)
                    && (quint8)rawData.at(i-(packetSize*2)) == (quint8)rawData.at(i-packetSize) + 1){
                return i;
            }
        }
    }
    return 0;
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

void DataProcessor::setSmartDataProcessor(bool flag){
    smartDataProcessor = flag;
}

bool DataProcessor::isSmart(){
    return smartDataProcessor;
}

void DataProcessor::setScale(int value){
    multiplier = value;
}

int DataProcessor::getScale(){
    return multiplier;
}


#include "dataprocessor.h"

DataProcessor::DataProcessor(){
}

void DataProcessor::setBitMode(bool BitMode){
    is8BitMode = BitMode;
}

//void DataProcessor::parseFrameMarkers(QByteArray rawData){
//    qDebug() << rawData.size();
//    if(leftOverData.size() > 0){
//        for(int i=leftOverData.size()-1;i>=0;i--){
//            rawData.prepend(leftOverData.at(i));
//        }
//        leftOverData.clear();
//    }
//    firstFrameMarker = findfirstFrameMarkers(rawData);
//    lastFrameMarker = findlastFrameMarkers(rawData);
//    if(lastFrameMarker > 0){
//        for(int i = 0; i < lastFrameMarker - 4; i = i + 1){
//            if (i%5 == firstFrameMarker && checkNextFrameMarker(rawData, i)){
//                fullWord_rawData = ((quint8) rawData.at(i+1) << 8 | (quint8) rawData.at(i+2))-32768;
//                appendAudioBuffer(0, rawData.at(i+2), rawData.at(i+1));
//                if(RecordEnabled){
//                    RecordData(fullWord_rawData);
//                }
//                ChannelData[0].append(fullWord_rawData*(0.000000195));

//                fullWord_rawData = ((quint8) rawData.at(i+3) << 8 | (quint8) rawData.at(i+4))-32768;
//                appendAudioBuffer(1, rawData.at(i+4), rawData.at(i+3));
//                if(RecordEnabled){
//                    RecordData(fullWord_rawData);
//                }
//                ChannelData[1].append(fullWord_rawData*(0.000000195));

//                if(RecordEnabled){
//                    if(ADC_Data.size()>0){
//                        RecordData(ADC_Data.at(0));
//                    }
//                    else{
//                        RecordData(0);
//                    }
//                    RecordData((quint8) rawData.at(i));
//                    RecordData(END_OF_LINE);
//                }
//                if(ADC_Data.size()>0){
//                    ChannelData[2].append(ADC_Data.at(0)/ 256.0 * 2.5);
//                    ADC_Data.remove(0, 1);
//                }
//                ChannelData[11].append((quint8) rawData.at(i));
//                total_data_count = total_data_count+1;
//                X_axis.append(total_data_count*0.000048);
//            }
//        }
//        for(int i = lastFrameMarker; i < rawData.size(); i++){
//            leftOverData.append(rawData.at(i));
//        }
//    }
//}

//bool DataProcessor::checkNextFrameMarker(QByteArray data, int currentIndex){
//    if((quint8) data.at(currentIndex + 5) == (quint8) data.at(currentIndex) + 1
//            || ((quint8) data.at(currentIndex) == 250 && (quint8) data.at(currentIndex + 5) == 0)){
//        return true;
//    }
//    else{
//        return false;
//    }
//}

//int DataProcessor::findfirstFrameMarkers(QByteArray rawData){
//    for(int i = 0; i < rawData.size()-10; i++){
//        if((quint8)rawData.at(i+5) == (quint8)rawData.at(i)+1
//                && (quint8)rawData.at(i+10) == (quint8)rawData.at(i+5)+1){
//            return i;
//        }
//        else if((quint8)rawData.at(i+10) == 1 && (quint8)rawData.at(i+5) == 0 && (quint8)rawData.at(i) == 250){
//            return i;
//        }
//        else if((quint8)rawData.at(i+10) == 0 && (quint8)rawData.at(i+5) == 250 && (quint8)rawData.at(i) == 249){
//            return i;
//        }
//    }
//    return 0;
//}

//int DataProcessor::findlastFrameMarkers(QByteArray rawData){
//    if(rawData.size()>8){
//        for(int i = rawData.size()-1; i > 9; i--){
//            if((quint8)rawData.at(i-5)+1 == (quint8)rawData.at(i) && (quint8)rawData.at(i-10)+1 == (quint8)rawData.at(i-5)){
//                return i;
//            }
//            else if((quint8)rawData.at(i-10) == 249 && (quint8)rawData.at(i-5) == 250 && (quint8)rawData.at(i) == 0){
//                return i;
//            }
//            else if((quint8)rawData.at(i-10) == 250 && (quint8)rawData.at(i-5) == 0 && (quint8)rawData.at(i) == 1){
//                return i;
//            }
//        }
//    }
//    return 0;
//}

void DataProcessor::parseFrameMarkers(QByteArray rawData){
//    qDebug() << rawData.size();
    if(leftOverData.size() > 0){
        for(int i=leftOverData.size()-1;i>=0;i--){
            rawData.prepend(leftOverData.at(i));
        }
        leftOverData.clear();
    }
    firstFrameMarker = findfirstFrameMarkers(rawData);
    lastFrameMarker = findlastFrameMarkers(rawData);
    if(lastFrameMarker > 0){
        for(int i = 0; i < lastFrameMarker - 20; i = i + 1){
            if (i%21 == firstFrameMarker && checkNextFrameMarker(rawData, i)){
                for(int j = 2; j < 10; j++){
                    fullWord_rawData = ((quint8) rawData.at(i+((2*j)+1)) << 8 | (quint8) rawData.at(i+((2*j)+2)))-32768;
                    appendAudioBuffer(j-2, rawData.at(i+(2*j)+2), rawData.at(i+(2*j)+1));
                    if(RecordEnabled){
                        RecordData(fullWord_rawData);
                    }
                    ChannelData[j-2].append(fullWord_rawData*(0.000000195));
                }
                for(int j = 0; j < 2; j++){
                    fullWord_rawData = ((quint8) rawData.at(i+((2*j)+1)) << 8 | (quint8) rawData.at(i+((2*j)+2)))-32768;
                    appendAudioBuffer(j+8, rawData.at(i+(2*j)+2), rawData.at(i+(2*j)+1));
                    if(RecordEnabled){
                        RecordData(fullWord_rawData);
                    }
                    ChannelData[j+8].append(fullWord_rawData*(0.000000195));
                }
                if(RecordEnabled){
                    RecordData((quint8) rawData.at(i));
                }
                if(ADC_Data.size()>4){
                    ChannelData[10].append(ADC_Data.at(0)/ 256.0 * 2.5);
                    if(RecordEnabled){
                        for(int i = 0; i < 5; i++){
                            *out << ADC_Data.at(i) << " , \n";
                        }
                    }
                    ADC_Data.remove(0, 5);
                }
                if(RecordEnabled){
                    RecordData(END_OF_LINE);
                }
                ChannelData[11].append((quint8) rawData.at(i));
                total_data_count = total_data_count+1;
                X_axis.append(total_data_count*0.000202);
            }
        }
        for(int i = lastFrameMarker; i < rawData.size(); i++){
            leftOverData.append(rawData.at(i));
        }
    }
    playAudio(getAudioChannel());
}

bool DataProcessor::checkNextFrameMarker(QByteArray data, int currentIndex){
    if((quint8) data.at(currentIndex + 21) == (quint8) data.at(currentIndex) + 1
            || ((quint8) data.at(currentIndex) == 250 && (quint8) data.at(currentIndex + 21) == 0)){
        return true;
    }
    else{
        return false;
    }
}

int DataProcessor::findfirstFrameMarkers(QByteArray rawData){
    for(int i = 0; i < rawData.size()-42; i++){
        if((quint8)rawData.at(i+42) == (quint8)rawData.at(i+21)+1
                && (quint8)rawData.at(i+21) == (quint8)rawData.at(i)+1){
            return i;
        }
        else if((quint8)rawData.at(i+42) == 1 && (quint8)rawData.at(i+21) == 0 && (quint8)rawData.at(i) == 250){
            return i;
        }
        else if((quint8)rawData.at(i+42) == 0 && (quint8)rawData.at(i+21) == 250 && (quint8)rawData.at(i) == 249){
            return i;
        }
    }
    return 0;
}

int DataProcessor::findlastFrameMarkers(QByteArray rawData){
    if(rawData.size()>40){
        for(int i = rawData.size()-1; i > 41; i--){
            if((quint8)rawData.at(i-21)+1 == (quint8)rawData.at(i) && (quint8)rawData.at(i-42)+1 == (quint8)rawData.at(i-21)){
                return i;
            }
            else if((quint8)rawData.at(i-42) == 249 && (quint8)rawData.at(i-21) == 250 && (quint8)rawData.at(i) == 0){
                return i;
            }
            else if((quint8)rawData.at(i-42) == 250 && (quint8)rawData.at(i-21) == 0 && (quint8)rawData.at(i) == 1){
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
    if(enableFlag){
        fileName = directory + QDateTime::currentDateTime().toString("'data_'yyyyMMdd_HHmmss'ADC.csv'");
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

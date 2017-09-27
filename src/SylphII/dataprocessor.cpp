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
        if(classifierEnabled && classifierChannel == 1){
            if(fullWord_rawData*(0.000000195) > classifierThreshold){
                startSavingData = true;
            }
            if(startSavingData){
                savedData.append(fullWord_rawData*(0.000000195));
            }
            if(savedData.size() > classifierWindowLength/period){
                classifyFeature(computeFeature(classifierChannel));
                startSavingData = false;
            }
        }

        fullWord_rawData = ((quint8) rawData.at(i+2) << 8 | (quint8) rawData.at(i+3))-32768;
        appendAudioBuffer(1, rawData.at(i+4), rawData.at(i+3));
        if(RecordEnabled){
            RecordData(fullWord_rawData);
        }
        ChannelData[1].append(fullWord_rawData*(0.000000195));
        if(classifierEnabled && classifierChannel == 2){
            if(fullWord_rawData*(0.000000195) > classifierThreshold){
                startSavingData = true;
            }
            if(startSavingData){
                savedData.append(fullWord_rawData*(0.000000195));
            }
            if(savedData.size() > classifierWindowLength/period){
                classifyFeature(computeFeature(classifierChannel));
                startSavingData = false;
            }
        }
        if(RecordEnabled){
//            if(ADC_Data.size()>0){
//                RecordData(ADC_Data.at(0));
//            }
//            else{
//                RecordData(0);
//            }
            RecordData((quint8) rawData.at(i+4));
            RecordData(END_OF_LINE);
        }
//        if(ADC_Data.size()>0){
//            ChannelData[2].append(ADC_Data.at(0)/ 256.0 * 2.5);
//            ADC_Data.remove(0, 1);
//        }
        ChannelData[3].append((quint8) rawData.at(i+4));
        total_data_count++;
        X_axis.append(total_data_count*period);
    }
//    playAudio(getAudioChannel());
}

float DataProcessor::computeFeature(int channel){
    float x = 0;
    for(int i = 0; i < savedData.size(); i++){
        if(savedData.at(i) > x){
            x = savedData.at(i);
        }
    }
    return x;
}

void DataProcessor::classifyFeature(float x){
    qDebug() << "Feature value: " << x;
    qDebug() << "K value: "<< classifierK;
    qDebug() << "L value: "<< classifierL;
    float temp = classifierK + x*classifierL;
    qDebug() << "X VALUE: " << temp;
    if(temp > 0){
        emit groupIsignal();
    }
    else{
        emit groupJsignal();
    }
    savedData.clear();
}

void DataProcessor::setClassifierK(float newValue){
    qDebug () << "Classifier value K: " << newValue;
    classifierK = newValue;
}

float DataProcessor::getClassifierK(){
    return classifierK;
}

void DataProcessor::setClassifierL(float newValue){
    qDebug () << "Classifier value L: " << newValue;
    classifierL = newValue;
}

float DataProcessor::getClassifierL(){
    return classifierL;
}

void DataProcessor::setClassifierWindowLength(float length){
    classifierWindowLength = length;
}

float DataProcessor::getClassifierWindowLength(){
    return classifierWindowLength;
}

void DataProcessor::setClassifierThreshold(float threshold){
    classifierThreshold = threshold;
}

float DataProcessor::getClassifierThreshold(){
    return classifierThreshold;
}

void DataProcessor::setClassifierChannel(int channel){
    classifierChannel = channel;
}

int DataProcessor::getClassifierChannel(){
    return classifierChannel;
}

void DataProcessor::setClassifierEnabled(bool flag){
    classifierEnabled = flag;
}

bool DataProcessor::getClassifierEnabled(){
    return classifierEnabled;
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


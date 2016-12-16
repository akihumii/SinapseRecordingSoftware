#include "dataprocessor_ma.h"

DataProcessor_MA::DataProcessor_MA(QObject *parent) : QObject(parent = Q_NULLPTR){
    QAudioFormat format;

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    format = info.preferredFormat();
    format.setChannelCount(1);
    format.setSampleRate(44100);
    format.setSampleType(QAudioFormat::SignedInt);
    format.setSampleSize(16);
    //format.setByteOrder(QAudioFormat::BigEndian);

//    foreach (const QAudioDeviceInfo &info, QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
//    {
//        qDebug() << "Supported Sample Rates: " << info.supportedSampleRates();
//        qDebug() << "Supported Byte Orders: " << info.supportedByteOrders();
//        qDebug() << "Supported Channel Counts: " << info.supportedChannelCounts();
//        qDebug() << "Supported Sample Size: " << info.supportedSampleSizes();
//        qDebug() << "Supported Sample Types: " << info.supportedSampleTypes();
//        qDebug() << "Preferred Format: " << info.preferredFormat();

//    }

    qDebug() << "Format set: " << format;

    if (!info.isFormatSupported(format)) {
        qDebug() << "Raw audio format not supported by backend, cannot play audio.";
        return;
    }

    audio = new QAudioOutput(format, this);
    qDebug() << "Volume: " << audio->volume();

    audio->setNotifyInterval(128);
    audioDevice->open(QIODevice::ReadWrite);
    audioDevice = audio->start();
//    out3 = new QDataStream(audioDevice);
    QDataStream out3(audioDevice);
    connect(audio, &QAudioOutput::stateChanged, this, &DataProcessor_MA::handleStateChanged);
    connect(audio, &QAudioOutput::notify, this, &DataProcessor_MA::onNotify);
}

void DataProcessor_MA::parseFrameMarkers(QByteArray rawData){
    if(leftOverData.size() > 0){
        for(int i=leftOverData.size()-1;i>=0;i--){
            rawData.prepend(leftOverData.at(i));
        }
        leftOverData.clear();
    }
    firstFrameMarker = findfirstFrameMarkers(rawData);
    lastFrameMarker = findlastFrameMarkers(rawData);
    if(lastFrameMarker > 0){
        for(int i=0;i<lastFrameMarker-4;i=i+1){
            if (i%5 == firstFrameMarker && checkNextFrameMarker(rawData, i)){
                fullWord_rawData = ((quint8) rawData.at(i+1) << 8 | (quint8) rawData.at(i+2))-32768;
                if(RecordEnabled){
                    RecordData(fullWord_rawData);
                }
                ChannelData[0].append(fullWord_rawData*(0.000000195));
                audioArray.append(fullWord_rawData);
                fullWord_rawData = ((quint8) rawData.at(i+3) << 8 | (quint8) rawData.at(i+4))-32768;
                if(RecordEnabled){
                    RecordData(fullWord_rawData);
                }
                ChannelData[1].append(fullWord_rawData*(0.000000195));
                if(RecordEnabled){
                    if(ADC_Data.size()>0){
                        RecordData(ADC_Data[0]);
                    }
                    else{
                        RecordData(0);
                    }
                    RecordData((quint8) rawData.at(i));
                    RecordData(END_OF_LINE);
                }
                if(ADC_Data.size()>0){
                    ChannelData[2].append(ADC_Data.at(0));
                    //*out3 << (quint8) ADC_Data.at(0);
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
    if(audioArray.size() > 128){
        qDebug() << (qint16) audioArray.at(0);
//        for(int i=0; i<audioArray.size();i++){
//            *out3<<audioArray;
//        }
        audioDevice->write(audioArray);
        audioArray.clear();
    }
}

bool DataProcessor_MA::checkNextFrameMarker(QByteArray data, int currentIndex){
    if(data.at(currentIndex + 5) == data.at(currentIndex) + 1){
        return true;
    }
    else{
        return false;
    }
}

int DataProcessor_MA::findfirstFrameMarkers(QByteArray rawData){
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

int DataProcessor_MA::findlastFrameMarkers(QByteArray rawData){
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

void DataProcessor_MA::sortADCData(QByteArray adcData){
    double temp;
    for(int i=0;i<adcData.size();i++){
        temp = ((quint8)adcData.at(i)/ 256.0) * 2.5;
        ADC_Data.append(temp);
    }
}

void DataProcessor_MA::handleStateChanged(QAudio::State newState)
{
    switch (newState) {
        case QAudio::IdleState:
            // Finished playing (no more data)
            qDebug() << "Idled";
//            audio->stop();
//            delete audio;
            break;

        case QAudio::StoppedState:
            // Stopped for other reasons
            if (audio->error() != QAudio::NoError) {
                // Error handling
                qDebug() << "Error: " << audio->error();
            }
            break;

        default:
            // ... other cases as appropriate
            break;
    }
}

void DataProcessor_MA::onNotify(){
    if(audioArray.size() > 0){
//        qDebug() << "Notified";
//        audioDevice->write(audioArray);
    }
}

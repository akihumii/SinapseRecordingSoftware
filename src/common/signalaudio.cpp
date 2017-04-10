#include "signalaudio.h"

SignalAudio::SignalAudio(){
    QAudioFormat format;

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    format = info.preferredFormat();
    format.setChannelCount(1);
    format.setSampleRate(8000);
    format.setSampleType(QAudioFormat::SignedInt);
    format.setSampleSize(sampleSize);

//    foreach (const QAudioDeviceInfo &info, QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
//    {
//        qDebug() << "Device name: " << info.deviceName();
//        qDebug() << "Supported Sample Rates: " << info.supportedSampleRates();
//        qDebug() << "Supported Byte Orders: " << info.supportedByteOrders();
//        qDebug() << "Supported Channel Counts: " << info.supportedChannelCounts();
//        qDebug() << "Supported Sample Size: " << info.supportedSampleSizes();
//        qDebug() << "Supported Sample Types: " << info.supportedSampleTypes();
//        qDebug() << "Preferred Format: " << info.preferredFormat();

//    }

//    qDebug() << "Format set: " << format;

    if (!info.isFormatSupported(format)) {
        qDebug() << "Raw audio format not supported by backend, cannot play audio.";
        return;
    }

    audio = new QAudioOutput(format, this);

    audio->setBufferSize(1000);
    audioDevice = audio->start();
    audio->setVolume(0.1);
    connect(audio, &QAudioOutput::stateChanged, this, &SignalAudio::handleStateChanged);
}

SignalAudio::~SignalAudio(){

}

void SignalAudio::appendAudioBuffer(int ChannelIndex, char MSB, char LSB){
    audioBuffer[ChannelIndex].append(MSB);
    audioBuffer[ChannelIndex].append(LSB);
}

bool SignalAudio::playAudio(int ChannelIndex){
    if(audioBuffer[ChannelIndex].size() >= 50 && audioBuffer[ChannelIndex].size()%2 == 0){
        if(audioDevice->write(audioBuffer[ChannelIndex])>0){
            clearAudioBuffer();
            return true;
        }
        else{
            clearAudioBuffer();
            return false;
        }
    }
    return false;
}

void SignalAudio::clearAudioBuffer(){
    for(int i = 0; i < 11; i++){
        audioBuffer[i].clear();
    }
}

void SignalAudio::handleStateChanged(QAudio::State newState)
{
    switch (newState) {
        case QAudio::IdleState:
            // Finished playing (no more data)
//            qDebug() << "Idled";
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

void SignalAudio::setAudioChannel(int ChannelIndex){
    clearAudioBuffer();
    audioChannelSelected = ChannelIndex;
}

int SignalAudio::getAudioChannel(){
    return audioChannelSelected;
}

void SignalAudio::setSampleRate(int rate){
    sampleRate = rate;
}

int SignalAudio::getSampleRate(){
    return sampleRate;
}

void SignalAudio::setAudioSampleSize(int size){
    sampleSize = size;
}

int SignalAudio::getAudioSampleSize(){
    return sampleSize;
}


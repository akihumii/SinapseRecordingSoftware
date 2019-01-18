#include "data.h"

Data::Data(){
    File = new QFile;
}

Data::~Data(){
}

QVector<double> Data::retrieveData(int ChannelIndex){
    return ChannelData[ChannelIndex];
}

QVector<double> Data::retrieveXAxis(){
    return X_axis;
}

void Data::clearChannelData(int ChannelIndex){
    ChannelData[ChannelIndex].remove(0,(ChannelData[ChannelIndex].size()));
}

void Data::clearallChannelData(){
    for(int i=0; i<12; i++){
        ChannelData[i].remove(0,ChannelData[i].size());
    }
}

bool Data::isEmpty(int ChannelIndex){
    return ChannelData[ChannelIndex].isEmpty();
}

void Data::removeXAxis(){
    X_axis.remove(0,(X_axis.size()));
}

void Data::setPlotEnabled(bool enableFlag){
    PlotEnabled = enableFlag;
}

bool Data::isPlotEnabled(){
    return PlotEnabled;
}

void Data::setRecordEnabled(bool enableFlag){
    if(enableFlag){
        fileName = directory + QDateTime::currentDateTime().toString("'data_'yyyyMMdd_HHmmss'.csv'");
        File->setFileName(fileName);
        if(File->open(QIODevice::WriteOnly|QIODevice::Text)){
            qDebug()<< "File opened";
        }
        else{
            qDebug() << "File failed to open";
        }
        out = new QTextStream(File);
        recordHeader();
        qDebug() << "setting Record Enabled";
    }
    else{
        File->close();
        qDebug() << "setting Record disabled";
    }
    RecordEnabled = enableFlag;
}

bool Data::isRecordEnabled(){
    return RecordEnabled;
}

void Data::recordHeader(){
    *out << headerSettingString;
}

void Data::setHeader(QString header){
    headerSettingString = header;
}

void Data::RecordData(int data){
    if(data == END_OF_LINE){
        *out << "\n";
    }
    else{
        *out << data << " ,";
    }
}

void Data::RecordData(double data){
    if(data == END_OF_LINE){
        *out << "\n";
    }
    else{
        *out << data << " ,";
    }
}

void Data::recordCommand(){
    RecordData((quint8) lastSentByte[0]);
    RecordData((quint8) lastSentByte[1]);
    RecordData((double) lastSentAmplitudes[0]);
    RecordData((double) lastSentAmplitudes[1]);
    RecordData((double) lastSentAmplitudes[2]);
    RecordData((double) lastSentAmplitudes[3]);
}

void Data::setFileName(QString filename){
    fileName = filename;
}

QString Data::getFileName(){
    return File->fileName();
}

void Data::setDirectory(QString dir){
    if(dir == NULL){
        directory = QDir::homePath() + "/Desktop/";
    }
    else{
        directory = dir + "/";
    }
}
QString Data::getDirectory(){
    return directory;
}

int Data::getNumDataPoints(){
    return numDataPoints;
}

void Data::setNumDataPoints(int timeFrames, double sampleFreq){
    qDebug() << sampleFreq;
    switch(timeFrames){
    case TimeFrames10ms:
        numDataPoints = 0.01/(1/sampleFreq);
        break;
    case TimeFrames20ms:
        numDataPoints = 0.02/(1/sampleFreq);
        break;
    case TimeFrames50ms:
        numDataPoints = 0.05/(1/sampleFreq);
        break;
    case TimeFrames100ms:
        numDataPoints = 0.1/(1/sampleFreq);
        break;
    case TimeFrames200ms:
        numDataPoints = 0.2/(1/sampleFreq);
        break;
    case TimeFrames500ms:
        numDataPoints = 0.5/(1/sampleFreq);
        break;
    case TimeFrames1000ms:
        numDataPoints = 1/(1/sampleFreq);
        break;
    case TimeFrames2000ms:
        numDataPoints = 2/(1/sampleFreq);
        break;
    case TimeFrames5000ms:
        numDataPoints = 5/(1/sampleFreq);
        break;
    default:
        numDataPoints = 0.1/(1/sampleFreq);
    }
    for(int i = 0; i < 12; i++){
        ChannelData[i].reserve(numDataPoints+1);
        ChannelData[i].resize(numDataPoints+1);
    }
    X_axis.reserve(numDataPoints+1);
    X_axis.resize(numDataPoints+1);
    for(int i = 0; i < numDataPoints+1; i++){
        X_axis.replace(i, i*(1.0/sampleFreq));
//        qDebug() << X_axis.at(i);
    }
}

void Data::setLastSentBytes(char *bytes){
    lastSentByte[0] = bytes[0];
    lastSentByte[1] = bytes[1];
}

void Data::setLastSentAmplitudes(double *amplitudes){
    lastSentAmplitudes[0] = amplitudes[0];
    lastSentAmplitudes[1] = amplitudes[1];
    lastSentAmplitudes[2] = amplitudes[2];
    lastSentAmplitudes[3] = amplitudes[3];
}

void Data::setDebounce(int value){
    qDebug() << "Setting debounce value : " << value;
    debounce = value;
}

void Data::setUpperThreshold(double value){
    qDebug() << "Setting upper threshold : " << value;
    upperThreshold = value;
}

void Data::setLowerThreshold(double value){
    qDebug() << "Setting lower threshold : " << value;
    lowerThreshold = value;
}

int Data::getDebounce(){
    return debounce;
}

void Data::setSmartDataProcessor(bool flag){
    smartDataProcessor = flag;
}

bool Data::isSmart(){
    return smartDataProcessor;
}

void Data::setScale(int value){
    multiplier = value;
}

int Data::getScale(){
    return multiplier;
}

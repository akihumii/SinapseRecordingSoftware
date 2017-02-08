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
    for(int i=0; i<4; i++){
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

void Data::RecordData(int data){
    if(data == END_OF_LINE){
        *out << "\n";
    }
    else{
        *out << data << " ,";
    }
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
    switch(timeFrames){
    case TimeFrames10ms:
        numDataPoints = 0.01*1/sampleFreq;
        break;
    case TimeFrames20ms:
        numDataPoints = 0.01*1/sampleFreq;
        break;
    case TimeFrames50ms:
        numDataPoints = 0.01*1/sampleFreq;
        break;
    case TimeFrames100ms:
        numDataPoints = 0.01*1/sampleFreq;
        break;
    case TimeFrames200ms:
        numDataPoints = 0.01*1/sampleFreq;
        break;
    case TimeFrames500ms:
        numDataPoints = 0.01*1/sampleFreq;
        break;
    case TimeFrames1000ms:
        numDataPoints = 0.01*1/sampleFreq;
        break;
    case TimeFrames2000ms:
        numDataPoints = 0.01*1/sampleFreq;
        break;
    case TimeFrames5000ms:
        numDataPoints = 0.01*1/sampleFreq;
        break;
    default:
        numDataPoints = 2082;
    }
}


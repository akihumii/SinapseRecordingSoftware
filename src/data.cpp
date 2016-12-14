#include "data.h"

Data::Data(){
    File = new QFile("Edison.txt");
}

QVector<double> Data::retrieveData(int ChannelIndex){
        if(isFilterEnabled() && ChannelIndex != 3){
            ChannelData[ChannelIndex] = filterData(ChannelData[ChannelIndex], ChannelIndex);
        }
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
        File->open(QIODevice::WriteOnly|QIODevice::Text);
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

void Data::setFileName(QString fileName){
    if(fileName.contains(".txt")){
        File->setFileName(fileName);
    }
    else{
        File->setFileName(fileName+".txt");
    }
}

QString Data::getFileName(){
    return File->fileName();
}

int Data::getNumDataPoints(){
    return numDataPoints;
}

void Data::setNumDataPoints(int timeFrames){
    switch(timeFrames){
    case TimeFrames10ms:
        numDataPoints = 208;
        break;
    case TimeFrames20ms:
        numDataPoints = 416;
        break;
    case TimeFrames50ms:
        numDataPoints = 1041;
        break;
    case TimeFrames100ms:
        numDataPoints = 2082;
        break;
    case TimeFrames200ms:
        numDataPoints = 4164;
        break;
    case TimeFrames500ms:
        numDataPoints = 10411;
        break;
    case TimeFrames1000ms:
        numDataPoints = 20822;
        break;
    case TimeFrames2000ms:
        numDataPoints = 41644;
        break;
    case TimeFrames5000ms:
        numDataPoints = 104110;
        break;
    default:
        numDataPoints = 1041;
    }
}


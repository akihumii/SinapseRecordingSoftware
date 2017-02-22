#include "chunkprocessor.h"

ChunkProcessor::ChunkProcessor(QStringList *loadedData_, int numChannels_, int threadID_){
    loadedData = loadedData_;
    numChannels = numChannels_;
    threadID = threadID_;
}

void ChunkProcessor::run(){
    QVector<QVector<double>> parsedData;
    parsedData.resize(numChannels);
    for(int i = 0; i < loadedData->size(); i++){
        for(int j = 0; j < numChannels; j++){
            parsedData[j].append(loadedData->at(i).split(" ,")[j].toInt()*1.2/256);
        }
    }
    emit finishedChunk(parsedData, threadID);
}

ChunkProcessor::~ChunkProcessor(){

}

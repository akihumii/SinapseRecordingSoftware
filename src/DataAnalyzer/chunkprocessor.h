#ifndef CHUNKPROCESSOR_H
#define CHUNKPROCESSOR_H

#include <QObject>
#include "qtincludes.h"

class ChunkProcessor : public QObject, public QRunnable
{
    Q_OBJECT
public:
    ChunkProcessor(QStringList *loadedData_, int numChannels_, int threadID_);
    ~ChunkProcessor();
    void run();
signals:
    void finishedChunk(QVector<QVector<double>>parsedData, int threadID);
private:
    int numChannels = 0;
    int threadID = 0;
    QStringList *loadedData;
};

#endif // CHUNKPROCESSOR_H

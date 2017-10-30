#ifndef LOOPINGTHREAD_H
#define LOOPINGTHREAD_H

#include <QThread>
#include "../common/qtincludes.h"
#include "commandodin.h"
#include <QThread>
//#include <windows.h>

namespace Odin {

class LoopingThread : public QThread{
    Q_OBJECT
public:
    int num = 50;
    int delay = 3000;
    bool send = false;
    LoopingThread(){
    }

    void run() Q_DECL_OVERRIDE {
        QThread::msleep(2000);
//        qDebug() << "Sleeping for 2 seconds " << num;
        QMutex mutex;
        mutex.lock();
        for(int i = 0; i < num; i++){
//            qDebug() << "Looping in looping thread , " << i;
//            qDebug() << delay;
            if(this->send){
//                qDebug() << "Sending Command" << delay;
                emit commandSent();
                QThread::msleep(delay);
            }
            else{
//                qDebug() << "Breaking out";
                break;
            }
        }
        mutex.unlock();
        emit finishedSending();
//        qDebug() << "Finished sending everything";
    }
signals:
    void commandSent();
    void finishedSending();

private slots:
};

}

#endif // LOOPINGTHREAD_H

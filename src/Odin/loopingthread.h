#ifndef LOOPINGTHREAD_H
#define LOOPINGTHREAD_H

#include <QThread>
#include "../common/qtincludes.h"
#include "commandodin.h"
#include <QThread>
#include <windows.h>

class LoopingThread : public QThread{
    Q_OBJECT
public:
    int num = 50;
    int delay = 3000;
    bool send = false;
    LoopingThread(){
    }

    void run() Q_DECL_OVERRIDE {
        QThread::msleep(1700);
        for(int i = 0; i < num; i++){
            if(this->send){
                emit commandSent();
                qDebug() << "Sending Command" << delay;
                QThread::msleep(delay);
            }
            else{
                qDebug() << "Breaking out";
                break;
            }
        }
        emit finishedSending();
        qDebug() << "Finished sending everything";
    }
signals:
    void commandSent();
    void finishedSending();

private slots:
};

#endif // LOOPINGTHREAD_H

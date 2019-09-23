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
    int num = 1;
    int delay = 3000;
    bool send = false;
    LoopingThread(){
    }

    void run() Q_DECL_OVERRIDE {
        QThread::msleep(delay);
        for(int i = 0; i < num; i++){
            if(this->send){
                emit commandSent();
            }
            else{
                break;
            }
        }
        emit finishedSending();
    }
signals:
    void commandSent();
    void finishedSending();

private slots:
};

}

#endif // LOOPINGTHREAD_H

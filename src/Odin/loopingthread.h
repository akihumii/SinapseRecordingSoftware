#ifndef LOOPINGTHREAD_H
#define LOOPINGTHREAD_H

#include <QThread>
#include "../common/qtincludes.h"
#include "commandodin.h"

class LoopingThread : public QThread{
    Q_OBJECT
public:
    CommandOdin* commandOdin;
    int num = 50;
    int delay = 3000;
    bool send = false;
    LoopingThread(CommandOdin *commandOdin_){
        commandOdin = commandOdin_;
    }

    void run() Q_DECL_OVERRIDE {
        for(int i = 0; i < num; i++){
            if(this->send){
                commandOdin->sendCommand();
                emit commandSent();
                qDebug() << "Sending Command";
                this->msleep(delay);
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
};

#endif // LOOPINGTHREAD_H

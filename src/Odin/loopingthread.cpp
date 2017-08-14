#include "loopingthread.h"

LoopingThread::LoopingThread()
{

}

void LoopingThread::run()
{
    QThread::msleep(2000);
    //qDebug() << "sleeping for 2 second" << num;
    mutex.lock();
    for(int i = 0; i < num; i++){
        //qDebug() << "Looping in looping thread ," << i;
        //qDebug() << delay;
        if(this -> send){
            //qDebug() << "sending command" << delay;
            emit commandSent();
            QThread::msleep(delay);
        }
        else{
            //qDebug() << "breaking out";
            break;
        }
    }
    mutex.unlock();
    emit finishedSending();
    //qDebug() << "finished sending everything;
}

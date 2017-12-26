#ifndef DELAYTHREAD_H
#define DELAYTHREAD_H

#include <QThread>
#include "../common/qtincludes.h"
#include <QThread>

class DelayThread : public QThread{
    Q_OBJECT
public:
    int delay = 3000;
    DelayThread(){
    }

    void run() Q_DECL_OVERRIDE {
        this->msleep(delay);
        qDebug() << "From worker thread: " << currentThreadId();
        qDebug() << "3 Seconds passed";
//        emit timesUp();
    }
signals:
//    void timesUp();

private slots:
};

#endif // DELAYTHREAD_H

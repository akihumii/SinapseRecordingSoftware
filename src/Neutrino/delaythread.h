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
        qDebug() << delay << " Milli-Seconds passed";
    }
signals:

private slots:
};

#endif // DELAYTHREAD_H

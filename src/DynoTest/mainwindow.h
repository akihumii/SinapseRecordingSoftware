#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qtincludes.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();
private:
    QTcpServer *server;
    QTcpSocket *socketAbstract;
    QString currentTime;
    QString previousTime;
    QMediaPlayer *player;
    QFile *file;
    QTextStream *out;
    QLabel *label;
    int count = 1;
    QByteArray temp;
    float min = 0.0, max = 0.0;
private slots:
    void motionDetected();
    void on_predictionPort_connect();
};

#endif // MAINWINDOW_H

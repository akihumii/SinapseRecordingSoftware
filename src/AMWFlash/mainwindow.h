#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../common/qtincludes.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow();
    ~MainWindow();

private:
    QSerialPort *amwPort;
    QList<QSerialPortInfo> portInfo;
    void createLayout();

private slots:
    void readFeedback();
    void on_browse_clicked();
};

#endif // MAINWINDOW_H

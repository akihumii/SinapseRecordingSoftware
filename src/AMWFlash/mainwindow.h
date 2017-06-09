#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../common/qtincludes.h"
#include "serialportdialog.h"
#include "console.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow();
    ~MainWindow();

private:
    QSerialPort *amwPort;
    QList<QSerialPortInfo> portInfo;

    QFile *File;
    Console *console;
    QPushButton *browse;
    QPushButton *flash;

    bool savereboot = false;

    void createLayout();

private slots:
    void readFeedback();
    void on_browse_clicked();
    void on_flash_clicked();
};

#endif // MAINWINDOW_H

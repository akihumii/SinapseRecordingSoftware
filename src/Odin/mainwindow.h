#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include "../common/qtincludes.h"
#include "serialodin.h"
#include "commandodin.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow();
    ~MainWindow();
public slots:

private:
    SerialOdin *serialOdin;
    CommandOdin *commandOdin;
    QPushButton *testButton;
private slots:
    void sendCommand();
};

#endif // MAINWINDOW_H

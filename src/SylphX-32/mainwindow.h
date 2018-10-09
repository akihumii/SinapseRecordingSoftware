#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../common/qtincludes.h"
#include "../common/qcustomplot.h"

class MainWindow : public QMainWindow, public QThread
{
    Q_OBJECT

public:
    explicit MainWindow();
    ~MainWindow();


};

#endif // MAINWINDOW_H

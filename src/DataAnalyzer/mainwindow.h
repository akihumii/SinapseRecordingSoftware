#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qtincludes.h"
#include "../common/qcustomplot.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QString filename);
    ~MainWindow();

private:
    QCustomPlot *dataGraph[10];

    QCheckBox *dataEnable[10];


};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_MA_H
#define MAINWINDOW_MA_H

#include <QMainWindow>
#include "qtincludes.h"
#include "qcustomplot.h"
#include "data.h"
#include "filter.h"
#include "signalchannel.h"
#include "serialportdialog.h"
#include "serialchannel.h"
#include "filenamedialog.h"
#include "filterdialog.h"

class QComboBox;
class QCustomPlot;

class MainWindow_MA : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow_MA();
    ~MainWindow_MA();
    QLabel *statusBarLabel;
    SerialChannel *serialChannel;
    DataProcessor_MA *data;
public slots:

private slots:
    void updateData();
    void on_filterConfig_trigger();
    void on_record_triggered();
    void on_recordFileName_triggered();
    void on_resetRange_triggered();
    void on_timeFrame10_triggered();
    void on_timeFrame20_triggered();
    void on_timeFrame50_triggered();
    void on_timeFrame100_triggered();
    void on_timeFrame200_triggered();
    void on_timeFrame500_triggered();
    void on_timeFrame1000_triggered();
    void on_timeFrame2000_triggered();
    void on_timeFrame5000_triggered();
    void on_voltage50u_triggered();
    void on_voltage100u_triggered();
    void on_voltage200u_triggered();
    void on_voltage500u_triggered();
    void on_voltage1000u_triggered();
    void on_voltage2000u_triggered();
    void on_voltage5000u_triggered();
    void about();
    void resetGraph1Range();
    void resetGraph2Range();
    void resetGraph3Range();
    void resetGraph4Range();

private:
    int numDataPoints;

    QElapsedTimer timer;
    QTimer dataTimer;

    QList<QSerialPortInfo> portInfo;

    QString connectionStatus;

    QCPRange *defaultRange;

    void createStatusBar();
    void createLayout();
    void createActions();
    void createMenus();
    QMenu *fileMenu;
    QMenu *layoutMenu;
    QMenu *timeFrameMenu;
    QMenu *voltageMenu;
    QMenu *helpMenu;

    QCustomPlot *channelGraph[10];

    QAction *serialPortAction;
    QAction *filterAction;
    QAction *aboutAction;

    QAction *exitAction;
    QAction *recordAction;
    QAction *recordFileNameAction;

    QAction *timeFrame10ms;
    QAction *timeFrame20ms;
    QAction *timeFrame50ms;
    QAction *timeFrame100ms;
    QAction *timeFrame200ms;
    QAction *timeFrame500ms;
    QAction *timeFrame1000ms;
    QAction *timeFrame2000ms;
    QAction *timeFrame5000ms;

    QActionGroup *timeFrameGroup;

    QAction *voltage50u;
    QAction *voltage100u;
    QAction *voltage200u;
    QAction *voltage500u;
    QAction *voltage1000u;
    QAction *voltage2000u;
    QAction *voltage5000u;

    QActionGroup *voltageGroup;

    QAction *resetDefaultRange;

    QStatusBar *statusBarMainWindow;
};

#endif // MAINWINDOW_MA_H

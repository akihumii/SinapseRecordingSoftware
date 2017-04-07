#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "socketsylph.h"
#include "../common/qtincludes.h"
#include "../common/qcustomplot.h"
#include "dataprocessor.h"
#include "../common/data.h"
#include "../common/filter.h"
#include "serialportdialog.h"
#include "serialchannel.h"
#include "../common/filterdialog.h"


class QComboBox;
class QCustomPlot;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow();
    ~MainWindow();
    QLabel *statusBarLabel;

    SerialChannel *serialChannel;
    DataProcessor *data;

public slots:

private:
    QElapsedTimer timer;
    QTimer dataTimer;

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

    QStatusBar *statusBarMainWindow;

    QMenu *fileMenu;
    QMenu *voltageMenu;
    QMenu *layoutMenu;
    QMenu *timeFrameMenu;

    void createStatusBar();
    void createActions();
    void createMenus();
    void createLayout();
    void connectSylph();

    QAction *exitAction;
    QAction *recordAction;
    QAction *chooseDirectoryAction;
    QAction *restartAction;

    QAction *pauseAction;
    bool pause = false;

    QAction *resetDefaultX;
    QAction *resetDefaultY;
    QAction *swapAction;
    QAction *filterAction;
    QAction *dataAnalyzerAction;

    QAction *voltage50u;
    QAction *voltage100u;
    QAction *voltage200u;
    QAction *voltage500u;
    QAction *voltage1000u;
    QAction *voltage2000u;
    QAction *voltage5000u;

    QActionGroup *voltageGroup;

    int numDataPoints;

    QList<QSerialPortInfo> portInfo;

    QString connectionStatus;

    QCPRange *defaultRange;

    QMenu *helpMenu;
    QMenu *audioOutputMenu;

    QCustomPlot *channelGraph[10];
    SocketSylph *socketSylph;

    QAction *serialPortAction;
    QAction *aboutAction;

    QAction *audio1;
    QAction *audio2;
    QAction *audio3;

    QActionGroup *audioGroup;

private slots:
    void updateData();
    void on_timeFrame10_triggered();
    void on_timeFrame20_triggered();
    void on_timeFrame50_triggered();
    void on_timeFrame100_triggered();
    void on_timeFrame200_triggered();
    void on_timeFrame500_triggered();
    void on_timeFrame1000_triggered();
    void on_timeFrame2000_triggered();
    void on_timeFrame5000_triggered();
    void on_resetY_triggered();
    void on_voltage50u_triggered();
    void on_voltage100u_triggered();
    void on_voltage200u_triggered();
    void on_voltage500u_triggered();
    void on_voltage1000u_triggered();
    void on_voltage2000u_triggered();
    void on_voltage5000u_triggered();
    void on_dataAnalyzer_triggered();
    void on_record_triggered();
    void on_chooseDirectory_triggered();
    void on_playPause_triggered();
    void on_resetX_triggered();
    void on_swap_triggered();
    void on_filterConfig_trigger();
    void on_restart_triggered();

    void about();
    void resetGraph1Range();
    void resetGraph2Range();
    void resetGraph3Range();
    void resetGraph4Range();
    void on_graph1_clicked();
    void on_graph2_clicked();
    void on_graph3_clicked();
};

#endif // MainWindow_H

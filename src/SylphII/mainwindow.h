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

#define DEFAULT_XAXIS 7
#define DEFAULT_YAXIS 3

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

    int restartCount = 0;
    float samplingRate = 18200.0;
    float period = 1/samplingRate;
    bool pause = false;

    QStatusBar *statusBarMainWindow;

    QMenu *fileMenu;
    QMenu *voltageMenu;
    QMenu *timeFrameMenu;
    QMenu *audioOutputMenu;
    QMenu *processorMenu;
    QMenu *helpMenu;

    QAction *exitAction;
    QAction *recordAction;
    QAction *chooseDirectoryAction;
    QAction *restartAction;
    QAction *pauseAction;
    QAction *resetDefaultX;
    QAction *resetDefaultY;
    QAction *swapAction;
    QAction *filterAction;
    QAction *dataAnalyzerAction;
    QAction *timeFrame10ms;
    QAction *timeFrame20ms;
    QAction *timeFrame50ms;
    QAction *timeFrame100ms;
    QAction *timeFrame200ms;
    QAction *timeFrame500ms;
    QAction *timeFrame1000ms;
    QAction *timeFrame2000ms;
    QAction *timeFrame5000ms;
    QAction *voltage50u;
    QAction *voltage100u;
    QAction *voltage200u;
    QAction *voltage500u;
    QAction *voltage1000u;
    QAction *voltage2000u;
    QAction *voltage5000u;
    QAction *voltage10000u;
    QAction *audio1;
    QAction *audio2;
    QAction *audio3;
    QAction *aboutAction;

    QActionGroup *timeFrameGroup;
    QSignalMapper *timeFrameMapper;
    QAction *timeFrameAction[9];
    QString timeFrameActionNames[9] = { "10 milliseconds",
                                            "20 milliseconds",
                                            "50 milliseconds",
                                            "100 milliseconds",
                                            "200 milliseconds",
                                            "500 milliseconds",
                                            "1 second",
                                            "2 seconds",
                                            "5 seconds" };
    double timeFrameSteps[9] = {    0.001,
                                    0.002,
                                    0.005,
                                    0.01,
                                    0.02,
                                    0.05,
                                    0.1,
                                    0.2,
                                    0.5};

    QSignalMapper *voltageMapper;
    QAction *voltageAction[8];
    QString voltageActionNames[8] = { "+/- 50uV",
                                      "+/- 100uV",
                                      "+/- 200uV",
                                      "+/- 500uV",
                                      "+/- 1mV",
                                      "+/- 2mV",
                                      "+/- 5mV",
                                      "+/- 10mV"};
    double voltageMin[8] =   { -50,
                               -100,
                               -200,
                               -500,
                               -1,
                               -2,
                               -5,
                               -10};
    double voltageRange[8] = { 100,
                               200,
                               400,
                               1000,
                               2,
                               4,
                               10,
                               20};
    double voltageStep[8] = { 10,
                              20,
                              40,
                              100,
                              0.2,
                              0.4,
                              1,
                              2,};

    QActionGroup *voltageGroup;
    QActionGroup *audioGroup;

    QList<QSerialPortInfo> portInfo;

    QString connectionStatus;

    QCPRange *defaultRange;
    QString statusBarText[4];

    QCustomPlot *channelGraph[10];
    SocketSylph *socketSylph;

    QAction *isSmart;
    QAction *isDumb;
    QActionGroup *smartOrDumbGroup;

    void createStatusBar();
    void createActions();
    void createMenus();
    void createLayout();
    void connectSylph();
    void setDefaultGraph();
    void activateChannelGraph(int index);
//    void setTimeFrameTickStep(TimeFrames timeframe, double step);
//    void setVoltageTickStep(double position, double size, double step);
    void updateStatusBar(int index, QString message);

private slots:
    void updateData();
    void on_resetX_triggered();
    void on_timeFrame_changed(int timeFrameIndex);
    void on_voltage_changed(int voltageIndex);
//    void on_timeFrame10_triggered();
//    void on_timeFrame20_triggered();
//    void on_timeFrame50_triggered();
//    void on_timeFrame100_triggered();
//    void on_timeFrame200_triggered();
//    void on_timeFrame500_triggered();
//    void on_timeFrame1000_triggered();
//    void on_timeFrame2000_triggered();
//    void on_timeFrame5000_triggered();
    void on_resetY_triggered();
//    void on_voltage50u_triggered();
//    void on_voltage100u_triggered();
//    void on_voltage200u_triggered();
//    void on_voltage500u_triggered();
//    void on_voltage1000u_triggered();
//    void on_voltage2000u_triggered();
//    void on_voltage5000u_triggered();
//    void on_voltage10000u_triggered();
    void on_dataAnalyzer_triggered();
    void on_record_triggered();
    void on_chooseDirectory_triggered();
    void on_playPause_triggered();
    void on_filterConfig_trigger();
    void on_restart_triggered();
    void on_smartDataProcessor_triggered();
    void on_dumbDataProcessor_triggered();
    void on_graph_clicked(int index);
//    void on_graph1_clicked();
//    void on_graph2_clicked();
//    void on_graph3_clicked();
    void about();
};

#endif // MainWindow_H

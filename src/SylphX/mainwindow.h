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
#include "../Odin/odinwindow.h"
#include "../common/datastream.h"
#include "../Cat/catwindow.h"
#include "../common/socketserver.h"

class QComboBox;
class QCustomPlot;

namespace SylphX {

#define DEFAULT_XAXIS 7
#define DEFAULT_YAXIS 3
#define EMG_CHANNELS 10
#define TOTAL_CHANNELS 13

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow();
    ~MainWindow();
    QLabel *statusBarLabel;

    SerialChannel *serialChannel;
    DataProcessor *dataProcessor;
    DataStream *dataStream;
    SocketServer *dynomometer;

    Odin::OdinWindow *x;
    Cat::CatWindow *catGUI;

public slots:

private:
    QElapsedTimer timer;
    QTimer dataTimer;

    int restartCount = 0;
    float samplingRate = 1250.0;
    float period = 1/samplingRate;
    bool pause = false;
    bool dyno = true;

    QStatusBar *statusBarMainWindow;

    QMenu *fileMenu;
    QMenu *voltageMenu;
    QMenu *timeFrameMenu;
    QMenu *audioOutputMenu;
    QMenu *processorMenu;
    QMenu *plotSelectMenu;
    QMenu *helpMenu;

    QVBoxLayout *mainLayout;
    QVBoxLayout *topLayout;
    QVBoxLayout *bottomLayout;
    QWidget *mainWidget;

    QAction *exitAction;
    QAction *recordAction;
    QAction *chooseDirectoryAction;
    QAction *restartAction;
    QAction *pauseAction;
    QAction *resetDefaultX;
    QAction *resetDefaultY;
    QAction *filterAction;
    QAction *dataAnalyzerAction;
    QAction *disableStream;
    QAction *dynoAction;
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
    int currentTimeFrame = DEFAULT_XAXIS;

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
    int currentVoltageScale = DEFAULT_YAXIS;
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
    QSignalMapper *plotSelectMapper;
    QAction *plotSelectAction[EMG_CHANNELS];
    QString plotSelect[EMG_CHANNELS] = {"Channel 1",
                              "Channel 2",
                              "Channel 3",
                              "Channel 4",
                              "Channel 5",
                              "Channel 6",
                              "Channel 7",
                              "Channel 8",
                              "Channel 9",
                              "Channel 10"};
    QAction *plotSelectAll;
    QAction *plotSelectNone;
    QAction *plotSelectDefault;

    bool plotEnable[EMG_CHANNELS] = {false,
                           false,
                           false,
                           true,
                           true,
                           true,
                           true,
                           false,
                           false,
                           false};

    QActionGroup *voltageGroup;

    QAction *audio[11];
    QSignalMapper *audioSelectMapper;
    QAction *aboutAction;
    QAction *odinAction;
    QAction *catAction;
    QAction *isSmart;
    QAction *isDumb;

    QActionGroup *smartOrDumbGroup;
    QActionGroup *audioGroup;

    QList<QSerialPortInfo> portInfo;

    QString statusBarText[4];

    QCustomPlot *channelGraph[NUM_CHANNEL];
    SocketSylph *socketSylph;
    QProcess *pythonProcess;

    void createStatusBar();
    void createActions();
    void createMenus();
    void createLayout();
    void destroyPlots();
    void refreshScreen();
    void connectSylph();
    void setDefaultGraph();
    void activateChannelGraph(int index);
    void updateStatusBar(int index, QString message);

private slots:
    void updateData();
    void on_resetX_triggered();
    void on_timeFrame_changed(int timeFrameIndex);
    void on_voltage_changed(int voltageIndex);
    void on_plotSelect_changed(int channel);
    void on_plotSelectAll_triggered();
    void on_plotSelectNone_triggered();
    void on_plotSelectDefault_triggered();
    void on_resetY_triggered();
    void on_dataAnalyzer_triggered();
    void on_disableStream_triggered();
    void on_record_triggered();
    void on_chooseDirectory_triggered();
    void on_playPause_triggered();
    void on_filterConfig_trigger();
    void on_restart_triggered();
    void on_odin_triggered();
    void on_cat_triggered();
    void on_smartDataProcessor_triggered();
    void on_dumbDataProcessor_triggered();
    void on_graph_clicked(int index);
    void on_dyno_triggered();
    void sendParameter(char *bytes);

    void about();

signals:
    void showOdinSignal();
    void showCatSignal();
};

}

#endif // MainWindow_H

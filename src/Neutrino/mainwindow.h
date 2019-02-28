#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "socketneutrino.h"
#include "command.h"
#include "../common/qtincludes.h"
#include "../common/qcustomplot.h"
#include "commanddialog.h"
#include "dataprocessor.h"
#include "channel.h"
#include "../common/data.h"
#include "../common/filter.h"
#include "serialchannel.h"
#include "../common/filterdialog.h"
#include "../Odin/odinwindow.h"
#include "../common/datastream.h"

#define DEFAULT_XAXIS 4
#define DEFAULT_YAXIS 6

class QComboBox;
class QCustomPlot;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow();
    ~MainWindow();
    QLabel *statusBarLabel;

    Odin::OdinWindow *x;
    DataStream *dataStream;


public slots:


private:
//    QString ipAddress = "192.168.137.102";
//    QString ipAddress = "169.254.68.98";
    QString ipAddress = "192.168.137.74";
//    QString ipAddress = "192.168.4.3";
    int portNumber = 8888;

    QElapsedTimer timer;
    QTimer dataTimer;

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
    QActionGroup *timeFrameGroup;

    QStatusBar *statusBarMainWindow;

    QString connectionStatus;

    QMenu *fileMenu;
    QMenu *voltageMenu;
    QMenu *layoutMenu;
    QMenu *timeFrameMenu;

    void createStatusBar();
    void createActions();
    void createMenus();
    void connectNeutrino();

    QAction *exitAction;
    QAction *recordAction;
    QAction *chooseDirectoryAction;

    QAction *pauseAction;
    bool pause = false;

    QAction *resetDefaultX;
    QAction *resetDefaultY;
    QAction *swapAction;
    QAction *filterAction;
    QAction *dataAnalyzerAction;
    
    QSignalMapper *voltageMapper;
    QAction *voltageAction[7];
    QString voltageActionNames[7] = { "+/- 10mV",
                                      "+/- 20mV",
                                      "+/- 50mV",
                                      "+/- 100mV",
                                      "+/- 200mV",
                                      "+/- 500mV",
                                      "+/- 700mV" };
    double voltageMin[7][2] =   { {-0.0105, 0.4895},
                                   {-0.021, 0.479},
                                   {-0.051, 0.449},
                                   {-0.101, 0.399},
                                   {-0.21, 0.29},
                                   {-0.51, -0.01},
                                   {-0.71, -0.21}};
    double voltageRange[7] = { 0.021,
                               0.042,
                               0.102,
                               0.202,
                               0.42,
                               1.02,
                               1.42};
    double voltageStep[7] = { 0.0025,
                              0.005,
                              0.01,
                              0.02,
                              0.05,
                              0.1,
                              0.25};

    QActionGroup *voltageGroup;

    Odin::SocketOdin *socketOdin;
    SocketNeutrino *socketNeutrino;
    SerialChannel *serialNeutrino;
    Command *NeutrinoCommand;
    DataProcessor *data;
    Channel *NeutrinoChannel;

    QList<QSerialPortInfo> portInfo;

    void create10x1Layout();
    void create5x2Layout();

    QCustomPlot *channelGraph[12];

    QAction *tenby1Action;
    QAction *fiveby2Action;
    QAction *commandAction;
    QAction *inputReferred;
    QPen colors[10] = { QPen(Qt::black),
                        QPen(Qt::darkRed),
                        QPen(Qt::darkGreen),
                        QPen(Qt::darkCyan),
                        QPen(Qt::blue),
                        QPen(Qt::darkBlue),
                        QPen(Qt::darkMagenta),
                        QPen(Qt::darkYellow),
                        QPen(Qt::darkGray),
                        QPen(Qt::red) };

private slots:
    void updateData();
    void on_timeFrame_changed(int timeFrameIndex);
    void on_resetX_triggered();
    void on_voltage_changed(int voltageIndex);
    void on_resetY_triggered();
    void on_record_triggered();
    void on_chooseDirectory_triggered();
    void on_playPause_triggered();
    void on_swap_triggered();
    void on_filterConfig_trigger();
    void on_dataAnalyzer_triggered();
    void on_CommandMenu_triggered();
    void on_tenby1_triggered();
    void on_fiveby2_triggered();
    void on_inputRefer_triggered();

};

#endif // MainWindow_H

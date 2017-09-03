#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

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
#include "signalchannel.h"
#include "serialportdialog.h"
#include "serialchannel.h"
#include "../common/filterdialog.h"

#include "import/cmddialog.h"

#define CHANNEL_TOTAL_NUMBER 8
#define IP_ADDRESS "192.168.4.1"

class QComboBox;
class QCustomPlot;
//class CmdDialog;

class GraphWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit GraphWindow();
    ~GraphWindow();
    QLabel *statusBarLabel;


public slots:


private:
    QList<QColor> colorList = QList<QColor>({Qt::black,Qt::darkRed,Qt::darkGreen,Qt::darkCyan,Qt::blue,Qt::darkBlue,Qt::darkMagenta,Qt::darkYellow});
    CmdDialog *cmdDialog;
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

    QAction *voltage50u;
    QAction *voltage100u;
    QAction *voltage200u;
    QAction *voltage500u;
    QAction *voltage1000u;
    QAction *voltage2000u;
    QAction *voltage5000u;

    QActionGroup *voltageGroup;

    SocketNeutrino *socketNeutrino;
    SerialChannel *serialNeutrino;
    Command *NeutrinoCommand;
    DataProcessor *data;
    Channel *NeutrinoChannel;

    QList<QSerialPortInfo> portInfo;

    void create8x1Layout();
    void create4x2Layout();

    QMenu *connectivityMenu;

    QCustomPlot *channelGraph[8];

    QAction *eightby1Action;
    QAction *fourby2Action;

//    QAction *connectAction;
//    QAction *disconnectAction;
    QAction *commandAction;

    QActionGroup *connectivityGroup;

//    QAction *wiredMode;
//    QAction *wifiMode;

    void createLayoutAction();

    void createVoltageScaleAction();

    void createTimeScaleAction();

    void createFileAction();

    void createConnectivityAction();

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
    void setRangeTime(int timeFrame);
    void setRangeVoltage(int channelTotalNo, float tickRate, float lowerVoltage, float upperVoltage);
    void on_record_triggered();
    void on_chooseDirectory_triggered();
    void on_playPause_triggered();
    void on_resetX_triggered();
    void on_swap_triggered();
    void on_filterConfig_trigger();

//    void on_ConnectMenu_triggered();
//    void on_DisconnectMenu_triggered();
    void on_dataAnalyzer_triggered();
    void on_CommandMenu_triggered();
    void on_8x1_triggered();
    void on_4x2_triggered();
//    void on_wired_triggered();
//    void on_wifi_triggered();

};

#endif // GRAPHWINDOW_H

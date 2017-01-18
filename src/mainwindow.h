#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "socketedison.h"
#include "command.h"
#include "qtincludes.h"
#include "qcustomplot.h"
#include "connectiondialog.h"
#include "commanddialog.h"
#include "filenamedialog.h"
#include "dataprocessor.h"
#include "channel.h"
#include "data.h"
#include "filter.h"
#include "signalchannel.h"
#include "serialportdialog.h"
#include "serialchannel.h"
#include "filterdialog.h"


class QComboBox;
class QCustomPlot;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow();
    ~MainWindow();
    QLabel *statusBarLabel;

#ifdef NEUTRINO_II

#endif //NEUTRINO_II PUBLIC

#ifdef SYLPH
    SerialChannel *serialChannel;
    DataProcessor *data;
#endif //SYLPH PUBLIC

public slots:
#ifdef NEUTRINO_II
#endif //NEUTRINO_II PUBLIC SLOTS

#ifdef SYLPH
#endif //SYLPH PUBLIC SLOTS

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
    QMenu *layoutMenu;
    QMenu *timeFrameMenu;

    void createStatusBar();
    void createActions();
    void createMenus();

    QAction *exitAction;
    QAction *recordAction;
    QAction *chooseDirectoryAction;

    QAction *pauseAction;
    bool pause = false;

    QAction *resetDefaultX;
    QAction *resetDefaultY;

#ifdef NEUTRINO_II
    SocketEdison *socketEdison;
    SerialChannel *serialNeutrino;
    Command *NeutrinoCommand;
    DataProcessor *data;
    Channel *NeutrinoChannel;

    QList<QSerialPortInfo> portInfo;

    void create10x1Layout();
    void create5x2Layout();

    QMenu *connectivityMenu;

    QCustomPlot *channelGraph[10];

    QAction *tenby1Action;
    QAction *fiveby2Action;

    QAction *swapAction;

//    QAction *connectAction;
//    QAction *disconnectAction;
    QAction *commandAction;

    QActionGroup *connectivityGroup;

//    QAction *wiredMode;
//    QAction *wifiMode;
#endif //NEUTRINO_II PRIVATE

#ifdef SYLPH
    int numDataPoints;

    QList<QSerialPortInfo> portInfo;

    QString connectionStatus;

    QCPRange *defaultRange;

    void createLayout();

    QMenu *voltageMenu;
    QMenu *helpMenu;
    QMenu *audioOutputMenu;

    QCustomPlot *channelGraph[10];

    QAction *serialPortAction;
    QAction *filterAction;
    QAction *aboutAction;

    QAction *audio1;
    QAction *audio2;
    QAction *audio3;

    QActionGroup *audioGroup;

    QAction *voltage50u;
    QAction *voltage100u;
    QAction *voltage200u;
    QAction *voltage500u;
    QAction *voltage1000u;
    QAction *voltage2000u;
    QAction *voltage5000u;

    QActionGroup *voltageGroup;
#endif //SYLPH PRIVATE

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
    void on_record_triggered();
    void on_chooseDirectory_triggered();
    void on_playPause_triggered();
    void on_resetX_triggered();

#ifdef NEUTRINO_II
//    void on_ConnectMenu_triggered();
//    void on_DisconnectMenu_triggered();
    void on_swap_triggered();
    void on_CommandMenu_triggered();
    void on_tenby1_triggered();
    void on_fiveby2_triggered();
//    void on_wired_triggered();
//    void on_wifi_triggered();
#endif //NEUTRINO_II PRIVATE SLOTS

#ifdef SYLPH
    void on_filterConfig_trigger();
    void on_resetY_triggered();
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
    void on_graph1_clicked();
    void on_graph2_clicked();
    void on_graph3_clicked();
#endif //SYLPH PRIVATE SLOTS

};

#endif // MainWindow_H

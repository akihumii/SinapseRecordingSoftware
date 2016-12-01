#ifndef MAINWINDOW_KA_H
#define MAINWINDOW_KA_H

#include <QMainWindow>
#include "socketedison.h"
#include "command.h"
#include "qtincludes.h"
#include "qcustomplot.h"
#include "connectiondialog.h"
#include "commanddialog.h"
#include "filenamedialog.h"
#include "dataprocessor_ka.h"
#include "channel.h"
//#include "serialneutrino.h"

class QComboBox;
class QCustomPlot;

class MainWindow_KA : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow_KA();
    ~MainWindow_KA();
    QLabel *statusBarLabel;
public slots:

private slots:
    void on_ConnectMenu_triggered();
    void on_DisconnectMenu_triggered();
    void on_CommandMenu_triggered();
    void on_tenby1_triggered();
    void on_fiveby2_triggered();
    void updateData();
    void on_record_triggered();
    void on_recordFileName_triggered();
    void on_timeFrame10_triggered();
    void on_timeFrame20_triggered();
    void on_timeFrame50_triggered();
    void on_timeFrame100_triggered();
    void on_timeFrame200_triggered();
    void on_timeFrame500_triggered();
    void on_timeFrame1000_triggered();
    void on_timeFrame2000_triggered();
    void on_timeFrame5000_triggered();
    void on_wired_triggered();
    void on_wifi_triggered();

private:
    SocketEdison *socketEdison;
    //SerialNeutrino *serialNeutrino;
    Command *NeutrinoCommand;
    DataProcessor_KA *NeutrinoData;
    Channel *NeutrinoChannel;
    QElapsedTimer timer;
    QTimer dataTimer;

    void createStatusBar();
    void create10x1Layout();
    void create5x2Layout();
    void createActions();
    void createMenus();
    QMenu *fileMenu;
    QMenu *layoutMenu;
    QMenu *timeFrameMenu;
    QMenu *connectivityMenu;

    QCustomPlot *channelGraph[10];

    QAction *tenby1Action;
    QAction *fiveby2Action;

    QAction *connectAction;
    QAction *disconnectAction;
    QAction *commandAction;
    QAction *exitAction;
    QAction *recordAction;
    QAction *recordFileNameAction;

    QActionGroup *timeFrameGroup;

    QAction *timeFrame10ms;
    QAction *timeFrame20ms;
    QAction *timeFrame50ms;
    QAction *timeFrame100ms;
    QAction *timeFrame200ms;
    QAction *timeFrame500ms;
    QAction *timeFrame1000ms;
    QAction *timeFrame2000ms;
    QAction *timeFrame5000ms;

    QActionGroup *connectivityGroup;

    QAction *wiredMode;
    QAction *wifiMode;

    QStatusBar *statusBarMainWindow;

};

#endif // MAINWINDOW_KA_H

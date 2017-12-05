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

class QComboBox;
class QCustomPlot;

namespace SylphX {

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow();
    ~MainWindow();
    QLabel *statusBarLabel;

    SerialChannel *serialChannel;
    DataProcessor *data;

    Odin::OdinWindow *x;

public slots:

private:
    QElapsedTimer timer;
    QTimer dataTimer;

    int restartCount = 0;
    float samplingRate = 1005.0;
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
    QAction *filterAction;
    QAction *dataAnalyzerAction;
    QAction *pythonLaunchAction;
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
    QAction *audio[11];
    QAction *aboutAction;
    QAction *odinAction;
    QAction *isSmart;
    QAction *isDumb;

    QActionGroup *smartOrDumbGroup;
    QActionGroup *timeFrameGroup;
    QActionGroup *voltageGroup;
    QActionGroup *audioGroup;

    QList<QSerialPortInfo> portInfo;

    QString connectionStatus;

    QCustomPlot *channelGraph[12];
    SocketSylph *socketSylph;
    QProcess *pythonProcess;

    void createStatusBar();
    void createActions();
    void createMenus();
    void createLayout();
    void connectSylph();
    void setDefaultGraph();
    void activateChannelGraph(int index);
    void setTimeFrameTickStep(TimeFrames timeframe, double step);
    void setVoltageTickStep(double position, double size, double step);

private slots:
    void updateData();
    void on_resetX_triggered();
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
    void on_voltage10000u_triggered();
    void on_dataAnalyzer_triggered();
    void on_pythonLaunch_triggered();
    void on_record_triggered();
    void on_chooseDirectory_triggered();
    void on_playPause_triggered();
    void on_filterConfig_trigger();
    void on_restart_triggered();
    void on_odin_triggered();
    void on_smartDataProcessor_triggered();
    void on_dumbDataProcessor_triggered();

    void on_graph1_clicked();
    void on_graph2_clicked();
    void on_graph3_clicked();
    void on_graph4_clicked();
    void on_graph5_clicked();
    void on_graph6_clicked();
    void on_graph7_clicked();
    void on_graph8_clicked();
    void on_graph9_clicked();
    void on_graph10_clicked();
    void on_graph11_clicked();
    void about();
};

}

#endif // MainWindow_H

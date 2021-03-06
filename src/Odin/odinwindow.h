#ifndef ODINWINDOW_H
#define ODINWINDOW_H

#include <QMainWindow>
#include "../common/qtincludes.h"
#include "../common/qcustomplot.h"
#include "../common/connectiondialog.h"
#include "../common/led.h"
#include "../common/configurationfile.h"
#include "serialodin.h"
#include "commandodin.h"
#include "socketodin.h"
#include "loopingthread.h"
#include "pulseplot.h"
#include <QtMath>

namespace Odin {

#define STARTDELAY 1000

class OdinWindow : public QMainWindow
{
    Q_OBJECT

public:
    OdinWindow();
    ~OdinWindow();

    QLabel *statusBarLabel;
public slots:
    void sendParameter();

private:
    SerialOdin *serialOdin;
    SocketOdin *socketOdin;
    CommandOdin *commandOdin;
    LoopingThread *loopingThread;
    QTcpServer *tcpServer;
    QTcpSocket *tcpServerConnection;
    ConfigurationFile *configurationFile;

    QWidget *mainWidget;
    QStatusBar *statusBarMainWindow;

    QPushButton *sendParameterButton;
    QPushButton *sendButton;
    QPushButton *modeButton;

    QPushButton *channelSelectAll;
    QPushButton *channelSelectNone;
    QPushButton *thresholdSelectAll;
    QPushButton *thresholdSelectNone;

    QGroupBox *stimParameters;
    QHBoxLayout *stimParaLayout[6];
    QCheckBox *thresholdEnable[4];
    QCheckBox* channelAllEnable;
    QCheckBox *channelEnable[4];
    QLabel *chnLabels[4];
    QLabel *paraLabels[6];
    QDoubleSpinBox *amplitudeSpinBox[4];
    QSpinBox *pulseDurationSpinBox[4];
    QSpinBox *frequencySpinBox;

    QGroupBox *delayParameters;
    QCheckBox *delayEnabledCheckBox;
    QSpinBox *delaySpinBox;

    QGroupBox *thresholdParameters;
    QLabel *thresholdLabels[4];
    QSpinBox *stepSizeSpinBox;
    QDoubleSpinBox *lowerThresholdSpinBox;
    QDoubleSpinBox *upperThresholdSpinBox;
    QSpinBox *debounceSpinBox;

    QList<QSerialPortInfo> portInfo;
    QString connectionStatus;
    QMenu *fileMenu;
    QAction *openSettingsAction;
    QMenu *openSettingsRecentAction;
    QAction *recentFilenameSettings[10];
    QAction *saveSettingsAction;
    QAction *saveSettingsAsAction;
    QMenu *GUIMenu;
    QAction *sylphxAction;
    QAction *catAction;

    QString filenameMostRecent = "odinMostRecent.ini";
    QString filenameSettings = filenameMostRecent;
    QString filenameSettingsTemp;
    QStringList filenameSettingsAll;
    int indexRecentFilenameSettings = 0;
    int indexRecentFilenameAction = 0;
    int indexTemp = -1;
    bool firstLoadingFlag = true;
    QSignalMapper *filenameSettingsAllMapper;
    QString filename;

    Led *sentLED;
    Led *receivedLED;

    bool start = false;
    bool highcurrent = true;
    bool thresholdIncreaseEnable = false;
    bool thresholdDecreaseEnable = false;
    bool delayFlag = false;

    int commandCount = 0;
    int delayValue = 0;
    int delayInterval = 60;
    int numChannelsEnabled = 0;
    char *lastSentCommand = new char[2];
    char *rpiCommand = new char[2];
    double *lastSentAmplitude = new double[4];
    int count = 0;

    void updateFilenameSettingsAll();
    void updateOpenSettingsRecent();
    void readSettings();

    void closeEvent(QCloseEvent *event);

    bool connectOdin();
    void createLayout();
    void createStatusBar();
private slots:
    void createActions();
    void sendCommand();
    void on_currentMode_clicked();
    void on_thresholdEnable_toggled();
    void on_channelEnable_toggled();
    void on_amplitude_Changed();
    void on_pulseDuration_Changed();
    void on_frequency_Changed();
    void on_odinDisconnected();
    void on_delayEnabled_toggled();
    void on_debounce_editted();
    void on_upperThreshold_editted();
    void on_lowerThreshold_editted();
    void on_stepSize_editted();
    void on_channelAThreshold_crossed();
    void on_channelBThreshold_crossed();
    void pauseOdin();
    void acceptConnection();
    void increaseCurrent();
    void on_channelSelectAll_clicked();
    void on_channelSelectNone_clicked();
    void on_thresholdSelectAll_clicked();
    void on_thresholdSelectNone_clicked();
    void on_sylphx_triggered();
    void on_cat_triggered();
    void on_write_settings_changed();
    void on_read_settings_changed();
    void on_read_settings_selected_changed(int index);
    void writeSettings();

signals:
    void commandSent(char *bytes);
    void upperThresholdEditted(double value);
    void lowerThresholdEditted(double value);
    void debounceEditted(int value);
    void amplitudeChanged(double *amplitudes);
    void showSylphXSignal();
    void showCatSignal();
};

}

#endif // ODINWINDOW_H

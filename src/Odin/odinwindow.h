#ifndef ODINWINDOW_H
#define ODINWINDOW_H

#include <QMainWindow>
#include "../common/qtincludes.h"
#include "../common/qcustomplot.h"
#include "../common/connectiondialog.h"
#include "../common/led.h"
#include "serialodin.h"
#include "commandodin.h"
#include "socketodin.h"
#include "loopingthread.h"
#include "pulseplot.h"
#include <QtMath>

namespace Odin {

class OdinWindow : public QMainWindow
{
    Q_OBJECT

public:
    OdinWindow();
    ~OdinWindow();

    QLabel *statusBarLabel;
public slots:

private:
    SerialOdin *serialOdin;
    SocketOdin *socketOdin;
    CommandOdin *commandOdin;

    QStatusBar *statusBarMainWindow;
    QStatusBar *statusBarMainWindow2;

    QPushButton *sendButton;
    QPushButton *characterisationButton;

    QLabel *ModeLabel;
    QComboBox *ModeComboBox;

    QLabel *channelLabel;
    QComboBox *channelComboBox;

    QGroupBox *Parameters;
    QGroupBox *Intensity;
    QGroupBox *Timing;
    QGroupBox *Looping;

    QLabel *pulseMagMainLabel;
    QLabel *pulseMagLabel[5];
    QDoubleSpinBox *pulseMag[5];

    QLabel *pulseDurationLabel;
    QSpinBox *pulseDurationSpinBox;

    QLabel *numPulseLabel;
    QLabel *numPulseLabel2;
    QSpinBox *numPulseSpinBox;

    QLabel *interPulseDurationLabel;
    QSpinBox *interPulseDurationSpinBox;

    QLabel *numPulseTrainLabel;
    QSpinBox *numPulseTrainSpinBox;

    QLabel *interPulseTrainDelayLabel;
    QSpinBox *interPulseTrainDelaySpinBox;

    QLabel *legend;

    QGroupBox *multiChannel;
    QHBoxLayout *channelSeqLayout[4];
    QHBoxLayout *zoneleftLayout[4];
    QHBoxLayout *zonerightLayout[4];
    QGroupBox *sequence;
    QLabel *channelSeqLabel[4];
    QComboBox *channelSeqComboBox[4];
    QGroupBox *zoneDuration;
    QLabel *zoneLabel[7];
    QComboBox *zoneSelector[7];
    QLabel *maskLabel;
    QComboBox *maskSelector;

    QList<QSerialPortInfo> portInfo;
    QString connectionStatus;

    QCustomPlot *pulseGraph;
    PulsePlot *pulsePlot;

    QMessageBox *mbox;
    QMessageBox *mboxStop;
    QAction *connectAction;
    QMenu *fileMenu;

    Led *sentLED;
    Led *receivedLED;

    bool start = false;
    LoopingThread *loopingThread;

    int commandCount = 0;

    bool connectOdin();
    void createLayout();
    void createStatusBar();
    void plotPulse();
    void setDelay();
    void displayError(QByteArray incomingCommand, QByteArray outgoingCommand);
private slots:
    void sendCommand();
    void startCharacterisation();
    void on_Mode_Changed(int Mode);
    void on_channel_Changed(int channel);
    void on_pulseMag_Changed();
    void on_pulseDuration_Changed();
    void on_numPulse_Changed();
    void on_interPulseDuration_Changed();
    void on_channelSeq_Changed();
    void on_zoneSelector_Changed();
    void on_zoneMask_Changed();
    void on_numPulseTrain_Changed();
    void on_interPulseTrainDelay_Changed();
    void on_finishedSending();
    void on_commandSent();
    void on_odinDisconnected();
    void on_ConnectMenu_triggered();
    void on_commandReceived(bool received);

signals:
    void commandSent();
};

}

#endif // ODINWINDOW_H

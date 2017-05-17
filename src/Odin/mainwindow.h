#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../common/qtincludes.h"
#include "serialodin.h"
#include "commandodin.h"
#include "socketodin.h"
#include "loopingthread.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow();
    ~MainWindow();

    QLabel *statusBarLabel;
public slots:

private:
    SerialOdin *serialOdin;
    SocketOdin *socketOdin;
    CommandOdin *commandOdin;

    QStatusBar *statusBarMainWindow;

    QPushButton *sendButton;

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

    QMessageBox *mbox;

    bool start = false;
    LoopingThread *loopingThread;

    int commandCount = 0;

    void createLayout();
    void createStatusBar();
private slots:
    void sendCommand();
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
};

#endif // MAINWINDOW_H

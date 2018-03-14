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
    LoopingThread *loopingThread;

    QStatusBar *statusBarMainWindow;

    QPushButton *sendButton;
    QPushButton *recordButton;

    QGroupBox *stimParameters;
    QHBoxLayout *stimParaLayout[5];
    QCheckBox *thresholdEnable[4];
    QLabel *chnLabels[4];
    QLabel *paraLabels[5];
    QDoubleSpinBox *amplitudeSpinBox[4];
    QSpinBox*pulseDurationSpinBox[4];
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

    Led *sentLED;
    Led *receivedLED;

    bool start = false;
    bool record = false;

    int commandCount = 0;
    int numChannelsEnabled = 0;

    bool connectOdin();
    void createLayout();
    void createStatusBar();
private slots:
    void sendCommand();
    void on_record_clicked();
    void on_thresholdEnable_toggled();
    void on_amplitude_Changed();
    void on_pulseDuration_Changed();
    void on_frequency_Changed();
    void on_odinDisconnected();
    void on_delayEnabled_toggled();
    void on_debounce_editted();
    void on_upperThreshold_editted();
    void on_lowerThreshold_editted();
    void on_stepSize_editted();
    void on_upperThreshold_crossed();
    void on_lowerThreshold_crossed();

signals:
    void commandSent();
    void upperThresholdEditted(double value);
    void lowerThresholdEditted(double value);
    void debounceEditted(int value);
};

}

#endif // ODINWINDOW_H

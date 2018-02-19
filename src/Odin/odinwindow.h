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

    QPushButton *sendButton;

    QGroupBox *stimParameters;
    QHBoxLayout *stimParaLayout[5];
    QCheckBox *thresholdEnable[4];
    QLabel *chnLabels[4];
    QLabel *paraLabels[5];
    QDoubleSpinBox *amplitudeSpinBox[4];
    QSpinBox*pulseDurationSpinBox[4];
    QSpinBox *frequencySpinBox;

    QList<QSerialPortInfo> portInfo;
    QString connectionStatus;
    QMenu *fileMenu;

    Led *sentLED;
    Led *receivedLED;

    bool start = false;

    int commandCount = 0;

    bool connectOdin();
    void createLayout();
    void createStatusBar();
private slots:
    void sendCommand();
    void on_thresholdEnable_toggled();
    void on_amplitude_Changed();
    void on_pulseDuration_Changed();
    void on_frequency_Changed();
    void on_odinDisconnected();

signals:
    void commandSent();
};

}

#endif // ODINWINDOW_H
